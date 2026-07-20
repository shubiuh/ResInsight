/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) Statoil ASA
//  Copyright (C) Ceetron Solutions AS
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
/// @brief Declares utilities for well log extraction.

#pragma once

#include <algorithm>
#include <cmath>

//==================================================================================================
///
/// Default tolerance is 0.1 meters to handle inaccuracies across faults
///
//==================================================================================================

/// @brief Provides utilities for well log extraction.
struct RigWellLogExtractionTools
{
    /// Returns whether equal depth.
    static bool isEqualDepth( double d1, double d2, const double tolerance )
    {
        double depthDiff = d1 - d2;

        return ( fabs( depthDiff ) < tolerance ); // Equal depth
    }
};

//==================================================================================================
///  Class used to sort the intersections along the wellpath,
///  Use as key in a map
/// Sorting according to MD first, then Cell Idx, then Leaving before entering cell
//==================================================================================================

/// @brief Models md cell idx enter leave key for reservoir-data processing.
struct RigMDCellIdxEnterLeaveKey
{
    /// Returns or processes md cell idx enter leave key.
    RigMDCellIdxEnterLeaveKey( double md, size_t cellIdx, bool entering, double tolerance )
        : measuredDepth( md )
        , hexIndex( cellIdx )
        , isEnteringCell( entering )
        , tolerance( tolerance )
    {
    }

    /// Stores measured depth.
    double measuredDepth;
    /// Stores hex index.
    size_t hexIndex;
    /// Stores is entering cell.
    bool   isEnteringCell; // As opposed to leaving.
    /// Returns whether leaving cell.
    bool   isLeavingCell() const { return !isEnteringCell; }
    /// Stores tolerance.
    double tolerance;

    /// Implements the operator< operation.
    bool operator<( const RigMDCellIdxEnterLeaveKey& other ) const
    {
        if ( RigWellLogExtractionTools::isEqualDepth( measuredDepth, other.measuredDepth, tolerance ) )
        {
            if ( hexIndex == other.hexIndex )
            {
                if ( isEnteringCell == other.isEnteringCell )
                {
                    // Completely equal: intersections at cell edges or corners or edges of the face triangles
                    return false;
                }

                return !isEnteringCell; // Sort Leaving cell before (less than) entering cell
            }

            return ( hexIndex < other.hexIndex );
        }

        // The depths are not equal

        return ( measuredDepth < other.measuredDepth );
    }
};

//==================================================================================================
///  Class used to sort the intersections along the wellpath,
///  Use as key in a map
/// Sorting according to MD first,then Leaving before entering cell, then Cell Idx,
//==================================================================================================

/// @brief Models md enter leave cell idx key for reservoir-data processing.
struct RigMDEnterLeaveCellIdxKey
{
    /// Returns or processes md enter leave cell idx key.
    RigMDEnterLeaveCellIdxKey( double md, bool entering, size_t cellIdx, double tolerance )
        : measuredDepth( md )
        , isEnteringCell( entering )
        , hexIndex( cellIdx )
        , tolerance( tolerance )
    {
    }

    /// Stores measured depth.
    double measuredDepth;
    /// Stores is entering cell.
    bool   isEnteringCell; // As opposed to leaving.
    /// Returns whether leaving cell.
    bool   isLeavingCell() const { return !isEnteringCell; }
    /// Stores hex index.
    size_t hexIndex;
    /// Stores tolerance.
    double tolerance;

    /// Implements the operator< operation.
    bool operator<( const RigMDEnterLeaveCellIdxKey& other ) const
    {
        if ( RigWellLogExtractionTools::isEqualDepth( measuredDepth, other.measuredDepth, tolerance ) )
        {
            if ( isEnteringCell == other.isEnteringCell )
            {
                if ( hexIndex == other.hexIndex )
                {
                    // Completely equal: intersections at cell edges or corners or edges of the face triangles
                    return false;
                }

                return ( hexIndex < other.hexIndex );
            }

            return isLeavingCell(); // Sort Leaving cell before (less than) entering cell
        }

        // The depths are not equal

        return ( measuredDepth < other.measuredDepth );
    }

    /// Returns whether proper cell enter leave pair.
    static bool isProperCellEnterLeavePair( const RigMDEnterLeaveCellIdxKey& key1, const RigMDEnterLeaveCellIdxKey& key2 )
    {
        return ( key1.hexIndex == key2.hexIndex && key1.isEnteringCell && key2.isLeavingCell() &&
                 !RigWellLogExtractionTools::isEqualDepth( key1.measuredDepth, key2.measuredDepth, key1.tolerance ) );
    }
};
