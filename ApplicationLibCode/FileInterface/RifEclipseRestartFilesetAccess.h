/////////////////////////////////////////////////////////////////////////////////

/// @file
/// Declares restart-result access for cases stored as one file per report step.
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

#include <vector>

class RifEclipseOutputFileTools;

//==================================================================================================
//
/// Reads an ordered set of Eclipse `.Xnnnn` restart files as one time series.
///
/// File handles are opened lazily per time step and retained for repeated result
/// queries. The parallel vectors in this class preserve the mapping between sorted
/// file names, timestamps, elapsed days, and ERT handles.
//==================================================================================================
class RifEclipseRestartFilesetAccess : public RifEclipseRestartDataAccess
{
public:
    RifEclipseRestartFilesetAccess();
    ~RifEclipseRestartFilesetAccess() override;

    /// Opens all configured report-step files and discovers available phases.
    bool open() override;
    /// Sorts and installs the restart file set, resetting all open handles.
    void setRestartFiles( const QStringList& fileSet ) override;
    /// Leaves lazy handles managed by the object lifetime; retained for interface compatibility.
    void close() override;

    /// Supplies timestamps already obtained from related case metadata.
    void             setTimeSteps( const std::vector<QDateTime>& timeSteps ) override;
    /// @return Number of exposed report steps.
    size_t           timeStepCount() override;
    /// Returns cached timestamps or extracts one timestamp from each restart file.
    void             timeSteps( std::vector<QDateTime>* timeSteps, std::vector<double>* daysSinceSimulationStart ) override;
    /// Returns report numbers for files that have already been opened.
    std::vector<int> reportNumbers() override;

    /// Aggregates keyword counts across all report-step files.
    std::vector<RifEclipseKeywordValueCount> keywordValueCounts() override;
    /// Loads and concatenates all grid occurrences of a result at @p timeStep.
    bool results( const QString& resultName, size_t timeStep, size_t gridCount, std::vector<double>* values ) override;

    /// Loads phase fluxes for dynamic NNCs from one report-step file.
    bool dynamicNNCResults( const ecl_grid_type* grid,
                            size_t               timeStep,
                            std::vector<double>* waterFlux,
                            std::vector<double>* oilFlux,
                            std::vector<double>* gasFlux ) override;

    /// Merges well data from every report step into @p well_info.
    void readWellData( well_info_type* well_info, bool importCompleteMswData ) override;
    /// Reads the unit-system code from the first report-step file.
    int readUnitsType() override;

    /// @return Union of phases discovered in opened files.
    std::set<RiaDefines::PhaseType> availablePhases() const override;

private:
    void openTimeStep( size_t timeStep ); ///< Lazily opens one report-step file and records its phases.
    static int reportNumber( const ecl_file_type* ecl_file ); ///< Extracts report number from the restart filename.

private:
    QStringList            m_fileNames;                ///< Restart files sorted by `.Xnnnn` suffix.
    std::vector<QDateTime> m_timeSteps;                ///< Cached timestamp for each file.
    std::vector<double>    m_daysSinceSimulationStart; ///< Cached elapsed days for each file.

    std::vector<ecl_file_type*>     m_ecl_files;       ///< Lazy ERT handles parallel to m_fileNames.
    std::set<RiaDefines::PhaseType> m_availablePhases; ///< Union accumulated as files are opened.
};
