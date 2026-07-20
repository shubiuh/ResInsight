/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2026-     Equinor ASA
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
/// @brief Declares calculation of selected fault distance results.

#pragma once

#include <QString>

#include <vector>

class RigEclipseCaseData;
class RigFault;

//==================================================================================================
///
//==================================================================================================
/// @brief Calculates selected fault distance results.
class RigSelectedFaultDistanceResultCalculator
{
public:
    static void compute( RigEclipseCaseData* caseData, const QString& resultName, const std::vector<const RigFault*>& selectedFaults );
};
