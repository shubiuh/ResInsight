/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2025 Equinor ASA
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
/// @brief Declares configuration for flooding.
#pragma once

/// @brief Stores configuration for flooding.
class RigFloodingSettings
{
public:
    /// Enumerates the supported flooding type values.
    enum class FloodingType
    {
        WATER_FLOODING,
        GAS_FLOODING,
        USER_DEFINED
    };

public:
    RigFloodingSettings( FloodingType oilFloodingType, double userDefFloodingOil, FloodingType gasFloodingType, double userDefFloodingGas );
    ~RigFloodingSettings();

    FloodingType oilFlooding() const;
    FloodingType gasFlooding() const;
    double       oilUserDefFlooding() const;
    double       gasUserDefFlooding() const;

protected:
    /// Stores oil flooding type.
    FloodingType m_oilFloodingType;
    /// Stores user def flooding oil.
    double       m_userDefFloodingOil;

    /// Stores gas flooding type.
    FloodingType m_gasFloodingType;
    /// Stores user def flooding gas.
    double       m_userDefFloodingGas;
};