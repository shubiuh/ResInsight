/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2026  Equinor ASA
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
/// @brief Declares utilities for well target mapping.

#pragma once

#include "RiaDefines.h"
#include "RigActiveCellInfo.h"
#include "RigEclipseResultAddress.h"
#include "RigWellTargetMapping.h"

#include "cafVecIjk.h"
#include "cvfBoundingBox.h"
#include "cvfStructGrid.h"

#include <list>
#include <map>
#include <optional>
#include <utility>
#include <vector>

class RigCaseCellResultsData;
class RigFloodingSettings;
class RigMainGrid;
class RimEclipseCase;

//==================================================================================================
///
///
//==================================================================================================
/// @brief Provides utilities for well target mapping.
class RigWellTargetMappingTools
{
public:
    /// Type alias used for cell face type.
    using CellFaceType     = cvf::StructGridInterface::FaceType;
    /// Type alias used for volume type.
    using VolumeType       = RigWellTargetMapping::VolumeType;
    /// Type alias used for volumes type.
    using VolumesType      = RigWellTargetMapping::VolumesType;
    /// Type alias used for volume result type.
    using VolumeResultType = RigWellTargetMapping::VolumeResultType;
    /// Type alias used for clustering limits.
    using ClusteringLimits = RigWellTargetMapping::ClusteringLimits;

    /// @brief Models data container for reservoir-data processing.
    struct DataContainer
    {
        /// Stores volume.
        std::vector<double>        volume;
        /// Stores saturation oil.
        std::vector<double>        saturationOil;
        /// Stores saturation gas.
        std::vector<double>        saturationGas;
        /// Stores pressure.
        std::vector<double>        pressure;
        /// Stores permeability x.
        std::vector<double>        permeabilityX;
        /// Stores permeability nnc.
        std::vector<double>        permeabilityNNC;
        /// Stores transmissibility x.
        std::vector<double>        transmissibilityX;
        /// Stores transmissibility y.
        std::vector<double>        transmissibilityY;
        /// Stores transmissibility z.
        std::vector<double>        transmissibilityZ;
        /// Stores transmissibility nnc.
        const std::vector<double>* transmissibilityNNC;
    };

    /// @brief Models cluster statistics for reservoir-data processing.
    class ClusterStatistics
    {
    public:
        ClusterStatistics()
            : id( -1 )
            , numCells( 0 )
            , totalPorvSoil( 0.0 )
            , totalPorvSgas( 0.0 )
            , totalPorvSoilAndSgas( 0.0 )
            , totalFipOil( 0.0 )
            , totalFipGas( 0.0 )
            , totalRfipOil( 0.0 )
            , totalRfipGas( 0.0 )
            , totalSfipOil( 0.0 )
            , totalSfipGas( 0.0 )
            , permeability( 0.0 )
            , pressure( 0.0 )
        {
        }

        /// Stores id.
        int    id;
        /// Stores num cells.
        size_t numCells;
        /// Stores total porv soil.
        double totalPorvSoil;
        /// Stores total porv sgas.
        double totalPorvSgas;
        /// Stores total porv soil and sgas.
        double totalPorvSoilAndSgas;
        /// Stores total fip oil.
        double totalFipOil;
        /// Stores total fip gas.
        double totalFipGas;
        /// Stores total rfip oil.
        double totalRfipOil;
        /// Stores total rfip gas.
        double totalRfipGas;
        /// Stores total sfip oil.
        double totalSfipOil;
        /// Stores total sfip gas.
        double totalSfipGas;
        /// Stores permeability.
        double permeability;
        /// Stores pressure.
        double pressure;
    };

    static double getValueForFace( const std::vector<double>& x,
                                   const std::vector<double>& y,
                                   const std::vector<double>& z,
                                   CellFaceType               face,
                                   ActiveCellIndex            resultIndex );

    static double getTransmissibilityValueForFace( const std::vector<double>& x,
                                                   const std::vector<double>& y,
                                                   const std::vector<double>& z,
                                                   CellFaceType               face,
                                                   ActiveCellIndex            resultIndex,
                                                   ActiveCellIndex            neighborResultIndex );

    static QString getOilVectorName( VolumesType volumesType );
    static QString getGasVectorName( VolumesType volumesType );

    static bool isSaturationSufficient( VolumeType volumeType, const DataContainer& data, const ClusteringLimits& limits, ActiveCellIndex idx );

    static void assignClusterIdToCells( const RigActiveCellInfo&               activeCellInfo,
                                        const std::vector<ReservoirCellIndex>& cells,
                                        std::vector<int>&                      clusters,
                                        int                                    clusterId );

    static std::list<std::pair<std::pair<ReservoirCellIndex, CellFaceType>, size_t>> nncConnectionCellAndResult( ReservoirCellIndex cellIdx,
                                                                                                                 RigMainGrid* mainGrid );

    static void createDynamicResultEntry( RigCaseCellResultsData* resultsData, const RigEclipseResultAddress& address );

    static void
        createResultVector( RimEclipseCase& eclipseCase, const QString& resultName, const std::vector<int>& clusterIds, size_t timeStepIdx );

    static void
        createResultVector( RimEclipseCase& eclipseCase, const QString& resultName, const std::vector<double>& values, size_t timeStepIdx );

    static void createStaticResultVector( RimEclipseCase& eclipseCase, const QString& resultName, const std::vector<int>& intValues );
    static void createStaticResultVector( RimEclipseCase& eclipseCase, const QString& resultName, const std::vector<double>& values );

    static void createResultVectorIfDefined( RimEclipseCase&            eclipseCase,
                                             const QString&             resultName,
                                             const std::vector<double>& values,
                                             int                        timeStepIdx = -1 );

    static std::optional<caf::VecIjk0> findStartCell( RimEclipseCase*            eclipseCase,
                                                      size_t                     timeStepIdx,
                                                      VolumeType                 volumeType,
                                                      const ClusteringLimits&    limits,
                                                      const DataContainer&       data,
                                                      const std::vector<double>& filterVector,
                                                      const std::vector<int>&    clusters );

    static void growCluster( RimEclipseCase*            eclipseCase,
                             const caf::VecIjk0&        startCell,
                             VolumeType                 volumeType,
                             const ClusteringLimits&    limits,
                             const DataContainer&       data,
                             const std::vector<double>& filterVector,
                             std::vector<int>&          clusters,
                             int                        clusterId,
                             size_t                     timeStepIdx,
                             int                        maxIterations );

    static std::vector<ReservoirCellIndex> findCandidates( RimEclipseCase*                        eclipseCase,
                                                           const std::vector<ReservoirCellIndex>& previousCells,
                                                           VolumeType                             volumeType,
                                                           const ClusteringLimits&                limits,
                                                           const DataContainer&                   data,
                                                           const std::vector<double>&             filterVector,
                                                           std::vector<int>&                      clusters );

    static std::optional<size_t> getActiveCellCount( RimEclipseCase* eclipseCase );

    static std::vector<ClusterStatistics> generateStatistics( RimEclipseCase*            eclipseCase,
                                                              const std::vector<double>& pressure,
                                                              const std::vector<double>& permeabilityX,
                                                              int                        numClustersFound,
                                                              size_t                     timeStepIdx,
                                                              const QString&             clusterResultName );

    static void computeStatisticsAndCreateVectors( RimEclipseCase&                         targetCase,
                                                   const QString&                          resultName,
                                                   const std::vector<std::vector<double>>& vec );

    static void accumulateResultsForSingleCase( RimEclipseCase&                                      eclipseCase,
                                                RimEclipseCase&                                      targetCase,
                                                std::map<QString, std::vector<std::vector<double>>>& resultNamesAndSamples,
                                                std::vector<int>&                                    occupancy,
                                                size_t                                               timeStepIdx );

    static cvf::BoundingBox computeBoundingBoxForResult( RimEclipseCase& eclipseCase, const QString& resultName, size_t timeStepIndex );

    static std::vector<double> loadVectorByName( RigCaseCellResultsData& resultsData, const QString& resultName, size_t timeStepIdx );

    static std::vector<double> loadOilVectorByName( RigCaseCellResultsData&    resultsData,
                                                    VolumesType                volumesType,
                                                    VolumeResultType           volumeResultType,
                                                    size_t                     timeStepIdx,
                                                    const RigFloodingSettings& floodingSettings );

    static std::vector<double> loadGasVectorByName( RigCaseCellResultsData&       resultsData,
                                                    RiaDefines::EclipseUnitSystem unitsType,
                                                    VolumesType                   volumesType,
                                                    VolumeResultType              volumeResultType,
                                                    size_t                        timeStepIdx,
                                                    const RigFloodingSettings&    floodingSettings );
};
