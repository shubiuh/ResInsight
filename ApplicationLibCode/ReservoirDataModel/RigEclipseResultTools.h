/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2025  Equinor ASA
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
/// @brief Declares utilities for eclipse result.

#pragma once

#include <QString>
#include <vector>

#include "cafVecIjk.h"
#include "cvfArray.h"
#include "cvfStructGrid.h"

class RimEclipseCase;
class RimEclipseView;
class RigGridExportAdapter;

namespace RigEclipseResultTools
{
/// Enumerates the supported border type values.
enum BorderType : int
{
    INVISIBLE_CELL = 0,
    BORDER_CELL    = 1,
    INTERIOR_CELL  = 2
};

/// @brief Models border cell face for reservoir-data processing.
struct BorderCellFace
{
    /// Stores ijk.
    caf::VecIjk0                       ijk; // Cell indices (0-based)
    /// Stores face type.
    cvf::StructGridInterface::FaceType faceType;
    /// Stores boundary condition.
    int                                boundaryCondition; // BCCON grid value
};

void createResultVector( RimEclipseCase& eclipseCase, const QString& resultName, const std::vector<int>& intValues );

// Border detection and boundary condition functions - work with RigGridExportAdapter for refinement support
std::vector<int> generateBorderResult( const RigGridExportAdapter& gridAdapter, const std::vector<int>& visibility );

std::pair<std::vector<int>, int> generateOperNumResult( RimEclipseCase*             eclipseCase,
                                                        const RigGridExportAdapter& gridAdapter,
                                                        const std::vector<int>&     borderResult,
                                                        int                         maxOperNum,
                                                        int                         borderCellValue = -1 );

int findMaxOperNumValue( RimEclipseCase* eclipseCase );

int findMaxBcconValue( RimEclipseCase* eclipseCase );

std::vector<int> generateBcconResult( const RigGridExportAdapter& gridAdapter, const std::vector<int>& borderResult );

std::vector<BorderCellFace> generateBorderCellFaces( const RigGridExportAdapter& gridAdapter,
                                                     const std::vector<int>&     borderResult,
                                                     const std::vector<int>&     bcconResult );

} // namespace RigEclipseResultTools
