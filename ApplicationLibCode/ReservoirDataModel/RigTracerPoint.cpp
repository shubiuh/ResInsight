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
/// @brief Implements tracer point reservoir-data functionality.

#include "RigTracerPoint.h"

//--------------------------------------------------------------------------------------------------
/// Creates a RigTracerPoint instance.
//--------------------------------------------------------------------------------------------------
RigTracerPoint::RigTracerPoint( cvf::Vec3d position, cvf::Vec3d direction, RiaDefines::PhaseType phase )
    : m_position( position )
    , m_direction( direction )
    , m_phaseType( phase )
{
    m_absValue = m_direction.length();
}

//--------------------------------------------------------------------------------------------------
/// Destroys the RigTracerPoint instance.
//--------------------------------------------------------------------------------------------------
RigTracerPoint::~RigTracerPoint()
{
}

//--------------------------------------------------------------------------------------------------
/// Returns the position.
//--------------------------------------------------------------------------------------------------
const cvf::Vec3d& RigTracerPoint::position() const
{
    return m_position;
}

//--------------------------------------------------------------------------------------------------
/// Returns the direction.
//--------------------------------------------------------------------------------------------------
const cvf::Vec3d& RigTracerPoint::direction() const
{
    return m_direction;
}

//--------------------------------------------------------------------------------------------------
/// Returns the abs value.
//--------------------------------------------------------------------------------------------------
double RigTracerPoint::absValue() const
{
    return m_absValue;
}

//--------------------------------------------------------------------------------------------------
/// Returns the phase type.
//--------------------------------------------------------------------------------------------------
RiaDefines::PhaseType RigTracerPoint::phaseType() const
{
    return m_phaseType;
}

//--------------------------------------------------------------------------------------------------
/// Returns the reverse.
//--------------------------------------------------------------------------------------------------
void RigTracerPoint::reverse()
{
    m_direction *= -1.0;
}
