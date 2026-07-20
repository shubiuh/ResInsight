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
/// @brief Declares geo mech property filter command support.

#pragma once

#include <cstddef>
#include <vector>

class RimGeoMechPropertyFilter;
class RimGeoMechPropertyFilterCollection;

/// @brief Utilities for caf command workflows.
namespace caf
{
class PdmObjectHandle;
}

//==================================================================================================
/// @brief Shared implementation helpers for geo mech property filter commands.
//==================================================================================================
class RicGeoMechPropertyFilterFeatureImpl
{
public:
    /// @return The selected property filters.
    static std::vector<RimGeoMechPropertyFilter*>           selectedPropertyFilters();
    /// @return The selected property filter collections.
    static std::vector<RimGeoMechPropertyFilterCollection*> selectedPropertyFilterCollections();

    /// Adds property filter.
    static void addPropertyFilter( RimGeoMechPropertyFilterCollection* propertyFilterCollection );
    /// Inserts property filter.
    static void insertPropertyFilter( RimGeoMechPropertyFilterCollection* propertyFilterCollection, size_t index );

    /// @return Whether property filter command available.
    static bool isPropertyFilterCommandAvailable( caf::PdmObjectHandle* object );

private:
    /// Creates property filter.
    static RimGeoMechPropertyFilter* createPropertyFilter( RimGeoMechPropertyFilterCollection* propertyFilterCollection );
    /// Sets defaults.
    static void                      setDefaults( RimGeoMechPropertyFilter* propertyFilter );
};
