/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2011-2012 Statoil ASA, Ceetron AS
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
/// @brief Implements well result point reservoir-data functionality.

#include "RigWellResultPoint.h"

//--------------------------------------------------------------------------------------------------
/// Creates a RigWellResultPoint instance.
//--------------------------------------------------------------------------------------------------
RigWellResultPoint::RigWellResultPoint()
    : m_gridIndex( cvf::UNDEFINED_SIZE_T )
    , m_cellIndex( cvf::UNDEFINED_SIZE_T )
    , m_isOpen( false )
    , m_ertBranchId( -1 )
    , m_ertSegmentId( -1 )
    , m_ertOutletBranchId( -1 )
    , m_ertOutletSegmentId( -1 )
    , m_bottomPosition( cvf::Vec3d::UNDEFINED )
    , m_flowRate( 0.0 )
    , m_oilRate( 0.0 )
    , m_gasRate( 0.0 )
    , m_waterRate( 0.0 )
    , m_connectionFactor( 0.0 )
    , m_isConnectedToValve( false )
{
}

//--------------------------------------------------------------------------------------------------
/// Sets grid index.
//--------------------------------------------------------------------------------------------------
void RigWellResultPoint::setGridIndex( size_t gridIndex )
{
    m_gridIndex = gridIndex;
}

//--------------------------------------------------------------------------------------------------
/// Sets grid cell index.
//--------------------------------------------------------------------------------------------------
void RigWellResultPoint::setGridCellIndex( size_t cellIndex )
{
    m_cellIndex = cellIndex;
}

//--------------------------------------------------------------------------------------------------
/// Sets is open.
//--------------------------------------------------------------------------------------------------
void RigWellResultPoint::setIsOpen( bool isOpen )
{
    m_isOpen = isOpen;
}

//--------------------------------------------------------------------------------------------------
/// Sets flow data.
//--------------------------------------------------------------------------------------------------
void RigWellResultPoint::setFlowData( double flowRate, double oilRate, double gasRate, double waterRate )
{
    m_flowRate  = flowRate;
    m_oilRate   = oilRate;
    m_gasRate   = gasRate;
    m_waterRate = waterRate;
}

//--------------------------------------------------------------------------------------------------
/// Sets connection factor.
//--------------------------------------------------------------------------------------------------
void RigWellResultPoint::setConnectionFactor( double connectionFactor )
{
    m_connectionFactor = connectionFactor;
}

//--------------------------------------------------------------------------------------------------
/// Sets segment data.
//--------------------------------------------------------------------------------------------------
void RigWellResultPoint::setSegmentData( int branchId, int segmentId )
{
    m_ertBranchId  = branchId;
    m_ertSegmentId = segmentId;
}

//--------------------------------------------------------------------------------------------------
/// Sets outlet segment data.
//--------------------------------------------------------------------------------------------------
void RigWellResultPoint::setOutletSegmentData( int outletBranchId, int outletSegmentId )
{
    m_ertOutletBranchId  = outletBranchId;
    m_ertOutletSegmentId = outletSegmentId;
}

//--------------------------------------------------------------------------------------------------
/// Sets bottom position.
//--------------------------------------------------------------------------------------------------
void RigWellResultPoint::setBottomPosition( const cvf::Vec3d& bottomPosition )
{
    m_bottomPosition = bottomPosition;
}

//--------------------------------------------------------------------------------------------------
/// Sets is connected to valve.
//--------------------------------------------------------------------------------------------------
void RigWellResultPoint::setIsConnectedToValve( bool enable )
{
    m_isConnectedToValve = true;
}

//--------------------------------------------------------------------------------------------------
/// Returns whether point valid.
//--------------------------------------------------------------------------------------------------
bool RigWellResultPoint::isPointValid() const
{
    return m_bottomPosition != cvf::Vec3d::UNDEFINED;
}

//--------------------------------------------------------------------------------------------------
/// Returns whether cell.
//--------------------------------------------------------------------------------------------------
bool RigWellResultPoint::isCell() const
{
    return m_cellIndex != cvf::UNDEFINED_SIZE_T;
}

//--------------------------------------------------------------------------------------------------
/// Returns whether valid.
//--------------------------------------------------------------------------------------------------
bool RigWellResultPoint::isValid() const
{
    return isCell() || isPointValid();
}

//--------------------------------------------------------------------------------------------------
/// Returns whether open.
//--------------------------------------------------------------------------------------------------
bool RigWellResultPoint::isOpen() const
{
    return m_isOpen;
}

//--------------------------------------------------------------------------------------------------
/// Returns whether equal.
//--------------------------------------------------------------------------------------------------
bool RigWellResultPoint::isEqual( const RigWellResultPoint& other ) const
{
    return ( m_gridIndex == other.m_gridIndex && m_cellIndex == other.m_cellIndex && m_isOpen == other.m_isOpen &&
             m_ertBranchId == other.m_ertBranchId && m_ertSegmentId == other.m_ertSegmentId && m_flowRate == other.m_flowRate &&
             m_oilRate == other.m_oilRate && m_gasRate == other.m_gasRate && m_waterRate == other.m_waterRate );
}

//--------------------------------------------------------------------------------------------------
/// Returns whether connected to valve.
//--------------------------------------------------------------------------------------------------
bool RigWellResultPoint::isConnectedToValve() const
{
    return m_isConnectedToValve;
}

//--------------------------------------------------------------------------------------------------
/// Returns the flow rate.
//--------------------------------------------------------------------------------------------------
double RigWellResultPoint::flowRate() const
{
    if ( isCell() && m_isOpen )
    {
        return m_flowRate;
    }
    else
    {
        return 0.0;
    }
}

//--------------------------------------------------------------------------------------------------
/// Returns the oil rate.
//--------------------------------------------------------------------------------------------------
double RigWellResultPoint::oilRate() const
{
    if ( isCell() && m_isOpen )
    {
        return m_oilRate;
    }
    else
    {
        return 0.0;
    }
}

//--------------------------------------------------------------------------------------------------
/// Returns the gas rate.
//--------------------------------------------------------------------------------------------------
double RigWellResultPoint::gasRate() const
{
    if ( isCell() && m_isOpen )
    {
        return m_gasRate;
    }
    else
    {
        return 0.0;
    }
}

//--------------------------------------------------------------------------------------------------
/// Returns the water rate.
//--------------------------------------------------------------------------------------------------
double RigWellResultPoint::waterRate() const
{
    if ( isCell() && m_isOpen )
    {
        return m_waterRate;
    }
    else
    {
        return 0.0;
    }
}

//--------------------------------------------------------------------------------------------------
/// Returns the connection factor.
//--------------------------------------------------------------------------------------------------
double RigWellResultPoint::connectionFactor() const
{
    return m_connectionFactor;
}

//--------------------------------------------------------------------------------------------------
/// Clears all flow.
//--------------------------------------------------------------------------------------------------
void RigWellResultPoint::clearAllFlow()
{
    m_connectionFactor = 0.0;
    m_flowRate         = 0.0;
    m_oilRate          = 0.0;
    m_gasRate          = 0.0;
    m_waterRate        = 0.0;
}

//--------------------------------------------------------------------------------------------------
/// Returns the grid index.
//--------------------------------------------------------------------------------------------------
size_t RigWellResultPoint::gridIndex() const
{
    return m_gridIndex;
}

//--------------------------------------------------------------------------------------------------
/// Returns the cell index.
//--------------------------------------------------------------------------------------------------
size_t RigWellResultPoint::cellIndex() const
{
    return m_cellIndex;
}

//--------------------------------------------------------------------------------------------------
/// Returns the branch id.
//--------------------------------------------------------------------------------------------------
int RigWellResultPoint::branchId() const
{
    return m_ertBranchId;
}

//--------------------------------------------------------------------------------------------------
/// Returns the segment id.
//--------------------------------------------------------------------------------------------------
int RigWellResultPoint::segmentId() const
{
    return m_ertSegmentId;
}

//--------------------------------------------------------------------------------------------------
/// Returns the outlet branch id.
//--------------------------------------------------------------------------------------------------
int RigWellResultPoint::outletBranchId() const
{
    return m_ertOutletBranchId;
}

//--------------------------------------------------------------------------------------------------
/// Returns the outlet segment id.
//--------------------------------------------------------------------------------------------------
int RigWellResultPoint::outletSegmentId() const
{
    return m_ertOutletSegmentId;
}

//--------------------------------------------------------------------------------------------------
/// Returns the bottom position.
//--------------------------------------------------------------------------------------------------
cvf::Vec3d RigWellResultPoint::bottomPosition() const
{
    return m_bottomPosition;
}

//--------------------------------------------------------------------------------------------------
/// Returns the cell ijk.
//--------------------------------------------------------------------------------------------------
std::optional<caf::VecIjk0> RigWellResultPoint::cellIjk() const
{
    return m_cellIjk;
}

//--------------------------------------------------------------------------------------------------
/// Sets ijk.
//--------------------------------------------------------------------------------------------------
void RigWellResultPoint::setIjk( caf::VecIjk0 cellIJK )
{
    m_cellIjk = cellIJK;
}
