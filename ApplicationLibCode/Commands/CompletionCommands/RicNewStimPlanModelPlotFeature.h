/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2020-     Equinor ASA
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
/// @brief Declares new stim plan model plot command support.

#pragma once

#include "cafCmdFeature.h"

#include "RiaDefines.h"
#include "RiaStimPlanModelDefines.h"
#include "RimPlot.h"
#include "RimPlotCurveAppearance.h"

#include "cvfColor3.h"

class RimEclipseCase;
class RimStimPlanModelPlot;
class RimStimPlanModelPlotCollection;
class RimStimPlanModel;

//==================================================================================================
/// @brief Command feature for new stim plan model plot.
//==================================================================================================
class RicNewStimPlanModelPlotFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

public:
    /// Creates plot.
    static RimStimPlanModelPlot* createPlot( RimStimPlanModel* stimPlanModel );

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

private:
    /// Creates formation track.
    static void createFormationTrack( RimStimPlanModelPlot* plot, RimStimPlanModel* stimPlanModel, RimEclipseCase* eclipseCase );
    /// Creates facies track.
    static void createFaciesTrack( RimStimPlanModelPlot* plot, RimStimPlanModel* stimPlanModel, RimEclipseCase* eclipseCase );
    /// Creates layers track.
    static void createLayersTrack( RimStimPlanModelPlot* plot, RimStimPlanModel* stimPlanModel, RimEclipseCase* eclipseCase );

    /// Creates parameters track.
    static void createParametersTrack( RimStimPlanModelPlot*                         plot,
                                       RimStimPlanModel*                             stimPlanModel,
                                       RimEclipseCase*                               eclipseCase,
                                       int                                           timeStep,
                                       const QString&                                trackTitle,
                                       const std::vector<RiaDefines::CurveProperty>& propertyTypes,
                                       bool                                          isPlotLogarithmic = false );

    /// @return Whether show by default.
    static bool shouldShowByDefault( const std::vector<RiaDefines::CurveProperty>& propertyTypes, bool useDetailedFluidLoss );

    /// Creates stim plan model plot.
    static RimStimPlanModelPlot* createStimPlanModelPlot( bool showAfterCreation, const QString& plotDescription );

    /// @return The stim plan model plot collection.
    static RimStimPlanModelPlotCollection* stimPlanModelPlotCollection();

    /// @return The default color.
    static cvf::Color3f                          defaultColor( RiaDefines::CurveProperty property, int colorIndex );
    /// @return The default fill color.
    static cvf::Color3f                          defaultFillColor( RiaDefines::CurveProperty property );
    /// @return The default fill style.
    static RimPlotCurveAppearance::FillStyle     defaultFillStyle( RiaDefines::CurveProperty property );
    /// @return The default line style.
    static RiuQwtPlotCurveDefines::LineStyleEnum defaultLineStyle( RiaDefines::CurveProperty property );
    /// @return The default col span.
    static RimPlot::RowOrColSpan                 defaultColSpan( RiaDefines::CurveProperty property );
    /// @return The use min max ticks only.
    static bool                                  useMinMaxTicksOnly( RiaDefines::CurveProperty property );
};
