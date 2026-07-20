/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief View-scoped grouping of fields shown in the Quick Access panel.
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

#include "RimNamedObject.h"

#include "cafPdmChildArrayField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPtrField.h"

class RimGridView;
class RimFieldQuickAccess;

//==================================================================================================
/// @brief Owns a deduplicated set of Quick Access entries belonging to one grid view.
///
/// A group may represent one quick-access category supplied by a model object. The owner-view
/// pointer confines the group to a particular view and prevents fields from another view being
/// mixed into it. Child-array ownership gives the entries the same persistence lifetime as the
/// project-level Quick Access collection.
//==================================================================================================
class RimFieldQuickAccessGroup : public RimNamedObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes an empty group with no owner view.
    RimFieldQuickAccessGroup();

    /// @return The grid view in which the group's fields are meaningful.
    RimGridView* ownerView() const;
    /// Associates the group with @p owner without taking ownership.
    void         setOwnerView( RimGridView* owner );

    /// Adds every eligible field in @p fields, ignoring null, foreign, and duplicate entries.
    void addFields( const std::vector<caf::PdmFieldHandle*>& fields );
    /// Adds @p field when it belongs to the owner view and is not already present.
    void addField( caf::PdmFieldHandle* field );

    /// @return The child Quick Access entries in stored display order.
    std::vector<RimFieldQuickAccess*> fieldQuickAccesses() const;
    /// @return The common owner of the represented fields, or `nullptr` for an empty group.
    caf::PdmObjectHandle*             groupOwner() const;

    /// Detaches @p fieldQuickAccess; the caller remains responsible for deleting it.
    void removeFieldQuickAccess( RimFieldQuickAccess* fieldQuickAccess );

private:
    /// Transfers @p fieldQuickAccess into the owned child array.
    void addFieldQuickAccess( RimFieldQuickAccess* fieldQuickAccess );
    /// @return Whether this group already represents @p field.
    bool findField( const caf::PdmFieldHandle* field ) const;

    /// Verifies that @p field belongs to this group's view ancestry.
    bool isOwnerViewMatching( caf::PdmFieldHandle* field );

private:
    /// Owned field entries rendered inside this UI group.
    caf::PdmChildArrayField<RimFieldQuickAccess*> m_fieldQuickAccess;
    /// Non-owning persistent pointer to the view that scopes the group.
    caf::PdmPtrField<RimGridView*>                m_ownerView;
};
