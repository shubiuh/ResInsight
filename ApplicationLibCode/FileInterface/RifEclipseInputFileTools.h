/////////////////////////////////////////////////////////////////////////////////

/// @file
/// Declares Eclipse deck/grid import, export, keyword, INCLUDE, and fault utilities.
//
//  Copyright (C) 2011-     Statoil ASA
//  Copyright (C) 2013-     Ceetron Solutions AS
//  Copyright (C) 2011-2012 Ceetron AS
//
//  ResInsight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ResInsight is distributed in the hope that it will be useful, but WITHOUT ANY
//  WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
//
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "cvfCollection.h"
#include "cvfObject.h"

#include "RiaDefines.h"
#include "RigFault.h"
#include "RigRefinement.h"

#include "ert/ecl/ecl_kw.h"

#include <expected>
#include <map>
#include <string>

#include <QString>

class RigEclipseCaseData;
class QFile;
class QTextStream;

//--------------------------------------------------------------------------------------------------
/// Cached location of a keyword occurrence in a text deck.
//--------------------------------------------------------------------------------------------------
struct RifKeywordAndFilePos
{
    QString keyword; ///< Normalized keyword token.
    qint64  filePos; ///< Byte position immediately after the keyword line.
};

//==================================================================================================
//
/// Stateless utilities for Eclipse text keyword files and grid export.
///
/// The class combines geometry import/export with deck-aware parsing. In particular,
/// fault discovery follows nested INCLUDE statements and PATHS aliases while guarding
/// against edit-section boundaries. Extraction helpers also apply crop/refinement
/// mappings so exported keywords and faults remain aligned with exported grids.
//==================================================================================================
class RifEclipseInputFileTools : public cvf::Object
{
public:
    RifEclipseInputFileTools();
    ~RifEclipseInputFileTools() override;

    /// Opens a text/binary grid, populates @p eclipseCase, and optionally imports faults.
    static bool openGridFile( const QString& fileName, RigEclipseCaseData* eclipseCase, bool readFaultData, QString* errorMessages );

    /// Exports grid geometry and ACTNUM for a cropped/refined region.
    static bool exportGrid( const QString&         gridFileName,
                            RigEclipseCaseData*    eclipseCase,
                            bool                   exportInLocalCoordinates,
                            const cvf::UByteArray* cellVisibilityOverrideForActnum = nullptr,
                            const cvf::Vec3st&     min                             = cvf::Vec3st::ZERO,
                            const cvf::Vec3st&     max                             = cvf::Vec3st::UNDEFINED,
                            const cvf::Vec3st&     refinement                      = cvf::Vec3st( 1, 1, 1 ) );

    /// Exports selected result keywords using the same crop/refinement convention as grid export.
    static bool exportKeywords( const QString&              resultFileName,
                                RigEclipseCaseData*         eclipseCase,
                                const std::vector<QString>& keywords,
                                bool                        writeEchoKeywords,
                                const cvf::Vec3st&          min        = cvf::Vec3st::ZERO,
                                const cvf::Vec3st&          max        = cvf::Vec3st::UNDEFINED,
                                const cvf::Vec3st&          refinement = cvf::Vec3st( 1, 1, 1 ) );

    /// Extracts one result keyword for a regularly refined export region.
    static std::expected<std::vector<double>, std::string> extractKeywordData( RigEclipseCaseData* eclipseCase,
                                                                               const QString&      keyword,
                                                                               const cvf::Vec3st&  min        = cvf::Vec3st::ZERO,
                                                                               const cvf::Vec3st&  max        = cvf::Vec3st::UNDEFINED,
                                                                               const cvf::Vec3st&  refinement = cvf::Vec3st( 1, 1, 1 ) );

    /// Extracts one result keyword using an explicit refinement description.
    static std::expected<std::vector<double>, std::string> extractKeywordData( RigEclipseCaseData*  eclipseCase,
                                                                               const QString&       keyword,
                                                                               const cvf::Vec3st&   min,
                                                                               const cvf::Vec3st&   max,
                                                                               const RigRefinement& refinement );

    /// Writes one named fault to @p completeFilename using Eclipse FAULTS syntax.
    static void saveFault( QString                                 completeFilename,
                           const RigMainGrid*                      mainGrid,
                           const std::vector<RigFault::FaultFace>& faultFaces,
                           QString                                 faultName,
                           const cvf::Vec3st&                      min        = cvf::Vec3st::ZERO,
                           const cvf::Vec3st&                      max        = cvf::Vec3st::UNDEFINED,
                           const cvf::Vec3st&                      refinement = cvf::Vec3st( 1, 1, 1 ) );

    /// Writes one named fault to an existing text stream.
    static void saveFault( QTextStream&                            stream,
                           const RigMainGrid*                      mainGrid,
                           const std::vector<RigFault::FaultFace>& faultFaces,
                           QString                                 faultName,
                           const cvf::Vec3st&                      min        = cvf::Vec3st::ZERO,
                           const cvf::Vec3st&                      max        = cvf::Vec3st::UNDEFINED,
                           const cvf::Vec3st&                      refinement = cvf::Vec3st( 1, 1, 1 ) );

    /// Writes every main-grid fault to an existing text stream.
    static void saveFaults( QTextStream&       stream,
                            const RigMainGrid* mainGrid,
                            const cvf::Vec3st& min        = cvf::Vec3st::ZERO,
                            const cvf::Vec3st& max        = cvf::Vec3st::UNDEFINED,
                            const cvf::Vec3st& refinement = cvf::Vec3st( 1, 1, 1 ) );

    /// Maps fault faces into a regularly refined/cropped export grid.
    static std::vector<RigFault::CellAndFace> extractFaults( const RigMainGrid*                      mainGrid,
                                                             const std::vector<RigFault::FaultFace>& faultFaces,
                                                             const cvf::Vec3st&                      min        = cvf::Vec3st::ZERO,
                                                             const cvf::Vec3st&                      max        = cvf::Vec3st::UNDEFINED,
                                                             const cvf::Vec3st&                      refinement = cvf::Vec3st( 1, 1, 1 ) );

    /// Maps fault faces using an explicit refinement description.
    static std::vector<RigFault::CellAndFace> extractFaults( const RigMainGrid*                      mainGrid,
                                                             const std::vector<RigFault::FaultFace>& faultFaces,
                                                             const cvf::Vec3st&                      min,
                                                             const cvf::Vec3st&                      max,
                                                             const RigRefinement&                    refinement );

    /// Imports fault definitions from a deck or include file into @p eclipseCase.
    static bool importFaultsFromFile( RigEclipseCaseData* eclipseCase, const QString& fileName );

    /// Reads grid-section faults and reports include files that supplied definitions.
    static void readFaultsInGridSection( const QString&             fileName,
                                         cvf::Collection<RigFault>* faults,
                                         std::vector<QString>*      filenamesWithFaults,
                                         const QString&             faultIncludeFileAbsolutePathPrefix );
    /// Parses all FAULTS occurrences in one file without following a parent deck.
    static void parseAndReadFaults( const QString& fileName, cvf::Collection<RigFault>* faults );

    /// Reads PATHS aliases used to resolve subsequent INCLUDE statements.
    static void parseAndReadPathAliasKeyword( const QString& fileName, std::vector<std::pair<QString, QString>>* pathAliasDefinitions );

    /// Reads one keyword's content, following nested includes until @p keywordToStopParsing.
    static QStringList readKeywordContentFromFile( const QString& keyword, const QString& keywordToStopParsing, QFile& file );

    /// Recursive implementation exposed for parser unit tests; normal clients use readKeywordContentFromFile().
    static bool readKeywordAndParseIncludeStatementsRecursively( const QString&                                  keyword,
                                                                 const QString&                                  keywordToStopParsing,
                                                                 const std::vector<std::pair<QString, QString>>& pathAliasDefinitions,
                                                                 const QString&        includeStatementAbsolutePathPrefix,
                                                                 QFile&                file,
                                                                 qint64                startPos,
                                                                 QStringList&          keywordDataContent,
                                                                 std::vector<QString>& filenamesContainingKeyword,
                                                                 bool&                 isStopParsingKeywordDetected );

    /// Decodes GRIDUNIT content at @p gridunitPos.
    static RiaDefines::EclipseUnitSystem readUnitSystem( QFile& file, qint64 gridunitPos );

    /// Maps Eclipse X/Y/Z or I/J/K face text to a structured-grid face.
    static cvf::StructGridInterface::FaceEnum faceEnumFromText( const QString& faceString );

    /// @return Whether @p fileName contains a COORD grid keyword.
    static bool hasGridData( const QString& fileName );

    /// Imports PFLOTRAN connection/fault definitions from @p fileName.
    static void parsePflotranInputFile( const QString& fileName, cvf::Collection<RigFault>* faults );

private:
    /// Parses one FAULTS data block beginning at @p filePos.
    static void readFaults( QFile& data, qint64 filePos, cvf::Collection<RigFault>* faults, bool* isEditKeywordDetected );

    /// Reads FAULTS blocks identified by a pre-scanned keyword-position table.
    static void readFaults( const QString& fileName, const std::vector<RifKeywordAndFilePos>& fileKeywords, cvf::Collection<RigFault>* faults );
    /// Traverses INCLUDE statements while collecting faults and edit-section state.
    static bool readFaultsAndParseIncludeStatementsRecursively( QFile&                                          file,
                                                                qint64                                          startPos,
                                                                const std::vector<std::pair<QString, QString>>& pathAliasDefinitions,
                                                                cvf::Collection<RigFault>*                      faults,
                                                                std::vector<QString>*                           filenamesWithFaults,
                                                                bool*                                           isEditKeywordDetected,
                                                                const QString& faultIncludeFileAbsolutePathPrefix );

    /// Reads data lines until slash terminator, a new keyword, or EDIT boundary.
    static void readKeywordDataContent( QFile& data, qint64 filePos, QStringList& textContent, bool& isStopParsingKeywordDetected );

    /// Locates geometry, activity, map-axis, and unit keywords in a scanned deck.
    static void findGridKeywordPositions( const std::vector<RifKeywordAndFilePos>& keywords,
                                          qint64*                                  coordPos,
                                          qint64*                                  zcornPos,
                                          qint64*                                  specgridPos,
                                          qint64*                                  actnumPos,
                                          qint64*                                  mapaxesPos,
                                          qint64*                                  gridunitPos );

    static size_t findFaultByName( const cvf::Collection<RigFault>& faults, const QString& name ); ///< Returns fault index or undefined size.

    /// Scans forward from @p startPos for a standalone keyword.
    static qint64 findKeyword( const QString& keyword, QFile& file, qint64 startPos );

    /// Writes one contiguous K range in Eclipse FAULTS record format.
    static void writeFaultLine( QTextStream&                       stream,
                                QString                            faultName,
                                size_t                             i,
                                size_t                             j,
                                size_t                             startK,
                                size_t                             endK,
                                cvf::StructGridInterface::FaceType faceType );

    static QString faultFaceText( cvf::StructGridInterface::FaceType faceType ); ///< Converts a face enum to Eclipse face notation.
};
