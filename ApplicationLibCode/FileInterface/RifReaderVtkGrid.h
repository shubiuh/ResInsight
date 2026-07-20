/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2026-     Equinor ASA
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

#include <QString>

class RigEclipseCaseData;

//==================================================================================================
//
// Reader for VTK grid files (.vtk legacy format and .vtu XML format)
//
// Supports unstructured grid data with various cell types (hexahedra, tetrahedra,
// wedges, pyramids) and converts them into ResInsight's Eclipse case data format.
//
//==================================================================================================
class RifReaderVtkGrid
{
public:
    /// Open a VTK file and populate the given Eclipse case data.
    /// Returns true on success, false on failure.
    static bool openFile( const QString& fileName, RigEclipseCaseData* eclipseCaseData, QString* errorMessages );
};
