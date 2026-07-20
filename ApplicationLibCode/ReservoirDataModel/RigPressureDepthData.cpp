/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2023 -     Equinor ASA
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
/// @brief Implements storage and handling of pressure depth data.

#include "RigPressureDepthData.h"

//--------------------------------------------------------------------------------------------------
/// Creates a RigPressureDepthData instance.
//--------------------------------------------------------------------------------------------------
RigPressureDepthData::RigPressureDepthData()
{
}

//--------------------------------------------------------------------------------------------------
/// Destroys the RigPressureDepthData instance.
//--------------------------------------------------------------------------------------------------
RigPressureDepthData::~RigPressureDepthData()
{
}

//--------------------------------------------------------------------------------------------------
/// Sets well name.
//--------------------------------------------------------------------------------------------------
void RigPressureDepthData::setWellName( const QString& wellName )
{
    m_wellName = wellName;
}

//--------------------------------------------------------------------------------------------------
/// Returns the well name.
//--------------------------------------------------------------------------------------------------
QString RigPressureDepthData::wellName() const
{
    return m_wellName;
}

//--------------------------------------------------------------------------------------------------
/// Sets time step.
//--------------------------------------------------------------------------------------------------
void RigPressureDepthData::setTimeStep( const QDateTime& timeStep )
{
    m_timeStep = timeStep;
}

//--------------------------------------------------------------------------------------------------
/// Returns the time step.
//--------------------------------------------------------------------------------------------------
QDateTime RigPressureDepthData::timeStep() const
{
    return m_timeStep;
}

//--------------------------------------------------------------------------------------------------
/// Adds pressure at depth.
//--------------------------------------------------------------------------------------------------
void RigPressureDepthData::addPressureAtDepth( double pressure, double depth )
{
    m_values.push_back( std::make_pair( pressure, depth ) );
}

//--------------------------------------------------------------------------------------------------
/// Returns pressure depth values.
//--------------------------------------------------------------------------------------------------
std::vector<std::pair<double, double>> RigPressureDepthData::getPressureDepthValues() const
{
    return m_values;
}

//--------------------------------------------------------------------------------------------------
/// Returns the tvdmsl.
//--------------------------------------------------------------------------------------------------
std::vector<double> RigPressureDepthData::tvdmsl() const
{
    std::vector<double> vals;
    std::transform( std::begin( m_values ), std::end( m_values ), std::back_inserter( vals ), []( auto const& v ) { return v.second; } );
    return vals;
}

//--------------------------------------------------------------------------------------------------
/// Returns the pressure.
//--------------------------------------------------------------------------------------------------
std::vector<double> RigPressureDepthData::pressure() const
{
    std::vector<double> vals;
    std::transform( std::begin( m_values ), std::end( m_values ), std::back_inserter( vals ), []( auto const& v ) { return v.first; } );
    return vals;
}
