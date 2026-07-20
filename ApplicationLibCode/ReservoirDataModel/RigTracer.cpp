/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2020 - Equinor ASA
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
/// @brief Implements tracer reservoir-data functionality.

#include "RigTracer.h"
#include "RigTracerPoint.h"

//--------------------------------------------------------------------------------------------------
/// Creates a RigTracer instance.
//--------------------------------------------------------------------------------------------------
RigTracer::RigTracer()
{
}

//--------------------------------------------------------------------------------------------------
/// Creates a RigTracer instance.
//--------------------------------------------------------------------------------------------------
RigTracer::RigTracer( const RigTracer& other )
    : cvf::Object()
{
    for ( auto p : other.m_points )
    {
        appendPoint( p.position(), p.direction(), p.phaseType() );
    }
}

//--------------------------------------------------------------------------------------------------
/// Destroys the RigTracer instance.
//--------------------------------------------------------------------------------------------------
RigTracer::~RigTracer()
{
}

//--------------------------------------------------------------------------------------------------
/// Appends point.
//--------------------------------------------------------------------------------------------------
void RigTracer::appendPoint( cvf::Vec3d position, cvf::Vec3d direction, RiaDefines::PhaseType dominantPhase )
{
    m_points.push_back( RigTracerPoint( position, direction, dominantPhase ) );
}

//--------------------------------------------------------------------------------------------------
/// Returns the tracer points.
//--------------------------------------------------------------------------------------------------
const std::vector<RigTracerPoint>& RigTracer::tracerPoints() const
{
    return m_points;
}

//--------------------------------------------------------------------------------------------------
/// Returns the size.
//--------------------------------------------------------------------------------------------------
size_t RigTracer::size() const
{
    return m_points.size();
}

//--------------------------------------------------------------------------------------------------
/// Returns the total distance.
//--------------------------------------------------------------------------------------------------
double RigTracer::totalDistance() const
{
    if ( m_points.size() < 2 ) return 0.0;

    cvf::Vec3d sp = m_points.front().position();

    double distance = 0.0;
    for ( size_t i = 1; i < m_points.size(); i++ )
    {
        distance += sp.pointDistance( m_points[i].position() );
        sp = m_points[i].position();
    }

    return distance;
}

//--------------------------------------------------------------------------------------------------
/// Returns the reverse.
//--------------------------------------------------------------------------------------------------
void RigTracer::reverse()
{
    if ( m_points.empty() ) return;

    // Reverse ordering of all tracer points
    std::reverse( m_points.begin(), m_points.end() );

    for ( auto& p : m_points )
    {
        // Reverse the flow direction in tracer point
        p.reverse();
    }
}
