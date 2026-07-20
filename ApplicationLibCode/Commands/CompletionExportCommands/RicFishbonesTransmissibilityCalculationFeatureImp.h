/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017 Statoil ASA
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
/// @brief Declares fishbones transmissibility calculation command support.

#pragma once

#include "RicWellBorePartForTransCalc.h"

#include <map>
#include <vector>

class RigCompletionData;
class RimWellPath;
class RimEclipseCase;
class RimFishbones;
class RicExportCompletionDataSettingsUi;
class RigEclipseCaseData;

//==================================================================================================
/// @brief Supports fishbones transmissibility calculation command workflows.
//==================================================================================================
class RicFishbonesTransmissibilityCalculationFeatureImp
{
public:
    /// @return The generate fishbone compdat values using adjusted cell volume.
    static std::vector<RigCompletionData>
        generateFishboneCompdatValuesUsingAdjustedCellVolume( const RimWellPath* wellPath, const RicExportCompletionDataSettingsUi& settings );

    /// @return The matching fishbone laterals well bore parts.
    static std::map<size_t, std::vector<WellBorePartForTransCalc>> findFishboneLateralsWellBoreParts( const RimWellPath*    wellPath,
                                                                                                      const RimEclipseCase* eclipseCase );

private:
    /// Appends main well bore parts.
    static void appendMainWellBoreParts( std::map<size_t, std::vector<WellBorePartForTransCalc>>& wellBorePartsInCells,
                                         const RimWellPath*                                       wellPath,
                                         const RigEclipseCaseData*                                eclipseCaseData,
                                         double                                                   skinFactor,
                                         double                                                   holeRadius,
                                         double                                                   startMeasuredDepth,
                                         double                                                   endMeasuredDepth,
                                         const RimFishbones*                                      fishbonesDefinitions );
};
