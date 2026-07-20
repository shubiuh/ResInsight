/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2023 Equinor ASA
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
/// @brief Declares create RFT plots command support.

#pragma once

#include "cafCmdFeature.h"

#include <QString>

#include <vector>

class RimRftPlotCollection;
class RimWellRftPlot;

//==================================================================================================
/// @brief Command feature for create RFT plots.
//==================================================================================================
class RicCreateRftPlotsFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

    /// Appends RFT plot for well.
    static void appendRftPlotForWell( const QString& wellName, RimRftPlotCollection* rftPlotColl );

private:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

    /// Appends RFT plot for well.
    void appendRftPlotForWell( const QString& wellName, RimRftPlotCollection* rftPlotColl, RimWellRftPlot* sourcePlot );
};
