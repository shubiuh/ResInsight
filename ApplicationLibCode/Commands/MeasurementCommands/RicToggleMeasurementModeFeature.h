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
/// @brief Declares toggle measurement mode command support.

#pragma once

#include "cafCmdFeature.h"

class RimMeasurement;
class Rim3dView;
class QObject;
class RiuMeasurementViewEventFilter;

//==================================================================================================
/// @brief Command feature for toggle measurement mode.
//==================================================================================================
class RicToggleMeasurementModeFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

    /// Performs the refresh action look command operation.
    void refreshActionLook();

protected:
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
    /// @return Whether the command is checked for the current selection.
    bool isCommandChecked() const override;

protected:
    /// @return The measurement.
    RimMeasurement* measurement() const;
    /// @return The active view.
    Rim3dView*      activeView() const;

private:
    /// Performs the refresh poly measuremeant action look command operation.
    void refreshPolyMeasuremeantActionLook();
};

//==================================================================================================
/// @brief Command feature for toggle poly measurement mode.
//==================================================================================================
class RicTogglePolyMeasurementModeFeature : public RicToggleMeasurementModeFeature
{
    CAF_CMD_HEADER_INIT;

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
    /// @return Whether the command is checked for the current selection.
    bool isCommandChecked() const override;

private:
    /// Performs the refresh measurement action look command operation.
    void refreshMeasurementActionLook();
};
