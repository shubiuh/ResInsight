/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Interface through which model objects expose fields eligible for Quick Access.
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

#include <map>
#include <vector>

#include <QString>

namespace caf
{
class PdmFieldHandle;
class PdmUiOrdering;
}; // namespace caf

/// @brief Supplies named groups of PDM fields that users may pin to the Quick Access panel.
///
/// Implementations return live, non-owning field pointers. Group names are UI identifiers and are
/// used to preserve the logical grouping when entries are added to a view.
class RimFieldQuickAccessInterface
{
public:
    /// @return Eligible fields grouped by their preferred Quick Access section name.
    virtual std::map<QString, std::vector<caf::PdmFieldHandle*>> quickAccessFields() = 0;
};
