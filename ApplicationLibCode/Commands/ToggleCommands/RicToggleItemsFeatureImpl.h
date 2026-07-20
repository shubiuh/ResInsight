/////////////////////////////////////////////////////////////////////////////////
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
/// @file
/// @brief Declares toggle items command support.

#pragma once

#include "cafPdmField.h"

#include <vector>

/// @brief Utilities for caf command workflows.
namespace caf
{
class PdmUiItem;
class PdmUiTreeOrdering;
class PdmUiTreeView;
class PdmChildArrayFieldHandle;
}; // namespace caf

//==================================================================================================
/// @brief Shared implementation helpers for toggle items commands.
//==================================================================================================
class RicToggleItemsFeatureImpl
{
public:
    enum SelectionToggleType
    {
        TOGGLE_ON,
        TOGGLE_OFF,
        TOGGLE_SUBITEMS,
        TOGGLE,
        TOGGLE_UNDEFINED
    };

    /// @return Whether toggle commands available.
    static bool isToggleCommandsAvailable();
    /// @return Whether toggle commands for sub items.
    static bool isToggleCommandsForSubItems();
    /// Sets object toggle state for selection.
    static void setObjectToggleStateForSelection( SelectionToggleType state );

    /// @return The matching collection name.
    static QString findCollectionName( SelectionToggleType state );

    /// @return The matching owner and child array field.
    static std::pair<caf::PdmObjectHandle*, caf::PdmChildArrayFieldHandle*> findOwnerAndChildArrayField( caf::PdmFieldHandle* fieldHandle );

private:
    /// @return The matching tree view.
    static caf::PdmUiTreeView*               findTreeView( const caf::PdmUiItem* uiItem );
    /// @return The matching tree item from selected UI item.
    static caf::PdmUiTreeOrdering*           findTreeItemFromSelectedUiItem( const caf::PdmUiItem* uiItem );
    /// @return The matching toggle fields from selection.
    static std::vector<caf::PdmField<bool>*> findToggleFieldsFromSelection( SelectionToggleType state );
};
