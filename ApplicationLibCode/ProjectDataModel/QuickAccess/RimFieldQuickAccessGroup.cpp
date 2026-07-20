/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Implementation of view-scoped Quick Access grouping and deduplication.
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

#include "RimFieldQuickAccessGroup.h"
#include "RimFieldQuickAccess.h"
#include "RimGridView.h"

#include "RiaLogging.h"

CAF_PDM_SOURCE_INIT( RimFieldQuickAccessGroup, "RimFieldQuickAccessGroup" );

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RimFieldQuickAccessGroup::RimFieldQuickAccessGroup()
{
    CAF_PDM_InitObject( "Quick Access Group" );

    CAF_PDM_InitFieldNoDefault( &m_fieldQuickAccess, "FieldReferences", "Field References" );
    CAF_PDM_InitFieldNoDefault( &m_ownerView, "OwnerView", "Owner View" );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RimGridView* RimFieldQuickAccessGroup::ownerView() const
{
    return m_ownerView;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFieldQuickAccessGroup::setOwnerView( RimGridView* viewObject )
{
    m_ownerView = viewObject;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFieldQuickAccessGroup::addFields( const std::vector<caf::PdmFieldHandle*>& fields )
{
    if ( !m_ownerView ) return;

    for ( auto field : fields )
    {
        addField( field );
    }
}

//--------------------------------------------------------------------------------------------------
/// Rejects fields from other views before allocating the entry. This keeps persisted groups valid
/// when two open views contain objects of the same type and expose identically named fields.
//--------------------------------------------------------------------------------------------------
void RimFieldQuickAccessGroup::addField( caf::PdmFieldHandle* field )
{
    if ( !field ) return;
    if ( !m_ownerView ) return;
    if ( !isOwnerViewMatching( field ) ) return;
    if ( findField( field ) ) return;

    auto fieldReference = new RimFieldQuickAccess();
    fieldReference->setField( field );

    addFieldQuickAccess( fieldReference );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
std::vector<RimFieldQuickAccess*> RimFieldQuickAccessGroup::fieldQuickAccesses() const
{
    return m_fieldQuickAccess.childrenByType();
}

//--------------------------------------------------------------------------------------------------
/// Determines the logical provider from the first valid entry. Group construction ensures entries
/// normally share an owner; returning null for an empty group also makes it eligible for cleanup.
//--------------------------------------------------------------------------------------------------
caf::PdmObjectHandle* RimFieldQuickAccessGroup::groupOwner() const
{
    for ( auto f : m_fieldQuickAccess )
    {
        if ( f && f->field() )
        {
            return f->field()->ownerObject();
        }
    }

    return nullptr;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFieldQuickAccessGroup::removeFieldQuickAccess( RimFieldQuickAccess* fieldQuickAccess )
{
    m_fieldQuickAccess.removeChild( fieldQuickAccess );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFieldQuickAccessGroup::addFieldQuickAccess( RimFieldQuickAccess* fieldQuickAccess )
{
    if ( !fieldQuickAccess ) return;

    m_fieldQuickAccess.push_back( fieldQuickAccess );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RimFieldQuickAccessGroup::findField( const caf::PdmFieldHandle* field ) const
{
    for ( auto fieldRef : m_fieldQuickAccess )
    {
        if ( field == fieldRef->field() )
        {
            return true;
        }
    }

    return false;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RimFieldQuickAccessGroup::isOwnerViewMatching( caf::PdmFieldHandle* field )
{
    if ( !field || !field->ownerObject() ) return false;
    auto parentView = field->ownerObject()->firstAncestorOrThisOfType<RimGridView>();

    if ( parentView != m_ownerView )
    {
        RiaLogging::debug( "Field does not belong to the owner view" );
        return false;
    }

    return true;
}
