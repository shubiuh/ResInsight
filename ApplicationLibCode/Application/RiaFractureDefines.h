/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2018-     Equinor ASA
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
/// @brief Fracture-related result name and unit helpers.

#pragma once

#include "RiaDefines.h"

#include <QString>

namespace RiaDefines
{
/// @return Canonical result identifier for fracture conductivity.
QString conductivityResultName();
/// @return Conductivity unit label appropriate for @p unitSystem.
QString unitStringConductivity( RiaDefines::EclipseUnitSystem unitSystem );

/// @return Unit-adjusted alpha coefficient used by the non-Darcy fracture-flow model.
double nonDarcyFlowAlpha( RiaDefines::EclipseUnitSystem unitSystem );

/// @name Standard fracture color-legend names
/// @{
QString faciesColorLegendName();
QString rockTypeColorLegendName();
/// @}
}; // namespace RiaDefines
