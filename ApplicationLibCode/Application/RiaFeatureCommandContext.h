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
/// @brief Transient context object passed to feature command execution.

#pragma once

#include <QObject>
#include <QPointer>

//--------------------------------------------------------------------------------------------------
/// @brief Process-wide, non-owning context made available while a command feature executes.
///
/// Some command features need transient information that is not part of their persistent command
/// data. This singleton carries a QObject for the duration of command setup/execution. Callers may
/// either inspect the object directly or read the conventional title/content dynamic properties.
///
/// QPointer automatically becomes null if the external QObject is deleted. The context is global,
/// not thread-local or stack-based; helpers should therefore be used on the UI thread and should
/// not be nested unless the caller accepts that destruction clears rather than restores a previous
/// context.
//--------------------------------------------------------------------------------------------------
class RiaFeatureCommandContext
{
    friend class RiaFeatureCommandContextHelper;

public:
    /// @return The process-wide feature command context.
    static RiaFeatureCommandContext* instance();

    /// The singleton cannot be copied.
    RiaFeatureCommandContext( const RiaFeatureCommandContext& ) = delete;

    /// @return The current non-owning context object, or nullptr if absent or deleted.
    QObject* object() const;

    /// Replaces the current non-owning context object.
    /// @param object Object to expose, or nullptr to clear the context.
    void     setObject( QObject* object );

    /// @return The current object's title property converted to QString, or an empty string.
    QString titleString() const;

    /// @return The current object's content property converted to QString, or an empty string.
    QString contentString() const;

    /// @return The dynamic-property key used by titleString().
    static std::string titleStringIdentifier();

    /// @return The dynamic-property key used by contentString().
    static std::string contentStringIdentifier();

private:
    /// Constructs the process-wide context with no active object.
    RiaFeatureCommandContext();

    /// Destroys the singleton at process termination if explicitly released.
    ~RiaFeatureCommandContext();

private:
    QPointer<QObject> m_pointerToQObject; ///< Non-owning current context, guarded against QObject deletion.
};

//--------------------------------------------------------------------------------------------------
/// @brief RAII helper that sets a QObject context for the duration of a scope.
///
/// Construction installs an external object and destruction clears the global context. The helper
/// does not own the object and does not restore a context that existed before construction.
//--------------------------------------------------------------------------------------------------
class RiaFeatureCommandContextHelper
{
public:
    /// Installs @p externalObject as the current non-owning context.
    explicit RiaFeatureCommandContextHelper( QObject* externalObject );

    /// Clears the process-wide context.
    ~RiaFeatureCommandContextHelper();
};

//--------------------------------------------------------------------------------------------------
/// @brief RAII helper that sets a title/text context for the duration of a scope.
///
/// This variant creates a temporary QObject with the conventional dynamic properties. Destruction
/// schedules that object for deletion through QObject::deleteLater(), allowing queued Qt consumers
/// to finish safely; the context's QPointer clears when deletion occurs.
//--------------------------------------------------------------------------------------------------
class RiaFeatureCommandContextTextHelper
{
public:
    /// Creates and installs a temporary context object containing @p title and @p text.
    RiaFeatureCommandContextTextHelper( const QString& title, const QString& text );

    /// Schedules the temporary context object for deletion.
    ~RiaFeatureCommandContextTextHelper();

private:
    QObject* m_object; ///< Owned temporary object; released asynchronously with deleteLater().
};
