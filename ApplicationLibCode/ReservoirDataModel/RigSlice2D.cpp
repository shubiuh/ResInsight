/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2021 -     Equinor ASA
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
/// @brief Implements slice2 d reservoir-data functionality.

#include "RigSlice2D.h"

#include "RiaLogging.h"

#include <QString>

//--------------------------------------------------------------------------------------------------
/// Creates a RigSlice2D instance.
//--------------------------------------------------------------------------------------------------
RigSlice2D::RigSlice2D( size_t nx, size_t ny )
    : m_nx( nx )
    , m_ny( ny )
{
    m_values.resize( nx * ny, 0.0 );
}

//--------------------------------------------------------------------------------------------------
/// Returns index.
//--------------------------------------------------------------------------------------------------
size_t RigSlice2D::getIndex( size_t x, size_t y ) const
{
    return y * m_nx + x;
}

//--------------------------------------------------------------------------------------------------
/// Returns the ny.
//--------------------------------------------------------------------------------------------------
size_t RigSlice2D::ny() const
{
    return m_ny;
}

//--------------------------------------------------------------------------------------------------
/// Returns the nx.
//--------------------------------------------------------------------------------------------------
size_t RigSlice2D::nx() const
{
    return m_nx;
}

//--------------------------------------------------------------------------------------------------
/// Sets value.
//--------------------------------------------------------------------------------------------------
void RigSlice2D::setValue( size_t x, size_t y, double value )
{
    m_values[getIndex( x, y )] = value;
}

//--------------------------------------------------------------------------------------------------
/// Returns value.
//--------------------------------------------------------------------------------------------------
double RigSlice2D::getValue( size_t x, size_t y ) const
{
    return m_values[getIndex( x, y )];
}
