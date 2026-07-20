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
/// @brief Implements storage and handling of well log data.

#include "RigWellLogData.h"

//--------------------------------------------------------------------------------------------------
/// Creates a RigWellLogData instance.
//--------------------------------------------------------------------------------------------------
RigWellLogData::RigWellLogData()
    : cvf::Object()
{
}

//--------------------------------------------------------------------------------------------------
/// Destroys the RigWellLogData instance.
//--------------------------------------------------------------------------------------------------
RigWellLogData::~RigWellLogData()
{
}

//--------------------------------------------------------------------------------------------------
/// Returns the depth unit.
//--------------------------------------------------------------------------------------------------
RiaDefines::DepthUnitType RigWellLogData::depthUnit() const
{
    RiaDefines::DepthUnitType unitType = RiaDefines::DepthUnitType::UNIT_METER;

    if ( depthUnitString().toUpper() == "F" || depthUnitString().toUpper() == "FT" )
    {
        unitType = RiaDefines::DepthUnitType::UNIT_FEET;
    }

    return unitType;
}

//--------------------------------------------------------------------------------------------------
/// Converts converted well log channel unit string.
//--------------------------------------------------------------------------------------------------
QString RigWellLogData::convertedWellLogChannelUnitString( const QString& wellLogChannelName, RiaDefines::DepthUnitType displayDepthUnit ) const
{
    QString unit = wellLogChannelUnitString( wellLogChannelName );

    if ( unit == depthUnitString() )
    {
        if ( displayDepthUnit != depthUnit() )
        {
            if ( displayDepthUnit == RiaDefines::DepthUnitType::UNIT_METER )
            {
                return "M";
            }
            else if ( displayDepthUnit == RiaDefines::DepthUnitType::UNIT_FEET )
            {
                return "FT";
            }
            else if ( displayDepthUnit == RiaDefines::DepthUnitType::UNIT_NONE )
            {
                CVF_ASSERT( false );
                return "";
            }
        }
    }

    return unit;
}
