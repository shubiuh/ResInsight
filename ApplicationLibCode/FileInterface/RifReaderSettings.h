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
/// Defines the common import policy passed to reservoir file readers.

#pragma once

#include <QString>

//==================================================================================================
/// Value object controlling optional and potentially expensive reader behavior.
///
/// The settings are deliberately independent of a concrete file format so the
/// application can apply one import policy consistently to ECLIPSE, OPM, and
/// other reservoir readers.
//==================================================================================================
struct RifReaderSettings
{
    bool importFaults = false; ///< Import FAULTS definitions referenced by the case deck.
    bool importNNCs   = false; ///< Import non-neighbor connections and their transmissibilities.
    bool includeInactiveCellsInFaultGeometry = false; ///< Include faces belonging only to inactive cells in fault geometry.
    bool importAdvancedMswData = false; ///< Read complete multisegment-well topology instead of basic well data.
    bool skipWellData          = true;  ///< Avoid loading simulation well data when it is not needed.
    bool importSummaryData     = false; ///< Import summary data while opening formats that can provide it.
    QString includeFileAbsolutePathPrefix = ""; ///< Prefix used to resolve absolute-looking deck INCLUDE paths on another machine.
    bool onlyLoadActiveCells       = false; ///< Limit cell-property allocation and loading to active cells.
    bool invalidateLongThinCells   = true;  ///< Mark numerically degenerate long, thin cells invalid during import.
    bool useCylindricalCoordinates = false; ///< Interpret supported grids using cylindrical coordinates.
    int minimumAngularCellCount    = 20;    ///< Minimum circumferential resolution used for cylindrical grids.
};
