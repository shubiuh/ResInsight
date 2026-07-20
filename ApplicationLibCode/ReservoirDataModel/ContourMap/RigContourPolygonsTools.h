/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2024- Equinor ASA
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
/// @brief Declares utilities for contour polygons.

#pragma once

#include "cafContourLines.h"

#include "cvfBoundingBox.h"

class RigContourMapGrid;

//==================================================================================================
///
///
//==================================================================================================
/// @brief Provides utilities for contour polygons.
class RigContourPolygonsTools
{
public:
    /// @brief Models contour polygon for reservoir-data processing.
    struct ContourPolygon
    {
        /// Stores vertices.
        std::vector<cvf::Vec3d> vertices;
        /// Stores value.
        double                  value;
        /// Stores area.
        double                  area;
        /// Stores bbox.
        cvf::BoundingBox        bbox;
    };

    /// Type alias used for contour polygons.
    using ContourPolygons = std::vector<ContourPolygon>;

    static ContourPolygons createContourPolygonsFromLineSegments( caf::ContourLines::ListOfLineSegments& unorderedLineSegments,
                                                                  double                                 contourValue,
                                                                  double                                 areaThreshold );
    static void            smoothContourPolygons( ContourPolygons& contourPolygons, bool favourExpansion, double sampleSpacing );
    static void            clipContourPolygons( ContourPolygons& contourPolygons, const ContourPolygons& clipBy );
    static double          sumPolygonArea( const ContourPolygons& contourPolygons );
    static bool lineOverlapsWithContourPolygons( const cvf::Vec3d& lineCenter, const ContourPolygons& contourPolygons, double tolerance );

    static std::vector<cvf::Vec3d> generatePickPointPolygon( const cvf::Vec2d& pickPoint, const RigContourMapGrid& contourMapGrid );
};
