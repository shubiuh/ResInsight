/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2019-     Equinor ASA
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
/// @brief Implements persistent string-list storage keyed by application settings keys.

#include "RiaStringListSerializer.h"

#include <QSettings>
#include <QStringList>

//--------------------------------------------------------------------------------------------------
/// Stores the settings key by value so later operations are independent of the caller's string.
//--------------------------------------------------------------------------------------------------
RiaStringListSerializer::RiaStringListSerializer( const QString& key )
    : m_key( key )
{
}

//--------------------------------------------------------------------------------------------------
/// Removing before prepending enforces uniqueness while preserving a most-recent-first order.
//--------------------------------------------------------------------------------------------------
void RiaStringListSerializer::addString( const QString& textString, int maxStringCount )
{
    QSettings   settings;
    QStringList stringList = settings.value( m_key ).toStringList();

    stringList.removeAll( textString );
    stringList.prepend( textString );
    while ( stringList.size() > maxStringCount )
        stringList.removeLast();

    settings.setValue( m_key, stringList );
}

//--------------------------------------------------------------------------------------------------
/// removeAll() also cleans up duplicate entries written by older code or manual settings edits.
//--------------------------------------------------------------------------------------------------
void RiaStringListSerializer::removeString( const QString& textString )
{
    QSettings   settings;
    QStringList files = settings.value( m_key ).toStringList();
    files.removeAll( textString );

    settings.setValue( m_key, files );
}

//--------------------------------------------------------------------------------------------------
/// QSettings converts an absent or incompatible value to an empty QStringList.
//--------------------------------------------------------------------------------------------------
QStringList RiaStringListSerializer::textStrings()
{
    QSettings   settings;
    QStringList stringList = settings.value( m_key ).toStringList();

    return stringList;
}
