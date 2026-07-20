/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2019- Equinor ASA
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
/// @brief Implements transient context object for feature command execution.

#include "RiaFeatureCommandContext.h"

#include <QVariant>

//--------------------------------------------------------------------------------------------------
/// The QPointer member is default-initialized to an empty context.
//--------------------------------------------------------------------------------------------------
RiaFeatureCommandContext::RiaFeatureCommandContext()
{
}

//--------------------------------------------------------------------------------------------------
/// The context never owns the QObject referenced by its QPointer.
//--------------------------------------------------------------------------------------------------
RiaFeatureCommandContext::~RiaFeatureCommandContext()
{
}

//--------------------------------------------------------------------------------------------------
/// Assignment to QPointer preserves automatic nulling if the QObject is destroyed elsewhere.
//--------------------------------------------------------------------------------------------------
void RiaFeatureCommandContext::setObject( QObject* object )
{
    m_pointerToQObject = object;
}

//--------------------------------------------------------------------------------------------------
/// Exposes the guarded raw pointer for consumers that need custom QObject properties or type checks.
//--------------------------------------------------------------------------------------------------
QObject* RiaFeatureCommandContext::object() const
{
    return m_pointerToQObject;
}

//--------------------------------------------------------------------------------------------------
/// QVariant conversion makes missing or non-string title properties safely produce an empty string.
//--------------------------------------------------------------------------------------------------
QString RiaFeatureCommandContext::titleString() const
{
    if ( m_pointerToQObject )
    {
        QVariant variant = m_pointerToQObject->property( titleStringIdentifier().data() );

        return variant.toString();
    }

    return QString();
}

//--------------------------------------------------------------------------------------------------
/// Content uses the same dynamic-property convention as titleString().
//--------------------------------------------------------------------------------------------------
QString RiaFeatureCommandContext::contentString() const
{
    if ( m_pointerToQObject )
    {
        QVariant variant = m_pointerToQObject->property( contentStringIdentifier().data() );

        return variant.toString();
    }

    return QString();
}

//--------------------------------------------------------------------------------------------------
/// A stable ASCII key allows unrelated command producers and consumers to exchange title text.
//--------------------------------------------------------------------------------------------------
std::string RiaFeatureCommandContext::titleStringIdentifier()
{
    return "titleStringIdentifier";
}

//--------------------------------------------------------------------------------------------------
/// A separate stable key prevents title and body content from being conflated.
//--------------------------------------------------------------------------------------------------
std::string RiaFeatureCommandContext::contentStringIdentifier()
{
    return "contentStringIdentifier";
}

//--------------------------------------------------------------------------------------------------
/// Heap allocation intentionally keeps the process-wide context alive through static destruction order.
//--------------------------------------------------------------------------------------------------
RiaFeatureCommandContext* RiaFeatureCommandContext::instance()
{
    static RiaFeatureCommandContext* commandFileExecutorInstance = new RiaFeatureCommandContext();
    return commandFileExecutorInstance;
}

//--------------------------------------------------------------------------------------------------
/// Installs the caller-owned object without extending its lifetime.
//--------------------------------------------------------------------------------------------------
RiaFeatureCommandContextHelper::RiaFeatureCommandContextHelper( QObject* externalObject )
{
    RiaFeatureCommandContext::instance()->setObject( externalObject );
}

//--------------------------------------------------------------------------------------------------
/// Clearing on scope exit prevents later commands from observing stale transient state.
//--------------------------------------------------------------------------------------------------
RiaFeatureCommandContextHelper::~RiaFeatureCommandContextHelper()
{
    RiaFeatureCommandContext::instance()->setObject( nullptr );
}

//--------------------------------------------------------------------------------------------------
/// Dynamic QObject properties avoid introducing a dedicated context subclass for two text values.
//--------------------------------------------------------------------------------------------------
RiaFeatureCommandContextTextHelper::RiaFeatureCommandContextTextHelper( const QString& title, const QString& text )
{
    m_object = new QObject;

    m_object->setProperty( RiaFeatureCommandContext::titleStringIdentifier().data(), title );
    m_object->setProperty( RiaFeatureCommandContext::contentStringIdentifier().data(), text );

    RiaFeatureCommandContext::instance()->setObject( m_object );
}

//--------------------------------------------------------------------------------------------------
/// deleteLater() avoids destroying the QObject while queued Qt code may still be using it.
//--------------------------------------------------------------------------------------------------
RiaFeatureCommandContextTextHelper::~RiaFeatureCommandContextTextHelper()
{
    if ( m_object )
    {
        m_object->deleteLater();
        m_object = nullptr;
    }
}
