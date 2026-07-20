/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2021     Equinor ASA
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
/// @brief Implements storage and handling of poly lines data.
#include "RigPolyLinesData.h"

//--------------------------------------------------------------------------------------------------
/// Creates a RigPolyLinesData instance.
//--------------------------------------------------------------------------------------------------
RigPolyLinesData::RigPolyLinesData()
    : m_showLines( true )
    , m_showSpheres( true )
    , m_lineThickness( 4 )
    , m_sphereRadiusFactor( 0.1 )
    , m_lockToZPlane( false )
    , m_lockedZValue( 0.0 )
    , m_closePolyline( true )
    , m_skipBoundingBoxCheck( false )
{
    m_sphereColor.set( 200, 200, 200 );
    m_lineColor.set( 200, 200, 200 );
}

//--------------------------------------------------------------------------------------------------
/// Destroys the RigPolyLinesData instance.
//--------------------------------------------------------------------------------------------------
RigPolyLinesData::~RigPolyLinesData()
{
}

//--------------------------------------------------------------------------------------------------
/// Returns the raw poly lines.
//--------------------------------------------------------------------------------------------------
const std::vector<std::vector<cvf::Vec3d>>& RigPolyLinesData::rawPolyLines() const
{
    return m_polylines;
}

//--------------------------------------------------------------------------------------------------
/// Returns the polylines with the last point equal to the first point if the polyline is closed
//--------------------------------------------------------------------------------------------------
const std::vector<std::vector<cvf::Vec3d>> RigPolyLinesData::completePolyLines() const
{
    if ( !m_closePolyline ) return m_polylines;

    std::vector<std::vector<cvf::Vec3d>> completeLines;
    for ( const auto& polyline : m_polylines )
    {
        auto completePolyline = polyline;
        if ( !polyline.empty() )
        {
            const double epsilon = 1e-6;

            if ( polyline.front().pointDistance( polyline.back() ) > epsilon )
            {
                completePolyline.push_back( polyline.front() );
            }
        }
        completeLines.push_back( completePolyline );
    }

    return completeLines;
}

//--------------------------------------------------------------------------------------------------
/// Sets poly lines.
//--------------------------------------------------------------------------------------------------
void RigPolyLinesData::setPolyLines( const std::vector<std::vector<cvf::Vec3d>>& polyLines )
{
    m_polylines = polyLines;
}

//--------------------------------------------------------------------------------------------------
/// Sets poly line.
//--------------------------------------------------------------------------------------------------
void RigPolyLinesData::setPolyLine( const std::vector<cvf::Vec3d>& polyline )
{
    m_polylines = { polyline };
}

//--------------------------------------------------------------------------------------------------
/// Adds poly line.
//--------------------------------------------------------------------------------------------------
void RigPolyLinesData::addPolyLine( const std::vector<cvf::Vec3d>& polyline )
{
    m_polylines.push_back( polyline );
}

//--------------------------------------------------------------------------------------------------
/// Sets visibility.
//--------------------------------------------------------------------------------------------------
void RigPolyLinesData::setVisibility( bool showLines, bool showSpheres )
{
    m_showLines   = showLines;
    m_showSpheres = showSpheres;
}

//--------------------------------------------------------------------------------------------------
/// Sets line appearance.
//--------------------------------------------------------------------------------------------------
void RigPolyLinesData::setLineAppearance( int lineThickness, cvf::Color3f color, bool closePolyline )
{
    m_lineThickness = lineThickness;
    m_lineColor     = color;
    m_closePolyline = closePolyline;
}

//--------------------------------------------------------------------------------------------------
/// Sets sphere appearance.
//--------------------------------------------------------------------------------------------------
void RigPolyLinesData::setSphereAppearance( double radiusFactor, cvf::Color3f color )
{
    m_sphereRadiusFactor = radiusFactor;
    m_sphereColor        = color;
}

//--------------------------------------------------------------------------------------------------
/// Returns the show lines.
//--------------------------------------------------------------------------------------------------
bool RigPolyLinesData::showLines() const
{
    return m_showLines;
}

//--------------------------------------------------------------------------------------------------
/// Returns the show spheres.
//--------------------------------------------------------------------------------------------------
bool RigPolyLinesData::showSpheres() const
{
    return m_showSpheres;
}

//--------------------------------------------------------------------------------------------------
/// Returns the line thickness.
//--------------------------------------------------------------------------------------------------
int RigPolyLinesData::lineThickness() const
{
    return m_lineThickness;
}

//--------------------------------------------------------------------------------------------------
/// Returns the line color.
//--------------------------------------------------------------------------------------------------
cvf::Color3f RigPolyLinesData::lineColor() const
{
    return m_lineColor;
}

//--------------------------------------------------------------------------------------------------
/// Returns the close polyline.
//--------------------------------------------------------------------------------------------------
bool RigPolyLinesData::closePolyline() const
{
    return m_closePolyline;
}

//--------------------------------------------------------------------------------------------------
/// Returns the sphere color.
//--------------------------------------------------------------------------------------------------
cvf::Color3f RigPolyLinesData::sphereColor() const
{
    return m_sphereColor;
}

//--------------------------------------------------------------------------------------------------
/// Returns the sphere radius factor.
//--------------------------------------------------------------------------------------------------
double RigPolyLinesData::sphereRadiusFactor() const
{
    return m_sphereRadiusFactor;
}

//--------------------------------------------------------------------------------------------------
/// Returns the locked z value.
//--------------------------------------------------------------------------------------------------
double RigPolyLinesData::lockedZValue() const
{
    return m_lockedZValue;
}

//--------------------------------------------------------------------------------------------------
/// Returns the lock to z plane.
//--------------------------------------------------------------------------------------------------
bool RigPolyLinesData::lockToZPlane() const
{
    return m_lockToZPlane;
}

//--------------------------------------------------------------------------------------------------
/// Sets z plane lock.
//--------------------------------------------------------------------------------------------------
void RigPolyLinesData::setZPlaneLock( bool lockToZ, double lockZValue )
{
    m_lockToZPlane = lockToZ;
    m_lockedZValue = lockZValue;
}

//--------------------------------------------------------------------------------------------------
/// Sets skip bounding box check.
//--------------------------------------------------------------------------------------------------
void RigPolyLinesData::setSkipBoundingBoxCheck( bool skipCheck )
{
    m_skipBoundingBoxCheck = skipCheck;
}

//--------------------------------------------------------------------------------------------------
/// Returns the skip bounding box check.
//--------------------------------------------------------------------------------------------------
bool RigPolyLinesData::skipBoundingBoxCheck() const
{
    return m_skipBoundingBoxCheck;
}
