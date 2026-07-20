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
/// @brief Persistent string-list storage keyed by application settings keys.

#pragma once

#include <QString>
#include <QStringList>

//--------------------------------------------------------------------------------------------------
/// @brief Reads and writes an ordered string list from the application settings store.
///
/// Each RiaStringListSerializer instance is bound to a single settings key and supports adding,
/// removing, and enumerating strings while capping the list to a configurable maximum length.
/// Adding an existing string moves it to the front, making the stored list suitable for recent-file
/// and recent-value histories. QSettings supplies the application/organization namespace.
//--------------------------------------------------------------------------------------------------
class RiaStringListSerializer
{
public:
    /// Binds this serializer to one QSettings entry.
    /// @param key Settings key whose value is stored as a QStringList.
    explicit RiaStringListSerializer( const QString& key );

    /// Moves @p textString to the front and trims older entries to @p maxStringCount.
    /// @param textString Value to add; all previous occurrences are removed first.
    /// @param maxStringCount Maximum retained entries. Values below the current size remove entries from the back.
    void addString( const QString& textString, int maxStringCount );

    /// Removes every occurrence of @p textString from the persistent list.
    void removeString( const QString& textString );

    /// @return The stored strings in most-recent-first order.
    QStringList textStrings();

private:
    QString m_key; ///< QSettings key used for every read and write.
};
