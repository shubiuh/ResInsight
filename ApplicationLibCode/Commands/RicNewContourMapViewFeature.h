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
/// @brief Declares new contour map view command support.

#pragma once

#include "cafCmdFeature.h"

class RimEclipseCase;
class RimGeoMechCase;
class RimEclipseContourMapView;
class RimGeoMechContourMapView;
class RimEclipseView;
class RimGeoMechView;

//==================================================================================================
/// @brief Command feature for new contour map view.
//==================================================================================================
class RicNewContourMapViewFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

protected:
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

    /// Creates Eclipse contour map from existing contour map.
    static RimEclipseContourMapView* createEclipseContourMapFromExistingContourMap( RimEclipseCase*           eclipseCase,
                                                                                    RimEclipseContourMapView* existingContourMap );
    /// Creates Eclipse contour map from3d view.
    static RimEclipseContourMapView* createEclipseContourMapFrom3dView( RimEclipseCase* eclipseCase, const RimEclipseView* sourceView );
    /// Creates Eclipse contour map.
    static RimEclipseContourMapView* createEclipseContourMap( RimEclipseCase* eclipseCase );

    /// Creates geo mech contour map from existing contour map.
    static RimGeoMechContourMapView* createGeoMechContourMapFromExistingContourMap( RimGeoMechCase*           geoMechCase,
                                                                                    RimGeoMechContourMapView* existingContourMap );
    /// Creates geo mech contour map from3d view.
    static RimGeoMechContourMapView* createGeoMechContourMapFrom3dView( RimGeoMechCase* geoMechCase, const RimGeoMechView* sourceView );
    /// Creates geo mech contour map.
    static RimGeoMechContourMapView* createGeoMechContourMap( RimGeoMechCase* geoMechCase );

    /// Performs the assign default result and legend command operation.
    static void assignDefaultResultAndLegend( RimEclipseContourMapView* contourMap );
};
