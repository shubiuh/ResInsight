/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2022     Equinor ASA
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
/// @brief Declares show grid calculator command support.

#pragma once

#include "cafCmdFeature.h"

class RicGridCalculatorDialog;
class RimEclipseResultAddress;

//==================================================================================================
/// @brief Command feature for show grid calculator.
//==================================================================================================
class RicShowGridCalculatorFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

public:
    /// Adds calculation and show.
    static void addCalculationAndShowDialog( const RimEclipseResultAddress& address );

private:
    /// @return The grid calculator.
    static RicGridCalculatorDialog* gridCalculatorDialog( bool createIfNotPresent );
    /// Performs the hide grid calculator command operation.
    static void                     hideGridCalculatorDialog();

    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
};
