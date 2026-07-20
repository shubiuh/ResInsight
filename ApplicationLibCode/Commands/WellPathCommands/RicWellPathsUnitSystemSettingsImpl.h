/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017-     Statoil ASA
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
/// @brief Declares well paths unit system settings command support.

#pragma once

#include "RiaDefines.h"

class RimWellPath;

//==================================================================================================
/// @brief Supports well paths unit system settings command workflows.
//==================================================================================================
class RicWellPathsUnitSystemSettingsImpl
{
public:
    /// @return The ensure has unit system.
    static bool ensureHasUnitSystem( RimWellPath* wellPath );
};
