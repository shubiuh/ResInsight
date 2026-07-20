/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2024 Equinor ASA
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
/// @brief Declares active cell local grid reservoir-data functionality.

#pragma once

#include "RigLocalGrid.h"

class RigActiveCellGrid;

/// @brief Represents active cell local grid.
class RigActiveCellLocalGrid : public RigLocalGrid
{
public:
    /// Returns or processes active cell local grid.
    explicit RigActiveCellLocalGrid( RigActiveCellGrid* mainGrid );
    ~RigActiveCellLocalGrid() override;

    RigCell&       cell( size_t gridLocalCellIndex ) override;
    const RigCell& cell( size_t gridLocalCellIndex ) const override;

private:
    RigActiveCellGrid* m_mainActiveGrid;
};
