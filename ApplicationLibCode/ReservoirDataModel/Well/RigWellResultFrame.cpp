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
/// @brief Implements well result frame reservoir-data functionality.

#include "RigWellResultFrame.h"

//--------------------------------------------------------------------------------------------------
/// Creates a RigWellResultFrame instance.
//--------------------------------------------------------------------------------------------------
RigWellResultFrame::RigWellResultFrame()
    : m_productionType( RiaDefines::WellProductionType::UNDEFINED_PRODUCTION_TYPE )
    , m_isOpen( false )
{
}

//--------------------------------------------------------------------------------------------------
/// Finds result cell well head included.
//--------------------------------------------------------------------------------------------------
RigWellResultPoint RigWellResultFrame::findResultCellWellHeadIncluded( size_t gridIndex, size_t gridCellIndex ) const
{
    const RigWellResultPoint wellResultPoint = findResultCellWellHeadExcluded( gridIndex, gridCellIndex );
    if ( wellResultPoint.isValid() ) return wellResultPoint;

    // If we could not find the cell among the real connections, we try the wellhead.
    // The wellhead does however not have a real connection state, and is rendering using pipe color
    // https://github.com/OPM/ResInsight/issues/4328

    // This behavior was different prior to release 2019.04 and was rendered as a closed connection (gray)
    // https://github.com/OPM/ResInsight/issues/712

    if ( m_wellHead.cellIndex() == gridCellIndex && m_wellHead.gridIndex() == gridIndex )
    {
        return m_wellHead;
    }

    return RigWellResultPoint();
}

//--------------------------------------------------------------------------------------------------
/// Finds result cell well head excluded.
//--------------------------------------------------------------------------------------------------
RigWellResultPoint RigWellResultFrame::findResultCellWellHeadExcluded( size_t gridIndex, size_t gridCellIndex ) const
{
    CVF_ASSERT( gridIndex != cvf::UNDEFINED_SIZE_T && gridCellIndex != cvf::UNDEFINED_SIZE_T );

    for ( const auto& wellResultBranch : m_wellResultBranches )
    {
        for ( const auto& branchResultPoint : wellResultBranch.branchResultPoints() )
        {
            if ( branchResultPoint.cellIndex() == gridCellIndex && branchResultPoint.gridIndex() == gridIndex )
            {
                return branchResultPoint;
            }
        }
    }
    return RigWellResultPoint();
}

//--------------------------------------------------------------------------------------------------
/// Sets well head.
//--------------------------------------------------------------------------------------------------
void RigWellResultFrame::setWellHead( RigWellResultPoint wellHead )
{
    m_wellHead = wellHead;
}

//--------------------------------------------------------------------------------------------------
/// Returns the well head.
//--------------------------------------------------------------------------------------------------
RigWellResultPoint RigWellResultFrame::wellHead() const
{
    return m_wellHead;
}

//--------------------------------------------------------------------------------------------------
/// Returns the well head or start cell.
//--------------------------------------------------------------------------------------------------
RigWellResultPoint RigWellResultFrame::wellHeadOrStartCell() const
{
    if ( m_wellHead.isCell() ) return m_wellHead;

    for ( const RigWellResultBranch& resBranch : m_wellResultBranches )
    {
        for ( const RigWellResultPoint& wrp : resBranch.branchResultPoints() )
        {
            if ( wrp.isCell() ) return wrp;
        }
    }

    return RigWellResultPoint(); // Nothing else matters
}

//--------------------------------------------------------------------------------------------------
/// Returns the all result points.
//--------------------------------------------------------------------------------------------------
std::vector<RigWellResultPoint> RigWellResultFrame::allResultPoints() const
{
    std::vector<RigWellResultPoint> allPoints;
    for ( const auto& resultBranch : m_wellResultBranches )
    {
        for ( const auto& resultPoint : resultBranch.branchResultPoints() )
        {
            allPoints.push_back( resultPoint );
        }
    }
    return allPoints;
}

//--------------------------------------------------------------------------------------------------
/// Sets is open.
//--------------------------------------------------------------------------------------------------
void RigWellResultFrame::setIsOpen( bool isOpen )
{
    m_isOpen = isOpen;
}

//--------------------------------------------------------------------------------------------------
/// Returns whether open.
//--------------------------------------------------------------------------------------------------
bool RigWellResultFrame::isOpen() const
{
    return m_isOpen;
}

//--------------------------------------------------------------------------------------------------
/// Sets production type.
//--------------------------------------------------------------------------------------------------
void RigWellResultFrame::setProductionType( RiaDefines::WellProductionType productionType )
{
    m_productionType = productionType;
}

//--------------------------------------------------------------------------------------------------
/// Returns the production type.
//--------------------------------------------------------------------------------------------------
RiaDefines::WellProductionType RigWellResultFrame::productionType() const
{
    return m_productionType;
}

//--------------------------------------------------------------------------------------------------
/// Sets timestamp.
//--------------------------------------------------------------------------------------------------
void RigWellResultFrame::setTimestamp( const QDateTime& timeStamp )
{
    m_timestamp = timeStamp;
}

//--------------------------------------------------------------------------------------------------
/// Returns the timestamp.
//--------------------------------------------------------------------------------------------------
QDateTime RigWellResultFrame::timestamp() const
{
    return m_timestamp;
}

//--------------------------------------------------------------------------------------------------
/// Returns the well result branches.
//--------------------------------------------------------------------------------------------------
std::vector<RigWellResultBranch> RigWellResultFrame::wellResultBranches() const
{
    return m_wellResultBranches;
}

//--------------------------------------------------------------------------------------------------
/// Clears well result branches.
//--------------------------------------------------------------------------------------------------
void RigWellResultFrame::clearWellResultBranches()
{
    m_wellResultBranches.clear();
}

//--------------------------------------------------------------------------------------------------
/// Adds well result branch.
//--------------------------------------------------------------------------------------------------
void RigWellResultFrame::addWellResultBranch( const RigWellResultBranch& wellResultBranch )
{
    m_wellResultBranches.push_back( wellResultBranch );
}

//--------------------------------------------------------------------------------------------------
/// Sets well result branches.
//--------------------------------------------------------------------------------------------------
void RigWellResultFrame::setWellResultBranches( const std::vector<RigWellResultBranch>& wellResultBranches )
{
    m_wellResultBranches = wellResultBranches;
}

//--------------------------------------------------------------------------------------------------
/// Returns the branch result points from branch index.
//--------------------------------------------------------------------------------------------------
std::vector<RigWellResultPoint> RigWellResultFrame::branchResultPointsFromBranchIndex( size_t index ) const
{
    CVF_ASSERT( index < m_wellResultBranches.size() );
    return m_wellResultBranches[index].branchResultPoints();
}
