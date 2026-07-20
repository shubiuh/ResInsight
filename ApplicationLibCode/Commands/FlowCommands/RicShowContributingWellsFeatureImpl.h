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
/// @brief Declares show contributing wells command support.

#pragma once

#include <vector>

#include <QString>

class RigSimWellData;
class RimEclipseResultCase;
class RimEclipseView;
class RimFlowDiagSolution;
class RimSimWellInView;

//==================================================================================================
/// @brief Shared implementation helpers for show contributing wells commands.
//==================================================================================================
class RicShowContributingWellsFeatureImpl
{
public:
    /// @return The manipulate selected view.
    static RimEclipseView* manipulateSelectedView( RimEclipseResultCase* wellAllocationResultCase, QString wellName, int timeStep );

private:
    /// Performs the modify view to show contributing wells command operation.
    static void modifyViewToShowContributingWells( RimEclipseView* viewToModify, const QString& wellName, int timeStep );

    /// @return The matching contributing tracer names.
    static std::vector<QString>
        findContributingTracerNames( const RimFlowDiagSolution* flowDiagSolution, const RigSimWellData* wellResults, int timeStep );
};
