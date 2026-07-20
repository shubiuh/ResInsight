/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2023- Equinor ASA
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
/// @brief Declares create depth adjusted LAS files command support.

#pragma once

#include "RiaDefines.h"

#include "cvfObject.h"

class RigEclipseWellLogExtractor;
class RigGeoMechWellLogExtractor;
class RigResultAccessor;
class RigWellLogLasFile;
class RimCase;
class RimEclipseCase;
class RimWellPath;
class RimWellLogLasFile;

//==================================================================================================
/// Object to hold Depth value and its original index in a LAS file vector categorized by K-layer.
//==================================================================================================
class LasDepthValueAndIndexPerKLayer
{
public:
    /// Constructs the command object.
    LasDepthValueAndIndexPerKLayer() = default;

    /// Inserts index and value.
    void                     insertIndexAndValue( int kLayer, size_t index, double value );
    /// @return The index and value pairs.
    std::map<size_t, double> indexAndValuePairs( int kLayer ) const;
    /// @return Whether K layer.
    bool                     hasKLayer( int kLayer ) const;

private:
    // Map of K-layer and Index and Value pairs for LAS file depth vectors
    std::map<int, std::map<size_t, double>> m_kLayerIndexAndValuePairsMap;
};

//==================================================================================================
/// @brief Utilities for create depth adjusted LAS files command workflows.
//==================================================================================================
namespace RicCreateDepthAdjustedLasFilesImpl
{
/// @brief Data used to configure or execute index K depth data command workflows.
struct IndexKDepthData
{
    double mdTop     = 0.0;
    double mdBottom  = 0.0;
    double tvdTop    = 0.0;
    double tvdBottom = 0.0;
};

/// Creates index K result accessor.
cvf::ref<RigResultAccessor> createIndexKResultAccessor( RimEclipseCase* selectedCase );

/// Creates destination wells LAS files.
void createDestinationWellsLasFiles( RimCase*                        selectedCase,
                                     RimWellPath*                    sourceWell,
                                     RimWellLogLasFile*              soureWellLogFile,
                                     const std::vector<RimWellPath*> destinationWells,
                                     const std::vector<QString>&     selectedResultProperties,
                                     const QString&                  exportFolder,
                                     double                          rkbDiff );

/// Creates destination well LAS file.
void createDestinationWellLasFile( const QString&                                wellName,
                                   const QString&                                caseDescription,
                                   const std::vector<double>&                    mdValues,
                                   const std::vector<double>&                    tvdMslValues,
                                   const std::vector<double>&                    tvdRkbValues,
                                   const std::map<QString, std::vector<double>>& propertyMap,
                                   const RigWellLogLasFile*                      sourceWellLogData,
                                   const QString&                                exportFolder );

/// Creates depth unit text.
std::string createDepthUnitText( RiaDefines::DepthUnitType depthUnitType );
/// Creates depth unit comment.
std::string createDepthUnitComment( RiaDefines::DepthUnitType depthUnitType );

/// Creates LAS depth index and perc value per K layer from map.
LasDepthValueAndIndexPerKLayer createLasDepthIndexAndPercValuePerKLayerFromMap( const std::vector<double>&            lasWellDepths,
                                                                                const std::map<int, IndexKDepthData>& indexKDepthDataMap );
/// Creates index K depth data map from case.
std::map<int, IndexKDepthData> createIndexKDepthDataMapFromCase( RimCase* selectedCase, RimWellPath* wellPath );
/// Creates index K depth data map.
std::map<int, IndexKDepthData> createIndexKDepthDataMap( cvf::ref<RigEclipseWellLogExtractor> wellExtractor,
                                                         cvf::ref<RigResultAccessor>          indexKResAcc );
/// Creates index K depth data map.
std::map<int, IndexKDepthData> createIndexKDepthDataMap( cvf::ref<RigGeoMechWellLogExtractor> wellExtractor );

/// Creates index K depth data map from vectors.
std::map<int, IndexKDepthData> createIndexKDepthDataMapFromVectors( const std::vector<double>& wellMdValues,
                                                                    const std::vector<double>& wellTvdValues,
                                                                    const std::vector<double>& wellIndexKValues );

/// Creates default property map.
std::map<QString, std::vector<double>> createDefaultPropertyMap( const std::vector<QString>& selectedProperties,
                                                                 const RigWellLogLasFile*    wellLogFile );

}; // namespace RicCreateDepthAdjustedLasFilesImpl
