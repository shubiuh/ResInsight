/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2025-     Equinor ASA
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
/// @brief Well-log track display enumerations for trajectory type, formation source, and level.

#pragma once

#include "cafAppEnum.h"

namespace RiaDefines
{

/// Data source that provides the trajectory of a well-log track.
enum class WellLogTrackTrajectoryType
{
    WELL_PATH,
    SIMULATION_WELL
};

/// Source from which formation intervals are obtained.
enum class WellLogTrackFormationSource
{
    CASE,
    WELL_PICK_FILTER
};

/// Hierarchy level of formation names displayed on a track.
enum class WellLogTrackFormationLevel
{
    GROUP,
    LEVEL0,
    LEVEL1,
    LEVEL2,
    LEVEL3,
    LEVEL4,
    LEVEL5,
    LEVEL6,
    LEVEL7,
    LEVEL8,
    LEVEL9,
    LEVEL10,
    ALL,
    UNKNOWN,
    NONE
};

} // namespace RiaDefines
