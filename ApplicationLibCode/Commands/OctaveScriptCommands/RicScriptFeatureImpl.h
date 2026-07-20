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
/// @brief Declares script command support.

#pragma once

#include "cafCmdFeature.h"

#include <vector>

class RimCalcScript;
class RimScriptCollection;

//==================================================================================================
/// @brief Shared implementation helpers for script commands.
//==================================================================================================
class RicScriptFeatureImpl
{
public:
    /// @return The selected scripts.
    static std::vector<RimCalcScript*>       selectedScripts();
    /// @return The selected script collections.
    static std::vector<RimScriptCollection*> selectedScriptCollections();
};
