/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Eclipse-compatible case constructed directly from corner-point arrays.
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

#pragma once

#include "RimEclipseCase.h"

#include "cafPdmObject.h"

#include <expected>

/// @brief Builds an in-memory RigMainGrid from COORD, ZCORN, and optional ACTNUM arrays.
///
/// This case supports programmatic grid creation without an Eclipse file on disk. Factory operations
/// validate dimensions and array lengths through `std::expected`; successful replacement rebuilds
/// active-cell and depth-derived results so ordinary Eclipse views can consume the case.
class RimCornerPointCase : public RimEclipseCase
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes an empty in-memory corner-point case.
    RimCornerPointCase();
    ~RimCornerPointCase() override;

    /// Ensures the already constructed in-memory grid is prepared as an open Eclipse case.
    bool openEclipseGridFile() override;

    /// Imports ASCII properties matching the in-memory grid.
    bool importAsciiInputProperties( const QStringList& fileNames ) override;

    /// @return Empty location because the grid has no required backing file.
    QString locationOnDisc() const override;

    /// Creates a case from Eclipse-format arrays or returns a validation/build error message.
    static std::expected<RimCornerPointCase*, QString> createFromCoordinatesArray( const int                 nx,
                                                                                   const int                 ny,
                                                                                   const int                 nz,
                                                                                   const std::vector<float>& coord,
                                                                                   const std::vector<float>& zcorn,
                                                                                   const std::vector<float>& actnum );

    /// Replaces @p cornerPointCase's grid using caller-validated arrays and returns completion status.
    static std::expected<void, QString> replaceGridFromCoordinatesArray( RimCornerPointCase&       cornerPointCase,
                                                                         const int                 nx,
                                                                         const int                 ny,
                                                                         const int                 nz,
                                                                         const std::vector<float>& coord,
                                                                         const std::vector<float>& zcorn,
                                                                         const std::vector<float>& actnum );

protected:
    /// Defines the minimal property editor appropriate for an in-memory case.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;

    /// Populates @p eclipseCaseData with cells and nodes decoded from Eclipse corner-point arrays.
    static void buildGrid( RigEclipseCaseData&       eclipseCaseData,
                           const int                 nx,
                           const int                 ny,
                           const int                 nz,
                           const std::vector<float>& coord,
                           const std::vector<float>& zcorn,
                           const std::vector<float>& actnum );

    /// Creates the static ACTNUM result from the grid's active-cell information.
    static void createActnumResult( RigEclipseCaseData& eclipseCaseData );

    /// Calculates depth, cell-size, and related generated results after grid construction.
    static void computeDepthRelatedResults( RimCornerPointCase& cornerPointCase );

    /// Decodes the eight corners of @p cellIdx, applying input @p offset and @p scale.
    static std::array<cvf::Vec3d, 8> getCorners( const RigMainGrid&        grid,
                                                 const std::vector<float>& coord,
                                                 const std::vector<float>& zcorn,
                                                 const size_t              cellIdx,
                                                 const cvf::Vec3d&         offset,
                                                 const cvf::Vec3d&         scale );
};
