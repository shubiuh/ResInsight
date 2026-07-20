/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2024-  Equinor ASA
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
/// @brief Declares well target mapping reservoir-data functionality.

#pragma once

#include "RigEclipseResultAddress.h"

#include "cvfVector3.h"

#include <vector>

class RigCaseCellResultsData;
class RimEclipseCase;
class RimRegularGridCase;
class RigFloodingSettings;

//==================================================================================================
///
///
//==================================================================================================
/// @brief Models well target mapping for reservoir-data processing.
class RigWellTargetMapping
{
public:
    /// Enumerates the supported volume type values.
    enum class VolumeType
    {
        OIL,
        GAS,
        HYDROCARBON
    };

    /// Enumerates the supported volume result type values.
    enum class VolumeResultType
    {
        MOBILE,
        TOTAL
    };

    /// Enumerates the supported volumes type values.
    enum class VolumesType
    {
        RESERVOIR_VOLUMES,
        SURFACE_VOLUMES_SFIP,
        SURFACE_VOLUMES_FIP,
        RESERVOIR_VOLUMES_COMPUTED
    };

    /// @brief Models clustering limits for reservoir-data processing.
    struct ClusteringLimits
    {
        /// Stores saturation oil.
        double                  saturationOil;
        /// Stores saturation gas.
        double                  saturationGas;
        /// Stores permeability.
        double                  permeability;
        /// Stores pressure.
        double                  pressure;
        /// Stores transmissibility.
        double                  transmissibility;
        /// Stores max num targets.
        int                     maxNumTargets;
        /// Stores max iterations.
        int                     maxIterations;
        /// Stores filter address.
        RigEclipseResultAddress filterAddress;
        /// Stores filter.
        std::vector<double>     filter;
    };

    static void generateCandidates( RimEclipseCase*            eclipseCase,
                                    size_t                     timeStepIdx,
                                    VolumeType                 volumeType,
                                    VolumesType                volumesType,
                                    VolumeResultType           volumeResultType,
                                    const RigFloodingSettings& floodingSettings,
                                    const ClusteringLimits&    limits,
                                    bool                       skipUndefinedResults,
                                    bool                       setTimeStepInView );

    static std::vector<double> getVolumeVector( RigCaseCellResultsData&       resultsData,
                                                RiaDefines::EclipseUnitSystem unitsType,
                                                VolumeType                    volumeType,
                                                VolumesType                   volumesType,
                                                VolumeResultType              volumeResultType,
                                                size_t                        timeStepIdx,
                                                const RigFloodingSettings&    floodingSettings );

    static RimRegularGridCase* generateEnsembleCandidates( const std::vector<RimEclipseCase*>& cases,
                                                           size_t                              timeStepIdx,
                                                           const cvf::Vec3st&                  resultGridCellCount,
                                                           VolumeType                          volumeType,
                                                           VolumesType                         volumesType,
                                                           VolumeResultType                    volumeResultType,
                                                           const RigFloodingSettings&          floodingSettings,
                                                           const ClusteringLimits&             limits );

    static QString wellTargetResultName();
};
