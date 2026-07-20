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
/// @brief Declares Eclipse property filter command support.

#pragma once

#include <cstddef>
#include <vector>

class RimCombinedFilter;
class RimEclipsePropertyFilter;
class RimEclipsePropertyFilterCollection;

/// @brief Utilities for caf command workflows.
namespace caf
{
class PdmObjectHandle;
}

//==================================================================================================
/// @brief Shared implementation helpers for Eclipse property filter commands.
//==================================================================================================
class RicEclipsePropertyFilterFeatureImpl
{
public:
    /// @return The selected property filters.
    static std::vector<RimEclipsePropertyFilter*>           selectedPropertyFilters();
    /// @return The selected property filter collections.
    static std::vector<RimEclipsePropertyFilterCollection*> selectedPropertyFilterCollections();

    // Looks at the strict selection first, then at any selected RimFilterInViewCollection facade. Returns
    // nullptr if neither is selected. Used by the property-filter "New ..." commands so they fire
    // when invoked from the unified facade tree node.
    static RimEclipsePropertyFilterCollection* resolveTargetPropertyFilterCollection();

    /// Adds property filter.
    static void                      addPropertyFilter( RimEclipsePropertyFilterCollection* propertyFilterCollection );
    /// Adds property filter to combined filter.
    static RimEclipsePropertyFilter* addPropertyFilterToCombinedFilter( RimCombinedFilter* combined );
    /// Inserts property filter.
    static void                      insertPropertyFilter( RimEclipsePropertyFilterCollection* propertyFilterCollection, size_t index );

    /// @return Whether property filter command available.
    static bool isPropertyFilterCommandAvailable( caf::PdmObjectHandle* object );

private:
    /// Sets defaults.
    static void setDefaults( RimEclipsePropertyFilter* propertyFilter );
};
