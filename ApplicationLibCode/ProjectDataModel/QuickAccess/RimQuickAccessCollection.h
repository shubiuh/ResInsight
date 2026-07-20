/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Project-level collection that builds the active view's Quick Access panel.
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

#include "cafPdmObjectCollection.h"

class RimFieldQuickAccess;
class RimFieldQuickAccessGroup;
class RimFieldReference;

//==================================================================================================
/// @brief Owns and renders all view-scoped Quick Access field groups in a project.
///
/// Eligible fields are discovered through RimFieldQuickAccessInterface, grouped by their owning
/// grid view, and stored in the project. UI ordering filters those groups to the active view and
/// removes stale or user-dismissed entries before exposing live source fields to the editor.
//==================================================================================================
class RimQuickAccessCollection : public caf::PdmObjectCollection<RimFieldQuickAccessGroup>
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes the owned group array.
    RimQuickAccessCollection();

    /// @return The Quick Access collection owned by the current project.
    static RimQuickAccessCollection* instance();

    /// Adds eligible fields from @p object and every PDM child reachable from it.
    void addQuickAccessFieldsRecursively( caf::PdmObjectHandle* object );
    /// Adds eligible fields exposed directly by @p object.
    void addQuickAccessFields( caf::PdmObjectHandle* object );

    /// Adds the field identified by @p fieldReference to its appropriate view and logical group.
    void addQuickAccessField( const RimFieldReference& fieldReference );

private:
    /// Builds groups for the active view after pruning invalid entries.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;

    /// Finds a matching view/object group or creates one owned by the collection.
    RimFieldQuickAccessGroup* findOrCreateGroup( caf::PdmObjectHandle* object, const QString& groupName );

    /// Deletes entries marked by UI callbacks, stale references, and groups left empty afterward.
    void deleteMarkedObjects();

    /// Refreshes a persisted group name from the current provider metadata when possible.
    static void    updateGroupName( RimFieldQuickAccessGroup* group );
    /// @return Fallback label for providers that do not specify a group name.
    static QString defaultGroupName();
};
