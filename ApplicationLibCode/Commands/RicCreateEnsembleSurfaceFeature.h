/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2021-     Equinor ASA
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
/// @brief Declares create ensemble surface command support.

#pragma once

#include "cafCmdFeature.h"

#include <list>

class RicCreateEnsembleSurfaceUi;

//==================================================================================================
/// @brief Command feature for create ensemble surface.
//==================================================================================================
class RicCreateEnsembleSurfaceFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

public:
    /// Performs the open dialog and execute command command operation.
    static void openDialogAndExecuteCommand();
    /// Executes command.
    static void executeCommand( const RicCreateEnsembleSurfaceUi& ui, const std::vector<QString>& fileNames );

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
};
