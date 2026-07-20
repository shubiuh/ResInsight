/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Resolution and editing of persistent PDM field references.
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

#include "RimFieldReference.h"
#include "RimFieldQuickAccessInterface.h"

CAF_PDM_SOURCE_INIT( RimFieldReference, "RimFieldReference" );

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RimFieldReference::RimFieldReference()
{
    CAF_PDM_InitFieldNoDefault( &m_object, "Object", "Object" );
    CAF_PDM_InitFieldNoDefault( &m_fieldKeyword, "FieldKeyword", "Field Keyword" );
}

//--------------------------------------------------------------------------------------------------
/// Selects the first provider field as a usable default. Only the stable keyword is stored; the
/// pointer itself is resolved from the object whenever field() is called.
//--------------------------------------------------------------------------------------------------
void RimFieldReference::setObject( caf::PdmObject* object )
{
    m_object = object;

    if ( auto quickInterface = dynamic_cast<RimFieldQuickAccessInterface*>( m_object() ) )
    {
        for ( const auto& [groupName, fields] : quickInterface->quickAccessFields() )
        {
            for ( auto field : fields )
            {
                if ( field )
                {
                    m_fieldKeyword = field->keyword();
                    return;
                }
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFieldReference::setField( caf::PdmFieldHandle* field )
{
    if ( !field ) return;

    auto ownerObject = dynamic_cast<caf::PdmObject*>( field->ownerObject() );
    if ( !ownerObject ) return;

    setField( ownerObject, field->keyword() );
}

//--------------------------------------------------------------------------------------------------
/// Traverses UI ordering rather than object->fields() so choices match the fields the user can
/// actually see. Hidden implementation fields must not become pinning candidates accidentally.
//--------------------------------------------------------------------------------------------------
std::vector<std::pair<QString, QString>> RimFieldReference::fieldKeywordAndNames( caf::PdmObject* object )
{
    std::vector<std::pair<QString, QString>> names;

    if ( object )
    {
        // Get the fields for the current uiOrdering. Calling object->fields() will not work as it will return all fields.
        caf::PdmUiOrdering uiOrdering;
        object->uiOrdering( "", uiOrdering );

        std::vector<caf::PdmFieldHandle*> fields;
        for ( auto item : uiOrdering.uiItems() )
        {
            findFieldsRecursively( item, fields );
        }

        for ( auto item : fields )
        {
            if ( auto field = dynamic_cast<caf::PdmFieldHandle*>( item ) )
            {
                auto text = field->keyword();

                if ( auto uiCapability = field->uiCapability() )
                {
                    text = uiCapability->uiName();
                }

                names.push_back( { field->keyword(), text } );
            }
        }
    }

    return names;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFieldReference::findFieldsRecursively( caf::PdmUiItem* object, std::vector<caf::PdmFieldHandle*>& fields )
{
    if ( auto uiFieldHandle = dynamic_cast<caf::PdmUiFieldHandle*>( object ) )
    {
        if ( uiFieldHandle->fieldHandle() ) fields.push_back( uiFieldHandle->fieldHandle() );
    }

    if ( auto group = dynamic_cast<caf::PdmUiGroup*>( object ) )
    {
        for ( auto child : group->uiItems() )
        {
            findFieldsRecursively( child, fields );
        }
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFieldReference::defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering )
{
    uiOrdering.add( &m_object );
    uiOrdering.add( &m_fieldKeyword );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
QList<caf::PdmOptionItemInfo> RimFieldReference::calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions )
{
    QList<caf::PdmOptionItemInfo> options;

    if ( fieldNeedingOptions == &m_fieldKeyword )
    {
        if ( auto quickInterface = dynamic_cast<RimFieldQuickAccessInterface*>( m_object() ) )
        {
            for ( const auto& [groupName, fields] : quickInterface->quickAccessFields() )
            {
                for ( auto field : fields )
                {
                    auto text = field->keyword();

                    if ( auto uiCapability = field->uiCapability() )
                    {
                        text = uiCapability->uiName();
                    }

                    options.push_back( caf::PdmOptionItemInfo( text, field->keyword() ) );
                }
            }
        }
    }
    else if ( fieldNeedingOptions == &m_object )
    {
        if ( m_object )
        {
            QString text = m_object()->uiName();
            options.push_back( caf::PdmOptionItemInfo( text, m_object ) );
        }
    }

    return options;
}

//--------------------------------------------------------------------------------------------------
/// Late resolution makes stale project references harmless: renamed or removed field keywords
/// simply return null and are pruned during the next Quick Access rebuild.
//--------------------------------------------------------------------------------------------------
caf::PdmFieldHandle* RimFieldReference::field() const
{
    if ( !m_object() ) return nullptr;

    return m_object->findField( m_fieldKeyword() );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
caf::PdmObject* RimFieldReference::object() const
{
    return m_object;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimFieldReference::setField( caf::PdmObject* object, const QString& fieldName )
{
    m_object       = object;
    m_fieldKeyword = fieldName;
}
