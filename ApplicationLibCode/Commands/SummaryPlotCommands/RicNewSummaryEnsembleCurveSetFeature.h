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
/// @brief Declares new summary ensemble curve set command support.

#pragma once

#include "cafCmdFeature.h"

#include <vector>

class RimSummaryPlot;
class RimEnsembleCurveSet;
class RimSummaryEnsemble;
class RifEclipseSummaryAddress;

//==================================================================================================
/// @brief Command feature for new summary ensemble curve set.
//==================================================================================================
class RicNewSummaryEnsembleCurveSetFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

public:
    /// Creates plot for curve sets and update.
    static RimSummaryPlot*                   createPlotForCurveSetsAndUpdate( std::vector<RimSummaryEnsemble*> ensembles );
    /// Adds default curve sets.
    static std::vector<RimEnsembleCurveSet*> addDefaultCurveSets( RimSummaryPlot* plot, RimSummaryEnsemble* ensemble );
    /// Adds curve set.
    static RimEnsembleCurveSet* addCurveSet( RimSummaryPlot* plot, RimSummaryEnsemble* ensemble, const RifEclipseSummaryAddress& address );

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
