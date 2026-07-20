/////////////////////////////////////////////////////////////////////////////////

/// @file
/// Declares restart-result access for a unified Eclipse restart file.
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

#include "RifEclipseRestartDataAccess.h"

class RifEclipseOutputFileTools;

// typedef struct ecl_file_struct ecl_file_type;

#include "ert/ecl_well/well_info.hpp"

//==================================================================================================
//
/// Reads all report steps stored in a single Eclipse `.UNRST` file.
///
/// The reader builds time-step views over the unified file and accounts for
/// simulator-specific empty header blocks. When available, an ERT index file is
/// used to reduce open time; grid-count updates refine the no-data block handling.
//==================================================================================================
class RifEclipseUnifiedRestartFileAccess : public RifEclipseRestartDataAccess
{
public:
    RifEclipseUnifiedRestartFileAccess();
    ~RifEclipseUnifiedRestartFileAccess() override;

    void setRestartFiles( const QStringList& fileSet ) override; ///< Selects the unified restart file from @p fileSet.
    bool open() override;  ///< Opens the unified source and extracts time metadata.
    void close() override; ///< Clears cached time metadata; the handle remains reusable until destruction.

    size_t timeStepCount() override; ///< @return Number of report steps detected in the file.
    /// Returns timestamps and elapsed simulation days in report-step order.
    void             timeSteps( std::vector<QDateTime>* timeSteps, std::vector<double>* daysSinceSimulationStart ) override;
    std::vector<int> reportNumbers() override; ///< @return Report numbers parallel to the time-step vectors.

    std::vector<RifEclipseKeywordValueCount> keywordValueCounts() override; ///< Aggregates keyword metadata over report-step views.
    /// Loads all grid occurrences of @p resultName from one report-step view.
    bool results( const QString& resultName, size_t timeStep, size_t gridCount, std::vector<double>* values ) override;

    /// Loads phase fluxes for dynamic NNCs from one report-step view.
    bool dynamicNNCResults( const ecl_grid_type* grid,
                            size_t               timeStep,
                            std::vector<double>* waterFlux,
                            std::vector<double>* oilFlux,
                            std::vector<double>* gasFlux ) override;

    void readWellData( well_info_type* well_info, bool importCompleteMswData ) override; ///< Imports wells across all report steps.
    int readUnitsType() override; ///< @return Unit-system code stored in the restart headers.

    std::set<RiaDefines::PhaseType> availablePhases() const override; ///< @return Phases detected while opening the file.

    /// Updates empty-block accounting after the case grid hierarchy is known.
    void updateFromGridCount( size_t gridCount ) override;

private:
    bool openFile();                    ///< Opens the ERT handle, optionally using an index sidecar.
    bool useResultIndexFile() const;    ///< Determines whether indexed opening is enabled and usable.
    void extractTimestepsFromEclipse(); ///< Builds timestamps, elapsed days, and report-number vectors.

private:
    QString        m_filename;               ///< Unified restart path.
    ecl_file_type* m_ecl_file;               ///< Owned ERT file handle while open.
    size_t         m_perTimeStepHeaderCount; ///< Header occurrences separating report-step views.
    size_t         m_noDataGridCount;        ///< Grid blocks carrying headers but no result payload.

    std::vector<QDateTime> m_timeSteps;                ///< Cached report-step timestamps.
    std::vector<double>    m_daysSinceSimulationStart; ///< Cached elapsed days.
    std::vector<int>       m_reportNr;                 ///< Cached simulator report numbers.

    std::set<RiaDefines::PhaseType> m_availablePhases; ///< Phases advertised by restart keywords.
};
