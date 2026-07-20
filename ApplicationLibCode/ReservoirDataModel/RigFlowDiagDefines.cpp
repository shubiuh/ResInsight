/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2016-     Statoil ASA
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
/// @brief Implements flow diag defines reservoir-data functionality.

#include "RigFlowDiagDefines.h"

namespace RigFlowDiagDefines
{

//--------------------------------------------------------------------------------------------------
/// Returns the tof result name.
//--------------------------------------------------------------------------------------------------
QString tofResultName()
{
    return "TOF";
}

//--------------------------------------------------------------------------------------------------
/// Returns the cell fraction result name.
//--------------------------------------------------------------------------------------------------
QString cellFractionResultName()
{
    return "Fraction";
}

//--------------------------------------------------------------------------------------------------
/// Returns the max fraction tracer result name.
//--------------------------------------------------------------------------------------------------
QString maxFractionTracerResultName()
{
    return "MaxFractionTracer";
}

//--------------------------------------------------------------------------------------------------
/// Returns the communication result name.
//--------------------------------------------------------------------------------------------------
QString communicationResultName()
{
    return "Communication";
}

//--------------------------------------------------------------------------------------------------
/// Returns the num flooded pv.
//--------------------------------------------------------------------------------------------------
QString numFloodedPv()
{
    return "Water Flooded PV";
}

//--------------------------------------------------------------------------------------------------
/// Returns the flow total name.
//--------------------------------------------------------------------------------------------------
QString flowTotalName()
{
    return "Total";
}

//--------------------------------------------------------------------------------------------------
/// Returns the flow oil name.
//--------------------------------------------------------------------------------------------------
QString flowOilName()
{
    return "Oil";
}

//--------------------------------------------------------------------------------------------------
/// Returns the flow gas name.
//--------------------------------------------------------------------------------------------------
QString flowGasName()
{
    return "Gas";
}

//--------------------------------------------------------------------------------------------------
/// Returns the flow water name.
//--------------------------------------------------------------------------------------------------
QString flowWaterName()
{
    return "Water";
}

//--------------------------------------------------------------------------------------------------
/// Returns the reservoir tracer name.
//--------------------------------------------------------------------------------------------------
QString reservoirTracerName()
{
    return "Reservoir";
}

//--------------------------------------------------------------------------------------------------
/// Returns the tiny tracer group name.
//--------------------------------------------------------------------------------------------------
QString tinyTracerGroupName()
{
    return "Other";
}

} // namespace RigFlowDiagDefines
