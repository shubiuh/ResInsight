/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Project-owned optional linked-view configuration.
//
//  Copyright (C) 2015-     Statoil ASA
//  Copyright (C) 2015-     Ceetron Solutions AS
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

#include "cafPdmChildField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"

class RimViewLinker;

//==================================================================================================
/// @brief Owns the single view-linking group and provides its global enable switch.
///
/// Keeping activation outside RimViewLinker allows the complete linking configuration to remain
/// serialized while temporarily disabled. Turning the collection off removes effective overrides;
/// re-enabling reapplies them without rebuilding controllers.
//==================================================================================================
class RimViewLinkerCollection : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes an active collection with an empty owned linker.
    RimViewLinkerCollection();
    ~RimViewLinkerCollection() override;

    /// Global activation state exposed as the tree item's checkbox.
    caf::PdmField<bool>                isActive;
    /// Owned master/dependent linking configuration.
    caf::PdmChildField<RimViewLinker*> viewLinker;

    /// Clears deleted child relationships and updates linked-view presentation.
    void onChildDeleted( caf::PdmChildArrayFieldHandle* childArray, std::vector<caf::PdmObjectHandle*>& referringObjects ) override;

protected:
    /// Uses isActive as the project-tree toggle field.
    caf::PdmFieldHandle* objectToggleField() override { return &isActive; }
    /// Shows the linker and its dependent controllers beneath this collection.
    void                 defineUiTreeOrdering( caf::PdmUiTreeOrdering& uiTreeOrdering, QString uiConfigName = "" ) override;
    /// Applies or removes all synchronization overrides when activation changes.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;
    /// Restores mandatory linker state and presentation after deserialization.
    void initAfterRead() override;
};
