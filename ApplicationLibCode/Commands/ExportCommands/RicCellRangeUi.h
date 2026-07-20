/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017 Statoil ASA
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
/// @brief Declares cell range command support.

#pragma once

#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPtrField.h"
#include "cafVecIjk.h"

class RimCase;
class RigActiveCellInfo;

//==================================================================================================
/// @brief UI model for configuring cell range.
//==================================================================================================
class RicCellRangeUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Constructs the command object.
    RicCellRangeUi();

    /// Sets case.
    void setCase( RimCase* rimCase );

    /// @return The start.
    caf::VecIjk1 start() const;
    /// @return The count.
    cvf::Vec3st  count() const;
    /// @return The grid name.
    QString      gridName() const;

private:
    /// Performs the define editor attribute command operation.
    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;
    /// @return The selectable values for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;
    /// Responds to a value changed through the command UI.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;
    /// Defines the field ordering used by the command UI.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;

    /// Performs the clamp values command operation.
    void               clampValues();
    /// Sets default values.
    void               setDefaultValues();
    /// @return The active cell info.
    RigActiveCellInfo* activeCellInfo() const;
    /// Updates legend text.
    void               updateLegendText();

private:
    caf::PdmPtrField<RimCase*> m_case;

    caf::PdmField<int> m_gridIndex;

    caf::PdmField<int> m_startIndexI; // Eclipse indexing, first index is 1
    caf::PdmField<int> m_startIndexJ; // Eclipse indexing, first index is 1
    caf::PdmField<int> m_startIndexK; // Eclipse indexing, first index is 1
    caf::PdmField<int> m_cellCountI;
    caf::PdmField<int> m_cellCountJ;
    caf::PdmField<int> m_cellCountK;
};
