/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2018-     Equinor ASA
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
/// @brief Declares holo lens export command support.

#pragma once

#include "RivCellSetEnum.h"

#include "VdeExportPart.h"

#include "cvfCollection.h"

class QString;
class RimGridView;

/// @brief Utilities for cvf command workflows.
namespace cvf
{
class Part;
} // namespace cvf

//==================================================================================================
/// @brief Supports holo lens export command workflows.
//==================================================================================================
class RicHoloLensExportImpl
{
public:
    /// @return The parts for export.
    static std::vector<VdeExportPart>                      partsForExport( const RimGridView& view );
    /// @return The labels for export.
    static std::vector<std::pair<cvf::Vec3f, cvf::String>> labelsForExport( const RimGridView& view );

private:
    /// Appends texture image.
    static void    appendTextureImage( VdeExportPart& exportPart, cvf::Part* part );
    /// @return The grid cell set type text.
    static QString gridCellSetTypeText( RivCellSetEnum cellSetType );

    /// @return Whether grid.
    static bool isGrid( const cvf::Part* part );
    /// @return Whether pipe.
    static bool isPipe( const cvf::Part* part );
    /// @return Whether mesh lines.
    static bool isMeshLines( const cvf::Part* part );
};
