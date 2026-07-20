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
/// @brief Declares well log plot curve command support.

#pragma once

#include "cafPdmFieldCvfColor.h"

class RimWellBoreStabilityPlot;
class RimWellLogCurve;
class RimWellAllocationPlot;
class RimWellRftPlot;

//==================================================================================================
/// @brief Shared implementation helpers for well log plot curve commands.
//==================================================================================================
class RicWellLogPlotCurveFeatureImpl
{
public:
    /// @return The curve color from table.
    static cvf::Color3f                  curveColorFromTable( size_t index );
    /// @return The selected well log curves.
    static std::vector<RimWellLogCurve*> selectedWellLogCurves();
    /// @return The parent well allocation plot.
    static RimWellAllocationPlot*        parentWellAllocationPlot();
    /// @return The parent well RFT plot.
    static RimWellRftPlot*               parentWellRftPlot();
    /// @return The parent well bore stability plot.
    static RimWellBoreStabilityPlot*     parentWellBoreStabilityPlot();
};
