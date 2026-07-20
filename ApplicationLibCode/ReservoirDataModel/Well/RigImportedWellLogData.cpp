/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2025     Equinor ASA
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
/// @brief Implements storage and handling of imported well log data.

#include "RigImportedWellLogData.h"

#include <QStringList>

#include <cmath>

const double RigImportedWellLogData::MISSING_VALUE = -9999.0;

//--------------------------------------------------------------------------------------------------
/// Creates a RigImportedWellLogData instance.
//--------------------------------------------------------------------------------------------------
RigImportedWellLogData::RigImportedWellLogData()
    : RigWellLogData()
{
}

//--------------------------------------------------------------------------------------------------
/// Destroys the RigImportedWellLogData instance.
//--------------------------------------------------------------------------------------------------
RigImportedWellLogData::~RigImportedWellLogData()
{
}

//--------------------------------------------------------------------------------------------------
/// Returns the well log channel names.
//--------------------------------------------------------------------------------------------------
QStringList RigImportedWellLogData::wellLogChannelNames() const
{
    QStringList channelNames;
    for ( const auto& channelPair : m_channelData )
    {
        channelNames << channelPair.first;
    }
    return channelNames;
}

//--------------------------------------------------------------------------------------------------
/// Returns the depth values.
//--------------------------------------------------------------------------------------------------
std::vector<double> RigImportedWellLogData::depthValues() const
{
    return m_depthValues;
}

//--------------------------------------------------------------------------------------------------
/// Returns the tvd msl values.
//--------------------------------------------------------------------------------------------------
std::vector<double> RigImportedWellLogData::tvdMslValues() const
{
    return m_tvdMslValues;
}

//--------------------------------------------------------------------------------------------------
/// Returns the tvd rkb values.
//--------------------------------------------------------------------------------------------------
std::vector<double> RigImportedWellLogData::tvdRkbValues() const
{
    return m_tvdRkbValues;
}

//--------------------------------------------------------------------------------------------------
/// Returns the values.
//--------------------------------------------------------------------------------------------------
std::vector<double> RigImportedWellLogData::values( const QString& name ) const
{
    auto it = m_channelData.find( name );
    if ( it != m_channelData.end() )
    {
        return it->second;
    }
    return std::vector<double>();
}

//--------------------------------------------------------------------------------------------------
/// Returns the well log channel unit string.
//--------------------------------------------------------------------------------------------------
QString RigImportedWellLogData::wellLogChannelUnitString( const QString& wellLogChannelName ) const
{
    // For imported data, assume dimensionless units
    Q_UNUSED( wellLogChannelName )
    return QString();
}

//--------------------------------------------------------------------------------------------------
/// Returns the depth unit string.
//--------------------------------------------------------------------------------------------------
QString RigImportedWellLogData::depthUnitString() const
{
    return "M";
}

//--------------------------------------------------------------------------------------------------
/// Returns whether tvd msl channel is available.
//--------------------------------------------------------------------------------------------------
bool RigImportedWellLogData::hasTvdMslChannel() const
{
    return !m_tvdMslValues.empty();
}

//--------------------------------------------------------------------------------------------------
/// Returns whether tvd rkb channel is available.
//--------------------------------------------------------------------------------------------------
bool RigImportedWellLogData::hasTvdRkbChannel() const
{
    return !m_tvdRkbValues.empty();
}

//--------------------------------------------------------------------------------------------------
/// Returns missing value.
//--------------------------------------------------------------------------------------------------
double RigImportedWellLogData::getMissingValue() const
{
    return MISSING_VALUE;
}

//--------------------------------------------------------------------------------------------------
/// Sets channel data.
//--------------------------------------------------------------------------------------------------
void RigImportedWellLogData::setChannelData( const QString& channelName, const std::vector<double>& values )
{
    m_channelData[channelName] = values;
}

//--------------------------------------------------------------------------------------------------
/// Sets depth values.
//--------------------------------------------------------------------------------------------------
void RigImportedWellLogData::setDepthValues( const std::vector<double>& depthValues )
{
    m_depthValues = depthValues;
}

//--------------------------------------------------------------------------------------------------
/// Sets tvd msl values.
//--------------------------------------------------------------------------------------------------
void RigImportedWellLogData::setTvdMslValues( const std::vector<double>& tvdMslValues )
{
    m_tvdMslValues = tvdMslValues;
}

//--------------------------------------------------------------------------------------------------
/// Sets tvd rkb values.
//--------------------------------------------------------------------------------------------------
void RigImportedWellLogData::setTvdRkbValues( const std::vector<double>& tvdRkbValues )
{
    m_tvdRkbValues = tvdRkbValues;
}