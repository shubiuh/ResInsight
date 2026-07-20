/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2020 Equinor ASA
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
/// @brief NNC (non-neighbouring connection) property name helpers.

#pragma once

#include <QString>

namespace RiaDefines
{
/// @name Canonical non-neighbour connection result names
/// These keys identify phase flux, transmissibility, and multiplier results on NNC faces.
/// @{
QString propertyNameFluxWat();
QString propertyNameFluxOil();
QString propertyNameFluxGas();
QString propertyNameCombTrans();
QString propertyNameRiCombTrans();
QString propertyNameRiCombTransByArea();
QString propertyNameRiCombMult();
/// @}
}; // namespace RiaDefines
