/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Serializable non-owning reference to a PDM field.
//
//  Copyright (C) 2024- Equinor ASA
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

#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPtrField.h"

//==================================================================================================
/// @brief Identifies a PDM field by an object pointer and the field's persistent keyword.
///
/// CAF provides object pointer fields but no directly serializable PDM-field pointer. Storing the
/// object relationship together with the stable field keyword allows the field to be resolved after
/// project loading while remaining non-owning. Resolution can return `nullptr` when the object no
/// longer contains the keyword, which lets Quick Access remove stale entries safely.
//==================================================================================================
class RimFieldReference : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes an unresolved field reference.
    RimFieldReference();

    /// Sets the referenced object and selects its first exposed Quick Access field when available.
    void setObject( caf::PdmObject* object );

    /// Stores the owner object and keyword of @p field.
    void                 setField( caf::PdmFieldHandle* field );
    /// Resolves and returns the current field, or `nullptr` if the relationship is stale.
    caf::PdmFieldHandle* field() const;

    /// @return The referenced object, without transferring ownership.
    caf::PdmObject* object() const;

private:
    /// Defines the compact object-and-field editor used for manual selection.
    void                          defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;
    /// Supplies valid object and field choices for the current relationship.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;

    /// Stores @p object and @p fieldName as one logical relationship.
    void setField( caf::PdmObject* object, const QString& fieldName );

    /// Collects keyword/display-name pairs from fields visible in @p object's UI ordering.
    static std::vector<std::pair<QString, QString>> fieldKeywordAndNames( caf::PdmObject* object );
    /// Traverses a UI-ordering tree and appends its field handles to @p fields.
    static void findFieldsRecursively( caf::PdmUiItem* object, std::vector<caf::PdmFieldHandle*>& fields );

private:
    /// Non-owning PDM pointer to the object containing the field.
    caf::PdmPtrField<caf::PdmObject*> m_object;
    /// Persistent keyword used to resolve the field within m_object.
    caf::PdmField<QString>            m_fieldKeyword;
};
