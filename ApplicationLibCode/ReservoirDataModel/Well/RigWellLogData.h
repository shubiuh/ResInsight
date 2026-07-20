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
/// @brief Declares storage and handling of well log data.

#pragma once

#include "RiaDefines.h"

#include "cvfObject.h"

#include <QStringList>
#include <vector>

class RimWellLogCurve;

//==================================================================================================
///
//==================================================================================================
/// @brief Stores well log data.
class RigWellLogData : public cvf::Object
{
public:
    RigWellLogData();
    ~RigWellLogData() override;

    /// Returns or processes well log channel names.
    virtual QStringList wellLogChannelNames() const = 0;

    /// Returns or processes depth values.
    virtual std::vector<double> depthValues() const  = 0;
    /// Returns or processes tvd msl values.
    virtual std::vector<double> tvdMslValues() const = 0;
    /// Returns or processes tvd rkb values.
    virtual std::vector<double> tvdRkbValues() const = 0;

    /// Returns or processes values.
    virtual std::vector<double> values( const QString& name ) const = 0;

    /// Returns or processes well log channel unit string.
    virtual QString wellLogChannelUnitString( const QString& wellLogChannelName ) const = 0;
    /// Returns or processes depth unit string.
    virtual QString depthUnitString() const                                             = 0;

    QString convertedWellLogChannelUnitString( const QString& wellLogChannelName, RiaDefines::DepthUnitType displayDepthUnit ) const;
    RiaDefines::DepthUnitType depthUnit() const;

    /// Returns whether tvd msl channel is available.
    virtual bool hasTvdMslChannel() const = 0;
    /// Returns whether tvd rkb channel is available.
    virtual bool hasTvdRkbChannel() const = 0;

    /// Returns or processes get missing value.
    virtual double getMissingValue() const = 0;
};
