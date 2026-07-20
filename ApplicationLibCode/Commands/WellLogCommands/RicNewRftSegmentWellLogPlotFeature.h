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
/// @brief Declares new RFT segment well log plot command support.

#pragma once

#include "RiaRftDefines.h"

#include "cafCmdFeature.h"

class RimWellLogPlot;
class RimSummaryCase;
class RimPlotCurve;
class RimWellLogTrack;
class RimWellLogRftCurve;

//==================================================================================================
/// @brief Command feature for new RFT segment well log plot.
//==================================================================================================
class RicNewRftSegmentWellLogPlotFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

public:
    /// Appends topology track.
    static void appendTopologyTrack( RimWellLogPlot* plot, const QString& wellName, RimSummaryCase* summaryCase );
    /// Appends pressure track.
    static void appendPressureTrack( RimWellLogPlot* plot, const QString& wellName, RimSummaryCase* summaryCase );
    /// Appends connection factor track.
    static void appendConnectionFactorTrack( RimWellLogPlot* plot, const QString& wellName, RimSummaryCase* summaryCase );

    /// @return The selected or first RFT case.
    static RimSummaryCase* getSelectedOrFirstRftCase();

private:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;

    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

    /// Appends track and curve for branch type.
    static RimWellLogRftCurve* appendTrackAndCurveForBranchType( RimWellLogPlot*           plot,
                                                                 const QString&            trackName,
                                                                 const QString&            resultName,
                                                                 const QString&            wellName,
                                                                 RiaDefines::RftBranchType branchType,
                                                                 RimSummaryCase*           summaryCase );

    /// Creates and add curve.
    static RimWellLogRftCurve* createAndAddCurve( RimWellLogTrack*          track,
                                                  const QString&            resultName,
                                                  const QString&            wellName,
                                                  RiaDefines::RftBranchType branchType,
                                                  RimSummaryCase*           summaryCase );

    /// Appends additional data source track.
    static std::vector<RimPlotCurve*>
        appendAdditionalDataSourceTrack( RimWellLogPlot* plot, const QString& wellName, RimSummaryCase* summaryCase );
};
