/////////////////////////////////////////////////////////////////////////////////
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
/// @file
/// @brief OSDU data-kind string constants and token path helper.

#pragma once

#include <QString>

namespace RiaOsduDefines
{
/// @name OSDU record-kind identifiers
/// @{
QString osduFieldKind();
QString osduWellKind();
QString osduWellboreKind();
QString osduWellboreTrajectoryKind();
QString osduWellLogKind();
/// @}

/// @return Local path used to cache the current user's OSDU access token.
QString tokenPath();

}; // namespace RiaOsduDefines
