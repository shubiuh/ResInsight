/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2021     Equinor ASA
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
/// @brief Geo-mechanical preferences for WIA and FRM external tools.

#pragma once

#include "cafPdmField.h"
#include "cafPdmObject.h"

//--------------------------------------------------------------------------------------------------
/// @brief Preferences for geo-mechanical WIA and FRM external tool integration.
///
/// RiaPreferencesGeoMech controls the default XML configuration files, executable commands,
/// temporary file retention, and user-prompt behaviour for Workflow Integrated Analysis (WIA)
/// and Fracture Risk Management (FRM) external processes. Validation checks the configured command
/// and default XML inputs before a workflow is launched; the class does not run either tool itself.
//--------------------------------------------------------------------------------------------------
class RiaPreferencesGeoMech : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes persisted fields and their editor metadata.
    RiaPreferencesGeoMech();

    /// @return The GeoMech preference object owned by the current application preferences.
    static RiaPreferencesGeoMech* current();

    /// Adds GeoMech fields to an existing preferences UI group.
    void appendItems( caf::PdmUiOrdering& uiOrdering ) const;

    /// @return true when all files required to launch WIA exist.
    bool validateWIASettings() const;

    /// @return true when all files required to launch FRM exist.
    bool validateFRMSettings() const;

    /// @return Default WIA XML input path.
    QString geomechWIADefaultXML() const;

    /// @return Executable or command used to launch WIA.
    QString geomechWIACommand() const;

    /// @return Default FRM XML input path.
    QString geomechFRMDefaultXML() const;

    /// @return Executable or command used to launch FRM.
    QString geomechFRMCommand() const;

    /// @return Whether execution pauses so the generated input can be inspected or edited.
    bool waitBeforeRun() const;

    /// @return Whether temporary workflow input and output files are retained.
    bool keepTemporaryFiles() const;

private:
    /// @return true when every non-empty path in @p filelist exists.
    bool filesExists( QStringList& filelist ) const;

    caf::PdmField<QString> m_geomechWIADefaultXML; ///< Default WIA XML configuration.
    caf::PdmField<QString> m_geomechWIACommand;    ///< WIA launch command.

    caf::PdmField<QString> m_geomechFRMDefaultXML; ///< Default FRM XML configuration.
    caf::PdmField<QString> m_geomechFRMCommand;    ///< FRM launch command.

    caf::PdmField<bool> m_waitForInputFileEdit; ///< Pause before execution for manual input editing.
    caf::PdmField<bool> m_keepTemporaryFiles;   ///< Preserve intermediate workflow files.
};
