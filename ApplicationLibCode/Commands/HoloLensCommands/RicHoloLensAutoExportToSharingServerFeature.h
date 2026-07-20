/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2019-     Equinor ASA
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
/// @brief Declares holo lens auto export to sharing server command support.

#pragma once

#include "cafCmdFeature.h"

//==================================================================================================
/// @brief Command feature for holo lens auto export to sharing server.
//==================================================================================================
class RicHoloLensAutoExportToSharingServerFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

public:
    /// Constructs the command object.
    RicHoloLensAutoExportToSharingServerFeature();

    /// Sets active.
    void setActive( bool enable );
    /// @return Whether active.
    bool isActive() const;

    /// Performs the trigger update session command operation.
    void triggerUpdateSession();

private:
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
    /// @return Whether the command is checked for the current selection.
    bool isCommandChecked() const override;

    /// @return Whether session valid.
    bool isSessionValid() const;

private:
    bool m_isActive;
};
