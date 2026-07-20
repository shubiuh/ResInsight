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
/// @brief Declares storage and handling of histogram data.

#pragma once

#include <cstddef>
#include <vector>

//==================================================================================================
///
///
//==================================================================================================
/// @brief Stores histogram data.
class RigHistogramData
{
public:
    RigHistogramData();

    /// Stores min.
    double              min;
    /// Stores max.
    double              max;
    /// Stores p10.
    double              p10;
    /// Stores p90.
    double              p90;
    /// Stores mean.
    double              mean;
    /// Stores sum.
    double              sum;
    /// Stores weighted mean.
    double              weightedMean;
    /// Stores histogram.
    std::vector<size_t> histogram;

    bool isMinMaxValid() const;
    bool isHistogramVectorValid() const;

private:
    bool isValid( double parameter ) const;
};
