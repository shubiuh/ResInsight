/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017-     Statoil ASA
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
/// @brief Declares well log command support.

#pragma once

#include "RiaRftDefines.h"

#include <QString>

#include <vector>

class RimCase;
class RimSimWellInView;
class Rim3dView;
class Rim3dWellLogCurveCollection;
class RimWellLogExtractionCurve;
class RimWellLogChannel;
class RimWellLogLasFileCurve;
class RimWellLogRftCurve;
class RimWellLogTrack;
class RimWellLogWbsCurve;
class RimWellPath;
class RimWellMeasurementCurve;
class RimSummaryCase;
class RimWellLogCurve;
class RimWellLogCalculatedCurve;

//--------------------------------------------------------------------------------------------------
/// @brief Utility functions for well log.
//--------------------------------------------------------------------------------------------------
class RicWellLogTools
{
public:
    /// @return The selected simulation well.
    static RimSimWellInView* selectedSimulationWell( int* branchIndex );
    /// @return Whether RFT data.
    static bool              hasRftData();
    /// @return Whether RFT data for well.
    static bool              hasRftDataForWell( const QString& wellName );
    /// @return Whether well path or sim well selected in view.
    static bool              isWellPathOrSimWellSelectedInView();
    /// Adds well log channels to plot track.
    static void addWellLogChannelsToPlotTrack( RimWellLogTrack* plotTrack, const std::vector<RimWellLogChannel*>& wellLogFileChannels );
    /// @return The selected well path with log.
    static RimWellPath*            selectedWellPathWithLog();
    /// @return The matching well path with log from selection.
    static RimWellPath*            findWellPathWithLogFromSelection();
    /// Adds RFT curve.
    static RimWellLogRftCurve*     addRftCurve( RimWellLogTrack* plotTrack, const RimSimWellInView* simWell, bool showPlotWindow = true );
    /// Adds file curve.
    static RimWellLogLasFileCurve* addFileCurve( RimWellLogTrack* plotTrack, bool showPlotWindow = true );

    /// Adds well log extraction curve.
    static RimWellLogExtractionCurve* addWellLogExtractionCurve( RimWellLogTrack*        plotTrack,
                                                                 RimCase*                rimCase,
                                                                 Rim3dView*              view,
                                                                 RimWellPath*            wellPath,
                                                                 const RimSimWellInView* simWell,
                                                                 int                     branchIndex,
                                                                 bool                    useBranchDetection,
                                                                 bool                    showPlotWindow = true );
    /// Adds well log WBS curve.
    static RimWellLogWbsCurve*        addWellLogWbsCurve( RimWellLogTrack* plotTrack,
                                                          RimCase*         rimCase,
                                                          Rim3dView*       view,
                                                          RimWellPath*     wellPath,
                                                          int              branchIndex,
                                                          bool             useBranchDetection,
                                                          bool             showPlotWindow = true );
    /// Adds well measurement curve.
    static RimWellMeasurementCurve*
        addWellMeasurementCurve( RimWellLogTrack* plotTrack, RimWellPath* wellPath, const QString& measurementName, bool showPlotWindow = true );
    /// Adds well log calculated curve.
    static RimWellLogCalculatedCurve* addWellLogCalculatedCurve( RimWellLogTrack* plotTrack, bool showPlotWindow = true );

    /// Adds summary RFT curve.
    static RimWellLogCurve*    addSummaryRftCurve( RimWellLogTrack* plotTrack, RimSummaryCase* rimCase );
    /// Adds summary RFT segment curve.
    static RimWellLogRftCurve* addSummaryRftSegmentCurve( RimWellLogTrack*          plotTrack,
                                                          const QString&            resultName,
                                                          const QString&            wellName,
                                                          RiaDefines::RftBranchType branchType,
                                                          RimSummaryCase*           rimCase );
    /// @return Whether data.
    static bool hasData( const QString& resultName, const QString& wellName, RiaDefines::RftBranchType branchType, RimSummaryCase* rimCase );

private:
    /// Adds extraction curve.
    template <typename ExtractionCurveType>
    static ExtractionCurveType* addExtractionCurve( RimWellLogTrack*        plotTrack,
                                                    RimCase*                rimCase,
                                                    Rim3dView*              view,
                                                    RimWellPath*            wellPath,
                                                    const RimSimWellInView* simWell,
                                                    int                     branchIndex,
                                                    bool                    useBranchDetection,
                                                    bool                    showPlotWindow );
};
