/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2016-     Statoil ASA
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
/// @brief Declares edit summary plot command support.

#pragma once

#include "cafCmdFeature.h"

class RimSummaryPlot;
class RicSummaryPlotEditorDialog;

//==================================================================================================
/// @brief Command feature for edit summary plot.
//==================================================================================================
class RicEditSummaryPlotFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;
    /// Constructs the command object.
    RicEditSummaryPlotFeature();

public:
    /// Removes or clears dialog and reset target plot.
    void closeDialogAndResetTargetPlot();

    /// @return The curve creator.
    static RicSummaryPlotEditorDialog* curveCreatorDialog( bool createIfNotPresent );
    /// Performs the edit summary plot command operation.
    static void                        editSummaryPlot( RimSummaryPlot* plot );

protected:
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

private:
    /// @return The selected summary plot.
    RimSummaryPlot* selectedSummaryPlot() const;
};
