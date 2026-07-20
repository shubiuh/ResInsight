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
/// @brief Declares result modifier reservoir-data functionality.

#pragma once

#include "RigActiveCellInfo.h"
#include "RigGridBase.h"

//==================================================================================================
///
//==================================================================================================
/// @brief Models result modifier for reservoir-data processing.
class RigResultModifier : public cvf::Object
{
public:
    /// Sets cell scalar.
    virtual void setCellScalar( size_t gridLocalCellIndex, double scalarValue ) = 0;
};

//==================================================================================================
///
//==================================================================================================
/// @brief Models all grid cells result modifier for reservoir-data processing.
class RigAllGridCellsResultModifier : public RigResultModifier
{
public:
    /// Returns or processes all grid cells result modifier.
    RigAllGridCellsResultModifier( const RigGridBase* grid, std::vector<double>* reservoirResultValues )
        : m_grid( grid )
        , m_reservoirResultValues( reservoirResultValues )
    {
    }

    /// Sets cell scalar.
    void setCellScalar( size_t gridLocalCellIndex, double scalarValue ) override
    {
        size_t reservoirCellIndex = m_grid->reservoirCellIndex( gridLocalCellIndex );
        CVF_TIGHT_ASSERT( reservoirCellIndex < m_reservoirResultValues->size() );

        ( *m_reservoirResultValues )[reservoirCellIndex] = scalarValue;
    }

private:
    const RigGridBase*   m_grid;
    std::vector<double>* m_reservoirResultValues;
};

//==================================================================================================
///
//==================================================================================================
/// @brief Models active cells result modifier for reservoir-data processing.
class RigActiveCellsResultModifier : public RigResultModifier
{
public:
    /// Returns or processes active cells result modifier.
    RigActiveCellsResultModifier( const RigGridBase* grid, const RigActiveCellInfo* activeCellInfo, std::vector<double>* reservoirResultValues )
        : m_grid( grid )
        , m_activeCellInfo( activeCellInfo )
        , m_reservoirResultValues( reservoirResultValues )
    {
    }

    /// Sets cell scalar.
    void setCellScalar( size_t gridLocalCellIndex, double scalarValue ) override
    {
        size_t reservoirCellIndex = m_grid->reservoirCellIndex( gridLocalCellIndex );
        size_t resultValueIndex   = m_activeCellInfo->cellResultIndex( ReservoirCellIndex( reservoirCellIndex ) ).value();

        CVF_TIGHT_ASSERT( m_reservoirResultValues != nullptr && resultValueIndex < m_reservoirResultValues->size() );

        ( *m_reservoirResultValues )[resultValueIndex] = scalarValue;
    }

private:
    const RigGridBase*       m_grid;
    const RigActiveCellInfo* m_activeCellInfo;
    std::vector<double>*     m_reservoirResultValues;
};
