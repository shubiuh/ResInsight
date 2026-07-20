/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2016-     Statoil ASA
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
/// @brief Declares ensemble parameter reservoir-data functionality.

#pragma once

#include "RiaDefines.h"

#include <QString>
#include <QVariant>

#include <utility>
#include <vector>

//==================================================================================================
///
//==================================================================================================
/// @brief Models ensemble parameter for reservoir-data processing.
class RigEnsembleParameter
{
public:
    /// Enumerates the supported type values.
    enum Type
    {
        TYPE_NONE,
        TYPE_NUMERIC,
        TYPE_TEXT
    };
    /// Enumerates the supported bins values.
    enum Bins
    {
        NO_VARIATION  = -1,
        LOW_VARIATION = 0,
        MEDIUM_VARIATION,
        HIGH_VARIATION,
        NR_OF_VARIATION_BINS
    };
    QString               uiName() const;
    /// Stores name.
    QString               name;
    /// Stores type.
    Type                  type;
    /// Stores values.
    std::vector<QVariant> values;
    /// Stores min value.
    double                minValue;
    /// Stores max value.
    double                maxValue;
    /// Stores variation bin.
    int                   variationBin;

    RigEnsembleParameter()
        : type( TYPE_NONE )
        , minValue( std::numeric_limits<double>::infinity() )
        , maxValue( -std::numeric_limits<double>::infinity() )
        , variationBin( static_cast<int>( MEDIUM_VARIATION ) )
    {
    }

    /// Returns whether valid.
    bool   isValid() const { return !name.isEmpty() && type != TYPE_NONE; }
    /// Returns whether numeric.
    bool   isNumeric() const { return type == TYPE_NUMERIC; }
    /// Returns whether text.
    bool   isText() const { return type == TYPE_TEXT; }
    double normalizedStdDeviation() const;

    bool operator<( const RigEnsembleParameter& other ) const;

private:
    double stdDeviation() const;
};
