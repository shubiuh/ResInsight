/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2026 Equinor ASA
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
/// @brief Declares msw segment reservoir-data functionality.
#pragma once

#include "RigMswTableRows.h"

#include <optional>
#include <string>
#include <vector>

// clang-format off


//==================================================================================================
/// Grid-cell intersection for a single MSW segment.
/// Used to populate the COMPSEGS (main grid) and COMPSEGL (LGR sub-grid) tables.
//==================================================================================================
/// @brief Models msw cell intersection for reservoir-data processing.
struct RigMswCellIntersection
{
    size_t      i;              ///< Grid cell I index (1-based).
    size_t      j;              ///< Grid cell J index (1-based).
    size_t      k;              ///< Grid cell K index (1-based).
    /// Stores distance start.
    double      distanceStart;  // Distance from well heel to start of intersection [m or ft]
    /// Stores distance end.
    double      distanceEnd;    // Distance from well heel to end of intersection [m or ft]
    /// Stores grid name.
    std::string gridName;       // Empty for main grid; LGR name for sub-grids (COMPSEGL)
};

//==================================================================================================
/// Primary building block for the MSW export.
/// Each RigMswSegment corresponds to exactly one row in the WELSEGS table.
/// Cell intersections (COMPSEGS/COMPSEGL) and optional valve data are embedded.
/// The branch number lives on the containing RigMswBranch, not here.
//==================================================================================================
/// @brief Models msw segment for reservoir-data processing.
struct RigMswSegment
{
    // WELSEGS fields
    /// Stores segment number.
    int    segmentNumber;        // ISEG1 / ISEG2
    /// Stores outlet segment number.
    int    outletSegmentNumber;  // ISEG3 (parent/outlet segment)

    /// Stores length.
    double                length;    // LENGTH (incremental or absolute MD)
    /// Stores depth.
    double                depth;     // DEPTH  (incremental or absolute TVD)
    /// Stores diameter.
    std::optional<double> diameter;  // ID      (liner inner diameter)
    /// Stores roughness.
    std::optional<double> roughness; // EPSILON (roughness factor)

    /// Stores description.
    std::string description;     // Comment shown in WELSEGS output
    /// Stores source well name.
    std::string sourceWellName;  // Name of the source well path object

    // COMPSEGS / COMPSEGL: grid-cell intersections for this segment
    /// Stores intersections.
    std::vector<RigMswCellIntersection> intersections;

    // Valve data — at most one type per segment
    /// Stores wsegvalv data.
    std::optional<WsegvalvRow> wsegvalvData;  // WSEGVALV: ICV / ICD valves
    /// Stores wsegaicd data.
    std::optional<WsegaicdRow> wsegaicdData;  // WSEGAICD: Autonomous ICD valves
    /// Stores wsegsicd data.
    std::optional<WsegsicdRow> wsegsicdData;  // WSEGSICD: Spiral ICD valves
};


//==================================================================================================
/// One branch in the MSW export.
/// All segments share the same IBRANCH number, which is stored here rather than per-segment.
/// An optional tie-in valve segment (ICV) may appear at the start of lateral branches.
//==================================================================================================
/// @brief Models msw branch for reservoir-data processing.
struct RigMswBranch
{
    /// Stores branch number.
    int                          branchNumber;  // IBRANCH for all segments in this branch
    /// Stores tie in valve.
    std::optional<RigMswSegment> tieInValve;    // Optional ICV at the tie-in point (laterals only)
    /// Stores segments.
    std::vector<RigMswSegment>   segments;      // Segments of this branch
};


//==================================================================================================
/// Complete pre-computed MSW export data for one well.
/// Contains all information needed to write WELSEGS, COMPSEGS, and valve tables without
/// any further tree traversal.
//==================================================================================================
/// @brief Stores msw well export data.
struct RigMswWellExportData
{
    /// Stores header.
    WelsegsHeader               header;    // WELSEGS well-level header
    /// Stores branches.
    std::vector<RigMswBranch>   branches;  // One entry per branch
};

// clang-format on
