/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017     Statoil ASA
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
/// @brief Declares Eclipse well show command support.

#pragma once

#include "cafCmdFeature.h"

//==================================================================================================
/// @brief Command feature for Eclipse well show label.
//==================================================================================================
class RicEclipseWellShowLabelFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// @return Whether the command is checked for the current selection.
    bool isCommandChecked() const override;
};

//==================================================================================================
/// @brief Command feature for Eclipse well show head.
//==================================================================================================
class RicEclipseWellShowHeadFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// @return Whether the command is checked for the current selection.
    bool isCommandChecked() const override;
};

//==================================================================================================
/// @brief Command feature for Eclipse well show pipe.
//==================================================================================================
class RicEclipseWellShowPipeFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// @return Whether the command is checked for the current selection.
    bool isCommandChecked() const override;
};

//==================================================================================================
/// @brief Command feature for Eclipse well show spheres.
//==================================================================================================
class RicEclipseWellShowSpheresFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// @return Whether the command is checked for the current selection.
    bool isCommandChecked() const override;
};

//==================================================================================================
/// @brief Command feature for Eclipse well show well cells.
//==================================================================================================
class RicEclipseWellShowWellCellsFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// @return Whether the command is checked for the current selection.
    bool isCommandChecked() const override;
};

//==================================================================================================
/// @brief Command feature for Eclipse well show well cell fence.
//==================================================================================================
class RicEclipseWellShowWellCellFenceFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// @return Whether the command is checked for the current selection.
    bool isCommandChecked() const override;
};
