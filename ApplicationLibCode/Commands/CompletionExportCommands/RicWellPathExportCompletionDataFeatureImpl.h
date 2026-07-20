/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017 Statoil ASA
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
/// @file
/// @brief Declares well path export completion data command support.

#pragma once

#include "RigCompletionData.h"

#include "RicExportCompletionDataSettingsUi.h"

#include "cvfVector2.h"
#include "cvfVector3.h"

#include <QDateTime>
#include <QFile>

#include <memory>
#include <optional>
#include <vector>

class RigCell;
class RigEclipseCaseData;
class RigMainGrid;
class RimEclipseCase;
class RimFishbones;
class RimPerforationInterval;
class RimWellPath;
class RimWellPathValve;
class RimWellPathFracture;
class RimNonDarcyPerforationParameters;
class RifTextDataTableFormatter;
class RigVirtualPerforationTransmissibilities;
class SubSegmentIntersectionInfo;

//==================================================================================================
/// @brief Shared file handle used by completion-data export helpers.
//==================================================================================================
using QFilePtr = std::shared_ptr<QFile>;

//==================================================================================================
/// @brief Shared implementation helpers for well path export completion data commands.
//==================================================================================================
class RicWellPathExportCompletionDataFeatureImpl
{
public:
    /// Exports completions.
    static void exportCompletions( const std::vector<RimWellPath*>& wellPaths, const RicExportCompletionDataSettingsUi& exportSettings );

    /// Computes static completions for well path.
    static std::vector<RigCompletionData> computeStaticCompletionsForWellPath( RimWellPath* wellPath, RimEclipseCase* eclipseCase );

    /// Computes dynamic completions for well path.
    static std::vector<RigCompletionData>
        computeDynamicCompletionsForWellPath( RimWellPath* wellPath, RimEclipseCase* eclipseCase, size_t timeStepIndex );

    /// @return The completion data for well path.
    static std::vector<RigCompletionData>
        completionDataForWellPath( RimWellPath* wellPath, RimEclipseCase* eCase, const std::optional<QDateTime>& exportDate = std::nullopt );

    /// @return The well path upper grid intersection IJ.
    static std::pair<double, cvf::Vec2i>
        wellPathUpperGridIntersectionIJ( const RimEclipseCase* gridCase, const RimWellPath* wellPath, const QString& gridName = "" );

    /// Exports date for time step.
    static std::optional<QDateTime> exportDateForTimeStep( const RimEclipseCase* eclipseCase, size_t timeStepIndex );

private:
    /// @return The generate perforations compdat values.
    static std::vector<RigCompletionData> generatePerforationsCompdatValues( const RimWellPath*                                wellPath,
                                                                             const std::vector<const RimPerforationInterval*>& intervals,
                                                                             const RicExportCompletionDataSettingsUi&          settings,
                                                                             const std::optional<QDateTime>& exportDate = std::nullopt );

    /// @return The combine Eclipse cell completions.
    static RigCompletionData combineEclipseCellCompletions( const std::vector<RigCompletionData>&    completions,
                                                            const RicExportCompletionDataSettingsUi& settings );

    /// @return The main grid completions.
    static std::vector<RigCompletionData> mainGridCompletions( const std::vector<RigCompletionData>& allCompletions );

    /// @return The sub grids completions.
    static std::map<QString, std::vector<RigCompletionData>> subGridsCompletions( const std::vector<RigCompletionData>& allCompletions );

    /// Exports well path fracture report.
    static void exportWellPathFractureReport( RimEclipseCase*                                   sourceCase,
                                              QFilePtr                                          exportFile,
                                              const std::vector<RicWellPathFractureReportItem>& wellPathFractureReportItems );

    /// Exports welspecs to file.
    static void exportWelspecsToFile( RimEclipseCase*                       gridCase,
                                      QFilePtr                              exportFile,
                                      const std::vector<RigCompletionData>& completions,
                                      bool                                  exportDataSourceAsComment );

    /// Exports welspecl to file.
    static void exportWelspeclToFile( RimEclipseCase*                                          gridCase,
                                      QFilePtr                                                 exportFile,
                                      const std::map<QString, std::vector<RigCompletionData>>& completions );

    /// Performs the sort and export completions to file command operation.
    static void sortAndExportCompletionsToFile( RimEclipseCase*                                      eclipseCase,
                                                const QString&                                       exportFolder,
                                                const QString&                                       fileName,
                                                const std::vector<RigCompletionData>&                completions,
                                                const std::vector<RicWellPathFractureReportItem>&    wellPathFractureReportItems,
                                                RicExportCompletionDataSettingsUi::CompdatExportType exportType,
                                                bool                                                 exportDataSourceAsComment,
                                                bool                                                 exportWelspec );

    /// Exports compdat and wpimult tables.
    static void exportCompdatAndWpimultTables( RimEclipseCase*                                          sourceCase,
                                               QFilePtr                                                 exportFile,
                                               const std::map<QString, std::vector<RigCompletionData>>& completionsPerGrid,
                                               RicExportCompletionDataSettingsUi::CompdatExportType     exportType,
                                               bool                                                     exportDataSourceAsComment );

    /// Exports compdat table using formatter.
    static void exportCompdatTableUsingFormatter( RifTextDataTableFormatter&            formatter,
                                                  const QString&                        gridName,
                                                  const std::vector<RigCompletionData>& completionData );

    /// Exports complump table using formatter.
    static void exportComplumpTableUsingFormatter( RifTextDataTableFormatter&            formatter,
                                                   const QString&                        gridName,
                                                   const std::vector<RigCompletionData>& completionData );

    /// Exports wpimult table using formatter.
    static void exportWpimultTableUsingFormatter( RifTextDataTableFormatter&            formatter,
                                                  const QString&                        gridName,
                                                  const std::vector<RigCompletionData>& completionData );

    /// Appends completion data.
    static void appendCompletionData( std::map<size_t, std::vector<RigCompletionData>>* completionData,
                                      const std::vector<RigCompletionData>&             data );

    /// Exports carfin for temporary lgrs.
    static void exportCarfinForTemporaryLgrs( const RimEclipseCase* sourceCase, const QString& folder );

    /// @return The top level well path.
    static RimWellPath* topLevelWellPath( const RigCompletionData& completion );
};
