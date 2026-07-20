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
/// @brief Declares delete sub items command support.

#pragma once

#include "cafCmdFeature.h"

/// @brief Utilities for caf command workflows.
namespace caf
{
class PdmUiItem;
}

//==================================================================================================
/// @brief Command feature for delete sub items.
//==================================================================================================
class RicDeleteSubItemsFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

public:
    /// @return Whether deletable sub items.
    static bool hasDeletableSubItems( caf::PdmUiItem* uiItem );
    /// Removes or clears sub items.
    static void deleteSubItems( bool onlyDeleteUnchecked );
    /// @return Whether command be enabled.
    static bool canCommandBeEnabled();

protected:
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
};
