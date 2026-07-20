/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2025     Equinor ASA
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
/// @brief OPM Flow external simulator preferences.

#pragma once

#include "cafPdmField.h"
#include "cafPdmObject.h"

class RimOpmFlowJobSettings;

//--------------------------------------------------------------------------------------------------
/// @brief Preferences for running OPM Flow as an external reservoir simulator.
///
/// RiaPreferencesOpm configures the OPM Flow executable path, WSL integration, MPI parallelism,
/// maximum parallel job count, and default job settings for launching Flow simulations from
/// within ResInsight. WSL distribution choices are discovered at runtime and exposed through PDM
/// option calculation. createDefaultJobSettings() returns a new job-specific copy so callers can
/// customize a run without mutating global preferences.
//--------------------------------------------------------------------------------------------------
class RiaPreferencesOpm : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes launch defaults and discovers available WSL distributions.
    RiaPreferencesOpm();

    /// @return The OPM preference object owned by the current application preferences.
    static RiaPreferencesOpm* current();

    /// Adds OPM Flow launch fields to an existing preferences UI group.
    void appendItems( caf::PdmUiOrdering& uiOrdering );

    /// @return true when the selected platform and executable/MPI settings form a usable launch configuration.
    bool validateFlowSettings() const;

    /// @return OPM Flow executable or command.
    QString     opmFlowCommand() const;
    /// @return WSL command-line options, including the selected distribution when configured.
    QStringList wslOptions() const;
    /// @return Whether Flow should run inside Windows Subsystem for Linux.
    bool        useWsl() const;
    /// @return Whether Flow should be launched through MPI.
    bool        useMpi() const;
    /// @return MPI launcher executable or command.
    QString     mpirunCommand() const;
    /// @return Maximum number of Flow jobs allowed to run concurrently.
    size_t      maxParallelJobs() const;

    /// @return A newly allocated job-settings object initialized from global defaults.
    RimOpmFlowJobSettings* createDefaultJobSettings() const;

protected:
    /// Supplies discovered WSL distributions as options for the distribution field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;

private:
    caf::PdmField<QString>                     m_opmFlowCommand; ///< Flow executable or launch command.
    caf::PdmField<bool>                        m_useWsl;         ///< Run Flow through WSL on Windows.
    caf::PdmField<QString>                     m_wslDistribution;///< Selected WSL distribution name.
    caf::PdmField<bool>                        m_useMpi;         ///< Launch Flow through MPI.
    caf::PdmField<QString>                     m_mpirunCommand;  ///< MPI launcher executable or command.
    caf::PdmChildField<RimOpmFlowJobSettings*> m_jobSettings;   ///< Persisted defaults copied into new jobs.
    caf::PdmField<size_t>                      m_maxParallelJobs;///< Concurrency limit for Flow jobs.

    QStringList m_availableWslDists; ///< Transient runtime-discovered WSL distribution names.
};
