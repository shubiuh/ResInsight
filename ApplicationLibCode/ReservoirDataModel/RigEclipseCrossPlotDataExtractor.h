/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2019- Equinor ASA
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
/// @brief Declares extraction of eclipse cross plot data extractor.
#pragma once

#include "RigGridCrossPlotCurveGrouping.h"

#include "cvfArray.h"

#include <map>
#include <utility>
#include <vector>

class RigEclipseCaseData;
class RimEclipseResultDefinition;

class QString;

/// @brief Stores a eclipse cross plot result result.
struct RigEclipseCrossPlotResult
{
    /// Stores x values.
    std::vector<double> xValues;
    /// Stores y values.
    std::vector<double> yValues;
    /// Stores group values continuous.
    std::vector<double> groupValuesContinuous;
    /// Stores group values discrete.
    std::vector<int>    groupValuesDiscrete;
};

/// @brief Extracts eclipse cross plot data extractor.
class RigEclipseCrossPlotDataExtractor
{
public:
    static RigEclipseCrossPlotResult extract( RigEclipseCaseData*               eclipseCase,
                                              int                               resultTimeStep,
                                              const RimEclipseResultDefinition& xAddress,
                                              const RimEclipseResultDefinition& yAddress,
                                              RigGridCrossPlotCurveGrouping     groupingType,
                                              const RimEclipseResultDefinition& groupAddress,
                                              std::map<int, cvf::UByteArray>    timeStepCellVisibilityMap );
};
