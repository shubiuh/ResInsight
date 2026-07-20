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
/// @brief Declares fly to object command support.

#pragma once

#include "cafCmdFeature.h"

#include "cvfBoundingBox.h"

/// @brief Utilities for caf command workflows.
namespace caf
{
class PdmObject;
}

//==================================================================================================
/// @brief Command feature for fly to object.
//==================================================================================================
class RicFlyToObjectFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

private:
    /// @return The bounding box for selected objects.
    static cvf::BoundingBox boundingBoxForSelectedObjects();
};
