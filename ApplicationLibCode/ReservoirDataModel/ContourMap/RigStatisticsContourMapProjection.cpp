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
/// @brief Implements statistics contour map projection reservoir-data functionality.

#include "RigStatisticsContourMapProjection.h"

#include "RigContourMapCalculator.h"
#include "RigContourMapGrid.h"

#include "cafAssert.h"

#include <cmath>
#include <utility>

//--------------------------------------------------------------------------------------------------
/// Creates a RigStatisticsContourMapProjection instance.
//--------------------------------------------------------------------------------------------------
RigStatisticsContourMapProjection::RigStatisticsContourMapProjection( const RigContourMapGrid* contourMapGrid )
    : RigContourMapProjection( contourMapGrid )
{
}

//--------------------------------------------------------------------------------------------------
/// Destroys the RigStatisticsContourMapProjection instance.
//--------------------------------------------------------------------------------------------------
RigStatisticsContourMapProjection::~RigStatisticsContourMapProjection()
{
}

std::vector<std::vector<std::pair<size_t, double>>>
    RigStatisticsContourMapProjection::generateGridMapping( RigContourMapCalculator::ResultAggregationType resultAggregation,
                                                            const std::vector<double>&                     weights,
                                                            const std::set<int>&                           kLayers,
                                                            const std::vector<std::vector<cvf::Vec3d>>&    limitToPolygons )
{
    // The grid mapping is usually necessary to produce the data. For the statistics projection
    // the data is already available, so we can just ignore it.
    return {};
}

//--------------------------------------------------------------------------------------------------
/// Generates and save results.
//--------------------------------------------------------------------------------------------------
void RigStatisticsContourMapProjection::generateAndSaveResults( const std::vector<double>& result )
{
    m_aggregatedResults = result;
}

//--------------------------------------------------------------------------------------------------
/// Finds intersecting cells.
//--------------------------------------------------------------------------------------------------
std::vector<size_t> RigStatisticsContourMapProjection::findIntersectingCells( const cvf::BoundingBox& bbox ) const
{
    CAF_ASSERT( false );
    return {};
}

//--------------------------------------------------------------------------------------------------
/// Returns the k layer.
//--------------------------------------------------------------------------------------------------
size_t RigStatisticsContourMapProjection::kLayer( size_t globalCellIdx ) const
{
    CAF_ASSERT( false );
    return 0;
}

//--------------------------------------------------------------------------------------------------
/// Returns the k layers.
//--------------------------------------------------------------------------------------------------
size_t RigStatisticsContourMapProjection::kLayers() const
{
    CAF_ASSERT( false );
    return 0;
}

//--------------------------------------------------------------------------------------------------
/// Calculates overlap volume.
//--------------------------------------------------------------------------------------------------
double RigStatisticsContourMapProjection::calculateOverlapVolume( size_t globalCellIdx, const cvf::BoundingBox& bbox ) const
{
    CAF_ASSERT( false );
    return 0.0;
}

//--------------------------------------------------------------------------------------------------
/// Calculates ray length in cell.
//--------------------------------------------------------------------------------------------------
double RigStatisticsContourMapProjection::calculateRayLengthInCell( size_t            globalCellIdx,
                                                                    const cvf::Vec3d& highestPoint,
                                                                    const cvf::Vec3d& lowestPoint ) const
{
    CAF_ASSERT( false );
    return 0.0;
}

//--------------------------------------------------------------------------------------------------
/// Returns parameter weight for cell.
//--------------------------------------------------------------------------------------------------
double RigStatisticsContourMapProjection::getParameterWeightForCell( size_t cellResultIdx, const std::vector<double>& cellWeights ) const
{
    return 1.0;
}

//--------------------------------------------------------------------------------------------------
/// Returns map cell visibility.
//--------------------------------------------------------------------------------------------------
std::vector<bool> RigStatisticsContourMapProjection::getMapCellVisibility( int viewStepIndex,
                                                                           RigContourMapCalculator::ResultAggregationType resultAggregation )

{
    return std::vector<bool>( numberOfCells(), true );
}

//--------------------------------------------------------------------------------------------------
/// Returns the cells at ij.
//--------------------------------------------------------------------------------------------------
std::vector<std::pair<size_t, double>> RigStatisticsContourMapProjection::cellsAtIJ( unsigned int i, unsigned int j ) const
{
    size_t cellIndex = m_contourMapGrid->cellIndexFromIJ( i, j );
    if ( cellIndex < m_aggregatedResults.size() && !std::isinf( m_aggregatedResults[cellIndex] ) &&
         !std::isnan( m_aggregatedResults[cellIndex] ) )
    {
        return { std::make_pair( cellIndex, m_aggregatedResults[cellIndex] ) };
    }

    return std::vector<std::pair<size_t, double>>();
}

//--------------------------------------------------------------------------------------------------
/// Returns whether cell active.
//--------------------------------------------------------------------------------------------------
bool RigStatisticsContourMapProjection::isCellActive( size_t globalCellIdx ) const
{
    // For statistics projections, all cells are considered active
    return true;
}
