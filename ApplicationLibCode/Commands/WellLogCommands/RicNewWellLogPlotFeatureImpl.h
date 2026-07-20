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
/// @brief Declares new well log plot command support.

#pragma once

#include <QString>

class RimWbsParameters;
class RimWellBoreStabilityPlot;
class RimWellLogPlotCollection;
class RimWellLogPlot;
class RimWellLogTrack;
class RimDepthTrackPlot;

//==================================================================================================
/// @brief Shared implementation helpers for new well log plot commands.
//==================================================================================================
class RicNewWellLogPlotFeatureImpl
{
public:
    /// Creates well bore stability plot.
    static RimWellBoreStabilityPlot* createWellBoreStabilityPlot( bool                    showAfterCreation = true,
                                                                  const QString&          plotDescription   = QString( "" ),
                                                                  const RimWbsParameters* params            = nullptr );

    /// Creates RFT segment plot.
    static RimWellLogPlot*  createRftSegmentPlot();
    /// Creates well log track with auto update.
    static RimWellLogTrack* createWellLogTrackWithAutoUpdate();

    /// Creates well log plot.
    static RimWellLogPlot* createWellLogPlot( bool showAfterCreation = true, const QString& plotDescription = QString( "" ) );

    /// Creates well log plot track.
    static RimWellLogTrack* createWellLogPlotTrack( bool               updateAfterCreation = true,
                                                    const QString&     trackDescription    = QString( "" ),
                                                    RimDepthTrackPlot* existingPlot        = nullptr );

    /// Updates after creation.
    static void updateAfterCreation( RimDepthTrackPlot* plot );

private:
    /// @return The well log plot collection.
    static RimWellLogPlotCollection* wellLogPlotCollection();
};
