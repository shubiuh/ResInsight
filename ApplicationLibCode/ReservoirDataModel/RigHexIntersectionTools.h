/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017     Statoil ASA
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
/// @brief Declares utilities for hex intersection.
#pragma once

#include "cvfPlane.h"
#include "cvfStructGrid.h"
#include "cvfVector3.h"

#include <array>
#include <list>

//==================================================================================================
///  Internal class for intersection point info
//==================================================================================================

/// @brief Models hex intersection info for reservoir-data processing.
struct HexIntersectionInfo
{
public:
    /// Returns or processes hex intersection info.
    HexIntersectionInfo( cvf::Vec3d intersectionPoint, bool isIntersectionEntering, cvf::StructGridInterface::FaceType face, size_t hexIndex )
        : m_intersectionPoint( intersectionPoint )
        , m_isIntersectionEntering( isIntersectionEntering )
        , m_face( face )
        , m_hexIndex( hexIndex )
    {
    }

    /// Stores intersection point.
    cvf::Vec3d                         m_intersectionPoint;
    /// Stores is intersection entering.
    bool                               m_isIntersectionEntering;
    /// Stores face.
    cvf::StructGridInterface::FaceType m_face;
    /// Stores hex index.
    size_t                             m_hexIndex;
};

bool operator<( const HexIntersectionInfo& hi1, const HexIntersectionInfo& hi2 );

//--------------------------------------------------------------------------------------------------
/// Specialized Line - Hex intersection
//--------------------------------------------------------------------------------------------------
/// @brief Provides utilities for hex intersection.
struct RigHexIntersectionTools
{
    static int lineHexCellIntersection( const cvf::Vec3d&                 p1,
                                        const cvf::Vec3d&                 p2,
                                        const std::array<cvf::Vec3d, 8>&  hexCorners,
                                        const size_t                      hexIndex,
                                        std::vector<HexIntersectionInfo>* intersections );

    static bool lineIntersectsHexCell( const cvf::Vec3d& p1, const cvf::Vec3d& p2, const std::array<cvf::Vec3d, 8>& hexCorners );

    static bool isPointInCell( const cvf::Vec3d& point, const std::array<cvf::Vec3d, 8>& hexCorners );

    static bool planeHexCellIntersection( const std::array<cvf::Vec3d, 8>&              hexCorners,
                                          const cvf::Plane&                             fracturePlane,
                                          std::list<std::pair<cvf::Vec3d, cvf::Vec3d>>& intersectionLineSegments );

    static bool planeHexIntersectionPolygons( const std::array<cvf::Vec3d, 8>&      hexCorners,
                                              cvf::Mat4d                            transformMatrixForPlane,
                                              std::vector<std::vector<cvf::Vec3d>>& polygons );
};
