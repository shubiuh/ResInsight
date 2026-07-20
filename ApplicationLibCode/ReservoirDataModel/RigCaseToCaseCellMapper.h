/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2015-     Statoil ASA
//  Copyright (C) 2015-     Ceetron Solutions AS
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
/// @brief Declares case to case cell mapper reservoir-data functionality.

#pragma once

#include "cvfObject.h"
#include "cvfVector3.h"

#include <vector>

class RigMainGrid;
class RigFemPart;

//==================================================================================================
///
//==================================================================================================
/// @brief Maps case to case cell mapper.
class RigCaseToCaseCellMapper : public cvf::Object
{
public:
    RigCaseToCaseCellMapper( RigMainGrid* masterEclGrid, RigFemPart* dependentFemPart );
    RigCaseToCaseCellMapper( RigMainGrid* masterEclGrid, RigMainGrid* dependentEclGrid );
    RigCaseToCaseCellMapper( RigFemPart* masterFemPart, RigMainGrid* dependentEclGrid );
    RigCaseToCaseCellMapper( RigFemPart* masterFemPart, RigFemPart* dependentFemPart );

    const int* masterCaseCellIndices( int dependentCaseReservoirCellIndex, int* masterCaseCellIndexCount ) const;

    /// Returns or processes master grid.
    const RigMainGrid* masterGrid() const { return m_masterGrid; }
    /// Returns or processes dependent grid.
    const RigMainGrid* dependentGrid() const { return m_dependentGrid; }
    /// Returns or processes master fem part.
    const RigFemPart*  masterFemPart() const { return m_masterFemPart; }
    /// Returns or processes dependent fem part.
    const RigFemPart*  dependentFemPart() const { return m_dependentFemPart; }

private:
    void addMapping( int depCaseCellIdx, int masterCaseMatchingCell );
    void calculateEclToGeomCellMapping( RigMainGrid* masterEclGrid, RigFemPart* dependentFemPart, bool eclipseIsMaster );

    std::vector<int>              m_masterCellOrIntervalIndex;
    std::vector<std::vector<int>> m_masterCellIndexSeries;

    RigMainGrid* m_masterGrid;
    RigMainGrid* m_dependentGrid;
    RigFemPart*  m_masterFemPart;
    RigFemPart*  m_dependentFemPart;
};
