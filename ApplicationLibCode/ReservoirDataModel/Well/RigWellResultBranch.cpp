/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2023- Equinor ASA
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
/// @brief Implements well result branch reservoir-data functionality.

#include "RigWellResultBranch.h"

//--------------------------------------------------------------------------------------------------
/// Creates a RigWellResultBranch instance.
//--------------------------------------------------------------------------------------------------
RigWellResultBranch::RigWellResultBranch()
    : m_ertBranchId( -1 )
{
}

//--------------------------------------------------------------------------------------------------
/// Returns the ert branch id.
//--------------------------------------------------------------------------------------------------
int RigWellResultBranch::ertBranchId() const
{
    return m_ertBranchId;
}

//--------------------------------------------------------------------------------------------------
/// Sets ert branch id.
//--------------------------------------------------------------------------------------------------
void RigWellResultBranch::setErtBranchId( int id )
{
    m_ertBranchId = id;
}

//--------------------------------------------------------------------------------------------------
/// Returns the branch result points.
//--------------------------------------------------------------------------------------------------
std::vector<RigWellResultPoint> RigWellResultBranch::branchResultPoints() const
{
    return m_branchResultPoints;
}

//--------------------------------------------------------------------------------------------------
/// Adds branch result point.
//--------------------------------------------------------------------------------------------------
void RigWellResultBranch::addBranchResultPoint( const RigWellResultPoint& point )
{
    m_branchResultPoints.push_back( point );
}

//--------------------------------------------------------------------------------------------------
/// Sets branch result points.
//--------------------------------------------------------------------------------------------------
void RigWellResultBranch::setBranchResultPoints( const std::vector<RigWellResultPoint>& points )
{
    m_branchResultPoints = points;
}
