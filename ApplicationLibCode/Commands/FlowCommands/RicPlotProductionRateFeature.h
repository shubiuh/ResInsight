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
/// @brief Declares plot production rate command support.

#pragma once

#include "cafCmdFeature.h"

#include "RiaPlotDefines.h"
#include "RimFlowDiagSolution.h"

class RimSummaryCase;
class RimSimWellInView;
class RimSummaryCurve;
class RimSummaryPlot;

//==================================================================================================
/// @brief Command feature for plot production rate.
//==================================================================================================
class RicPlotProductionRateFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

protected:
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

private:
    /// @return The grid summary case for well.
    static RimSummaryCase*  gridSummaryCaseForWell( RimSimWellInView* well );
    /// @return Whether injector.
    static bool             isInjector( RimSimWellInView* well );
    /// Adds summary curve.
    static RimSummaryCurve* addSummaryCurve( RimSummaryPlot*         plot,
                                             const RimSimWellInView* well,
                                             RimSummaryCase*         summaryCase,
                                             const QString&          vectorName,
                                             RiaDefines::PlotAxis    plotAxis,
                                             const cvf::Color3f&     color );
};
