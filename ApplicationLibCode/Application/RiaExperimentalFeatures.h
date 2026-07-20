/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2026     Equinor ASA
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
/// @brief Central registry of experimental (in-development) features.

#pragma once

#include <QString>

#include <vector>

//--------------------------------------------------------------------------------------------------
/// @brief Central, discoverable registry of opt-in experimental features.
///
/// To add a new experimental feature, append an entry to the list returned by availableFeatures()
/// (see RiaExperimentalFeatures.cpp) and gate the code path behind
/// RiaPreferencesSystem::isFeatureEnabled( "<keyword>" ).
///
/// Keywords are persisted in user preferences and must remain stable after release. The registry is
/// immutable after first access, which keeps references valid and gives the preferences UI a
/// deterministic order.
//--------------------------------------------------------------------------------------------------
class RiaExperimentalFeatures
{
public:
    /// Descriptive metadata used to expose and evaluate one feature flag.
    struct Feature
    {
        QString keyword; ///< Stable identifier, passed to RiaPreferencesSystem::isFeatureEnabled().
        QString displayName; ///< Human readable name shown as the checkbox label.
        QString description; ///< Short explanation, appended to the checkbox label.
    };

    /// Returns the single, immutable list of all experimental features.
    /// @return A process-lifetime reference in preferences display order.
    static const std::vector<Feature>& availableFeatures();
};
