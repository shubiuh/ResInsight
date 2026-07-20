/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2025     Equinor ASA
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
/// @brief Declares utilities for dogleg.

#pragma once

#include <vector>

/// @brief Provides utilities for dogleg.
class RigDoglegTools
{
public:
    /// @brief Models well survey point for reservoir-data processing.
    struct WellSurveyPoint
    {
        /// Stores inclination.
        double inclination;
        /// Stores azimuth.
        double azimuth;
        /// Stores measured depth.
        double measuredDepth;
    };

    /// Calculates dogleg angle.
    static double calculateDoglegAngle( const WellSurveyPoint& p1, const WellSurveyPoint& p2 );
    /// Calculates dogleg severity.
    static double calculateDoglegSeverity( const WellSurveyPoint& p1, const WellSurveyPoint& p2, double normalizationDistance = 30.0 );
    /// Calculates trajectory dogleg.
    static std::vector<double> calculateTrajectoryDogleg( const std::vector<WellSurveyPoint>& trajectory, double normalizationDistance = 30.0 );
};
