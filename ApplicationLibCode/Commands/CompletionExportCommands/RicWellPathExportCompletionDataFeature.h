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

#include "cafCmdFeature.h"

class RimSimWellInView;
class RimWellPath;

//==================================================================================================
/// @brief Command feature for well path export completion data.
//==================================================================================================
class RicWellPathExportCompletionDataFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

public:
    /// Performs the prepare export settings and export completions command operation.
    static void prepareExportSettingsAndExportCompletions( const QString& dialogTitle, const std::vector<RimWellPath*>& wellPaths );

protected:
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

private:
    /// @return The selected well paths.
    static std::vector<RimWellPath*> selectedWellPaths();
};
