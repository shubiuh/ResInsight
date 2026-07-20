/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2023-     Equinor ASA
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
/// @brief Declares create regression analysis curve command support.

#pragma once

#include "cafCmdFeature.h"

#include "RimSummaryRegressionAnalysisCurve.h"

class RimSummaryPlot;
class RimSummaryCurve;
class RimEnsembleCurveSet;

//==================================================================================================
/// @brief Command feature for create regression analysis curve.
//==================================================================================================
class RicCreateRegressionAnalysisCurveFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

protected:
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

    /// Creates regression analysis curve and add to plot.
    static RimSummaryRegressionAnalysisCurve* createRegressionAnalysisCurveAndAddToPlot( RimSummaryCurve* sourceCurve );
    /// Creates regression analysis curve and add to plot.
    static RimSummaryRegressionAnalysisCurve* createRegressionAnalysisCurveAndAddToPlot( RimEnsembleCurveSet* sourceCurveSet );
};
