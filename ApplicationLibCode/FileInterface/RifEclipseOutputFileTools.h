/////////////////////////////////////////////////////////////////////////////////

/// @file
/// Declares utilities for discovering and decoding binary Eclipse output files.
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

#include "RifEclipseReportKeywords.h"

#include "RiaDefines.h"
#include "RiaPorosityModel.h"

#include "ert/ecl/ecl_file_view.h"
#include "ert/ecl/ecl_grid.h"
#include "ert/ecl/ecl_util.h"

#include "cvfObject.h"

#include <QDateTime>
#include <QString>
#include <QStringList>

#include <optional>
#include <vector>

using ecl_file_type = struct ecl_file_struct;

class RifEclipseRestartDataAccess;
class RigEclipseTimeStepInfo;
class RigActiveCellInfo;
class RigEclipseCaseData;

class QByteArray;

//==================================================================================================
/// Stateless helpers for Eclipse grid, INIT, and restart output files.
///
/// This class isolates direct ERT interactions: file-family discovery, keyword
/// extraction, report-step metadata, active-cell mapping, unit reconciliation,
/// dynamic-result backend selection, and NNC flux transfer. Keeping these rules
/// centralized ensures all concrete readers interpret Eclipse files consistently.
//==================================================================================================
class RifEclipseOutputFileTools
{
public:
    RifEclipseOutputFileTools();
    virtual ~RifEclipseOutputFileTools();

    /// Aggregates keyword names, types, and value counts over @p ecl_files.
    static std::vector<RifEclipseKeywordValueCount> keywordValueCounts( const std::vector<ecl_file_type*>& ecl_files );

    /// Creates result metadata entries compatible with case active-cell layouts and time steps.
    static void createResultEntries( const std::vector<RifEclipseKeywordValueCount>& fileKeywordInfo,
                                     const std::vector<RigEclipseTimeStepInfo>&      timeStepInfo,
                                     RiaDefines::ResultCatType                       resultCategory,
                                     RigEclipseCaseData*                             eclipseCaseData,
                                     size_t                                          totalTimeSteps );

    /// Reads one floating-point keyword occurrence, converting supported storage types to double.
    static bool keywordData( const ecl_file_type* ecl_file, const QString& keyword, size_t fileKeywordOccurrence, std::vector<double>* values );
    /// Reads one integer keyword occurrence.
    static bool keywordData( const ecl_file_type* ecl_file, const QString& keyword, size_t fileKeywordOccurrence, std::vector<int>* values );

    /// Extracts report-step timestamps, elapsed days, and header multiplicity.
    static void timeSteps( const ecl_file_type*    ecl_file,
                           std::vector<QDateTime>* timeSteps,
                           std::vector<double>*    daysSinceSimulationStart,
                           size_t*                 perTimeStepHeaderKeywordCount );

    static bool isValidEclipseFileName( const QString& fileName ); ///< Tests whether ERT recognizes the path as an Eclipse output filename.
    static QByteArray md5sum( const QString& fileName ); ///< Computes a streaming MD5 digest for cache/index validation.
    /// Finds the Eclipse file family sharing @p fileName's base case name.
    static bool findSiblingFilesWithSameBaseName( const QString& fileName, QStringList* fileSet );

    static QString firstFileNameOfType( const QStringList& fileSet, ecl_file_enum fileType ); ///< Returns the first matching file or an empty string.
    static QStringList filterFileNamesOfType( const QStringList& fileSet, ecl_file_enum fileType ); ///< Filters a case file family by ERT type.

    /// Reads dimensions for the main grid and local refinements without importing full geometry.
    static void readGridDimensions( const QString& gridFileName, std::vector<std::vector<int>>& gridDimensions );

    /// Reads the Eclipse unit-system integer from an opened output file.
    static int readUnitsType( const ecl_file_type* ecl_file );

    /// Chooses unified or split-file dynamic-result access from @p fileName.
    static cvf::ref<RifEclipseRestartDataAccess> createDynamicResultAccess( const QString& fileName );

    /// Derives the sidecar result-index filename used for accelerated opening.
    static QString createIndexFileName( const QString& resultFileName );

    /// Detects fluid phases represented by restart keywords.
    static std::set<RiaDefines::PhaseType> findAvailablePhases( const ecl_file_type* ecl_file );

    /// Transfers water, oil, and gas NNC flux arrays from an ERT report-step view.
    static void transferNncFluxData( const ecl_grid_type*      grid,
                                     const ecl_file_view_type* summaryView,
                                     std::vector<double>*      waterFlux,
                                     std::vector<double>*      oilFlux,
                                     std::vector<double>*      gasFlux );

    /// @return Whether file metadata identifies an Intersect-exported case.
    static bool isExportedFromIntersect( const ecl_file_type* ecl_file );

    /// Opens a path using platform-correct encoding while retaining stdio semantics.
    static FILE* fopen( const QString& filePath, const QString& mode );

    /// Assigns ACTNUM arrays to the main grid and local grids in @p eclipseCaseData.
    static bool assignActiveCellData( std::vector<std::vector<int>>& actnumValuesPerGrid, RigEclipseCaseData* eclipseCaseData );

    /// Filters keywords whose value counts match the selected active-cell/porosity layout.
    static std::vector<RifEclipseKeywordValueCount>
        validKeywordsForPorosityModel( const std::vector<RifEclipseKeywordValueCount>& keywordItemCounts,
                                       const RigActiveCellInfo*                        activeCellInfo,
                                       const RigActiveCellInfo*                        fractureActiveCellInfo,
                                       RiaDefines::PorosityModelType                   matrixOrFracture,
                                       size_t                                          timeStepCount );

    /// Converts file-order values into the selected matrix or fracture active-cell ordering.
    static void extractResultValuesBasedOnPorosityModel( RigEclipseCaseData*           eclipseCaseData,
                                                         RiaDefines::PorosityModelType matrixOrFracture,
                                                         std::vector<double>*          values,
                                                         const std::vector<double>&    fileValues );

    /// Maps a raw Eclipse unit integer to the application enum when recognized.
    static std::optional<RiaDefines::EclipseUnitSystem> unitValueToEnum( int unitValue );
    /// Reconciles unit declarations from EGRID, INIT, and UNRST sources.
    static RiaDefines::EclipseUnitSystem                determineUnitSystem( const std::optional<RiaDefines::EclipseUnitSystem>& egridUnit,
                                                                             const std::optional<RiaDefines::EclipseUnitSystem>& initUnit,
                                                                             const std::optional<RiaDefines::EclipseUnitSystem>& unrstUnit );

private:
    static void getDayMonthYear( const ecl_kw_type* intehead_kw, int* day, int* month, int* year ); ///< Decodes date fields from INTEHEAD.
    static RifEclipseReportKeywords createReportStepsMetaData( const std::vector<ecl_file_type*>& ecl_files ); ///< Aggregates keyword metadata per report step.
};
