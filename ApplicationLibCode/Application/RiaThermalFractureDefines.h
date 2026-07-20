/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2022-     Equinor ASA
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
/// @brief Thermal fracture result name helpers.

#pragma once

#include "RiaDefines.h"

#include <QString>

namespace RiaDefines
{
/// @name Canonical thermal-fracture result identifiers
/// @{
QString leakoffPressureDropResultName();
QString filtratePressureDropResultName();
QString leakoffMobilityResultName();
QString filterCakeMobilityResultName();
QString injectivityFactorResultName();
QString viscosityResultName();
QString filtrateThicknessResultName();
/// @}

/// Returns the expected display unit for thermal-fracture result @p name in @p unitSystem.
QString getExpectedThermalFractureUnit( const QString& name, RiaDefines::EclipseUnitSystem unitSystem );

}; // namespace RiaDefines
