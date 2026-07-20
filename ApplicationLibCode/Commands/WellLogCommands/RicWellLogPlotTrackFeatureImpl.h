/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2015-     Statoil ASA
//  Copyright (C) 2015-     Ceetron Solutions AS
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
/// @brief Declares well log plot track command support.

#pragma once

#include <vector>

class RimWellLogPlot;
class RimWellLogTrack;
class RimWellLogCurve;

//==================================================================================================
/// @brief Shared implementation helpers for well log plot track commands.
//==================================================================================================
class RicWellLogPlotTrackFeatureImpl
{
public:
    /// Moves curves to well log plot track.
    static void moveCurvesToWellLogPlotTrack( RimWellLogTrack* dstTrack, const std::vector<RimWellLogCurve*>& curves, int insertAtPosition );
    /// Moves tracks to well log plot.
    static void moveTracksToWellLogPlot( RimWellLogPlot* wellLogPlot, const std::vector<RimWellLogTrack*>& tracks, int insertAtPosition );
};
