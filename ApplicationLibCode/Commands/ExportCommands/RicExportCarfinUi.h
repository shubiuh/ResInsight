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
/// @brief Declares export carfin command support.

#pragma once

#include "cafPdmChildField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPtrField.h"

#include "cvfVector3.h"

class RimEclipseCase;
class RicCellRangeUi;

//==================================================================================================
/// @brief UI model for configuring export carfin.
//==================================================================================================
class RicExportCarfinUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Constructs the command object.
    RicExportCarfinUi();

    /// Sets case.
    void setCase( RimEclipseCase* rimCase );

    /// @return The max well count.
    int                   maxWellCount() const;
    /// @return The LGR cell count.
    cvf::Vec3st           lgrCellCount() const;
    /// @return The cell range.
    const RicCellRangeUi* cellRange() const;
    /// Exports file name.
    QString               exportFileName() const;
    /// @return The case to apply.
    RimEclipseCase*       caseToApply() const;
    /// @return The grid name.
    QString               gridName() const;

private:
    /// Sets case pointers.
    void setCasePointers( RimEclipseCase* rimCase );
    /// Sets default values from case.
    void setDefaultValuesFromCase();

    /// @return The selectable values for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;
    /// Responds to a value changed through the command UI.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;
    /// Defines the field ordering used by the command UI.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;
    /// Performs the define editor attribute command operation.
    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;

private:
    caf::PdmField<QString>              m_exportFileName;
    caf::PdmPtrField<RimEclipseCase*>   m_caseToApply;
    caf::PdmChildField<RicCellRangeUi*> m_cellRange;

    caf::PdmField<int> m_cellCountI;
    caf::PdmField<int> m_cellCountJ;
    caf::PdmField<int> m_cellCountK;

    caf::PdmField<int> m_maxWellCount;
};
