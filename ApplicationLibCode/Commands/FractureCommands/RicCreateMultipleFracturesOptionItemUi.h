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
/// @brief Declares create multiple fractures option item command support.

#pragma once

#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPtrField.h"

class RimFractureTemplate;

//==================================================================================================
/// @brief UI model for configuring create multiple fractures option item.
//==================================================================================================
class RicCreateMultipleFracturesOptionItemUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Constructs the command object.
    RicCreateMultipleFracturesOptionItemUi();

    /// Sets values.
    void setValues( int topKOneBased, int baseKOneBased, RimFractureTemplate* fractureTemplate, double minimumSpacing );

    /// @return The top K layer.
    int                  topKLayer() const;
    /// @return The base K layer.
    int                  baseKLayer() const;
    /// @return The fracture template.
    RimFractureTemplate* fractureTemplate() const;
    /// @return The minimum spacing.
    double               minimumSpacing() const;

    /// @return Whether K layer contained.
    bool isKLayerContained( int oneBasedK ) const;

private:
    /// Responds to a value changed through the command UI.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;

    /// @return The selectable values for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;

private:
    caf::PdmField<int>                     m_topKOneBased; // Eclipse uses 1-based indexing
    caf::PdmField<int>                     m_baseKOneBased; // Eclipse uses 1-based indexing
    caf::PdmPtrField<RimFractureTemplate*> m_fractureTemplate;
    caf::PdmField<double>                  m_minSpacing;
};
