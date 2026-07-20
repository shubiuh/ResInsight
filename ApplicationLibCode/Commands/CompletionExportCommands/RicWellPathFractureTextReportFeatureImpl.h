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
/// @brief Declares well path fracture text report command support.

#pragma once

#include "cafCmdFeature.h"

class RimWellPath;
class RimWellPathFracture;
class RimEclipseCase;
class RimFractureTemplate;
class RimEllipseFractureTemplate;
class RimMeshFractureTemplate;
class RifTextDataTableFormatter;
class RicWellPathFractureReportItem;

//==================================================================================================
/// @brief Shared implementation helpers for well path fracture text report commands.
//==================================================================================================
class RicWellPathFractureTextReportFeatureImpl
{
public:
    /// @return The well path fracture report.
    QString wellPathFractureReport( RimEclipseCase*                                   sourceCase,
                                    const std::vector<RimWellPath*>&                  wellPaths,
                                    const std::vector<RicWellPathFractureReportItem>& wellPathFractureReportItems ) const;

    /// @return The well paths with active fractures.
    static std::vector<RimWellPath*> wellPathsWithActiveFractures();

private:
    /// Creates well file location text.
    QString createWellFileLocationText( const std::vector<RimWellPath*>& wellPaths ) const;
    /// Creates stim plan file location text.
    QString createStimPlanFileLocationText( const std::vector<RimMeshFractureTemplate*>& stimPlanTemplates ) const;
    /// Creates stim plan fracture text.
    QString createStimPlanFractureText( const std::vector<RimMeshFractureTemplate*>& stimPlanTemplates ) const;
    /// Creates ellipse fracture text.
    QString createEllipseFractureText( const std::vector<RimEllipseFractureTemplate*>& ellipseTemplates ) const;
    /// Creates fracture text.
    QString createFractureText( const std::vector<RimFractureTemplate*>& fractureTemplates ) const;
    /// Creates fracture instances text.
    QString createFractureInstancesText( const std::vector<RimWellPathFracture*>& fractureTemplates ) const;

    /// Creates fracture completion summary text.
    QString createFractureCompletionSummaryText( const std::vector<RicWellPathFractureReportItem>& wellPathFractureReportItems ) const;
    /// Creates fracture pressure depletion summary text.
    QString createFracturePressureDepletionSummaryText( const std::vector<RicWellPathFractureReportItem>& wellPathFractureReportItems ) const;

    /// Creates connections per well text.
    QString createConnectionsPerWellText( const std::vector<RicWellPathFractureReportItem>& wellPathFractureReportItems ) const;

    /// Configures formatter.
    void configureFormatter( RifTextDataTableFormatter* formatter ) const;
};
