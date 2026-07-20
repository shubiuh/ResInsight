/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2018-     Equinor ASA
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
/// @brief Declares new valve template command support.

#pragma once

#include "cafCmdFeature.h"

#include <vector>

class RimValveTemplate;
class RimValveTemplateCollection;
class RimWellPathValve;

//==================================================================================================
/// @brief Command feature for new valve template.
//==================================================================================================
class RicNewValveTemplateFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

public:
    /// Selects valve template and update.
    static void selectValveTemplateAndUpdate( RimValveTemplate* valveTemplate );
    /// Creates new valve template for valve and update.
    static void createNewValveTemplateForValveAndUpdate( RimWellPathValve* valve );

protected:
    /// Creates new valve template.
    static RimValveTemplate* createNewValveTemplate();
    /// Executes the command for the current selection.
    void                     onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void                     setupActionLook( QAction* actionToSetup ) override;
};
