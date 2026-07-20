/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2018-     Equinor ASA
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
/// @brief Declares new well bore stability plot command support.

#pragma once

#include "cafCmdFeature.h"

#include "RigWbsParameter.h"

#include "RimPlotCurveAppearance.h"

#include "cvfColor3.h"

#include <set>

class RimGeoMechCase;
class RimGeoMechView;
class RimWbsParameters;
class RimWellBoreStabilityPlot;
class RimWellPath;

//==================================================================================================
/// @brief Command feature for new well bore stability plot.
//==================================================================================================
class RicNewWellBoreStabilityPlotFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

public:
    /// Creates plot.
    static RimWellBoreStabilityPlot*
        createPlot( RimGeoMechCase* geoMechCase, RimWellPath* wellPath, int timeStep, const RimWbsParameters* parameters = nullptr );

protected:
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

private:
    /// Creates formation track.
    static void createFormationTrack( RimWellBoreStabilityPlot* plot, RimWellPath* wellPath, RimGeoMechCase* geoMechCase );
    /// Creates casing shoe track.
    static void createCasingShoeTrack( RimWellBoreStabilityPlot* plot, RimWellPath* wellPath, RimGeoMechCase* geoMechCase );
    /// Creates parameters track.
    static void createParametersTrack( RimWellBoreStabilityPlot* plot, RimWellPath* wellPath, RimGeoMechCase* geoMechCase, int timeStep );
    /// Creates stability curves track.
    static void createStabilityCurvesTrack( RimWellBoreStabilityPlot* plot, RimWellPath* wellPath, RimGeoMechCase* geoMechCase, int timeStep );
    /// Creates angles track.
    static void createAnglesTrack( RimWellBoreStabilityPlot* plot, RimWellPath* wellPath, RimGeoMechCase* geoMechCase, int timeStep );

    /// @return The parameters for track.
    static std::set<RigWbsParameter> parametersForTrack();
    /// @return The color and line style.
    static std::pair<cvf::Color3f, RiuQwtPlotCurveDefines::LineStyleEnum>
        getColorAndLineStyle( const QString& resultName, size_t i, const std::vector<cvf::Color3f>& colors );
};
