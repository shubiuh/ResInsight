/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Dialog model for selecting Quick Access fields from one PDM object.
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
#include "cafPdmPointer.h"
#include "cafPdmProxyValueField.h"

/// @brief Maintains a keyword-based selection of fields exposed by a Quick Access provider.
///
/// The source object is held through a guarded, non-owning PDM pointer. Selected fields are stored
/// as keywords instead of raw pointers so the selection remains stable while editors are rebuilt.
/// An empty keyword list intentionally means all eligible fields.
class RimFieldSelection : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes the read-only object-name proxy and tree-selection field.
    RimFieldSelection();

    /// Sets the non-owning object whose Quick Access fields can be selected.
    void setObject( caf::PdmObject* object );
    /// Explicitly records every currently exposed field keyword.
    void selectAllFields();

    /// @return Live fields matching the stored keywords; an empty selection returns every field.
    std::vector<caf::PdmFieldHandle*> fields() const;

private:
    /// @return Display name of the selected object, or an empty string when it has expired.
    QString objectName() const;

    /// Builds selectable field options from RimFieldQuickAccessInterface.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;

private:
    /// Persistent keywords of the selected fields; empty denotes all fields.
    caf::PdmField<std::vector<QString>> m_keywords;
    /// Read-only proxy presenting the current object's UI name.
    caf::PdmProxyValueField<QString>    m_objectName;

    /// Guarded, non-owning source object used to resolve keywords to live fields.
    caf::PdmPointer<caf::PdmObject> m_object;
};
