/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2018- Equinor ASA
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
/// @brief Declares geo mech contour map projection reservoir-data functionality.

#pragma once

#include "RigContourMapCalculator.h"
#include "RigContourMapProjection.h"
#include "RigFemPart.h"
#include "RigFemResultAddress.h"

#include "cvfBoundingBox.h"

class RigGeoMechCaseData;

//==================================================================================================
///
///
//==================================================================================================
/// @brief Models geo mech contour map projection for reservoir-data processing.
class RigGeoMechContourMapProjection : public RigContourMapProjection
{
public:
    RigGeoMechContourMapProjection( RigGeoMechCaseData* caseData,
                                    const RigContourMapGrid*,
                                    bool   limitToPorePressureRegions,
                                    double paddingAroundPorePressureRegion );

    void generateAndSaveResults( RigFemResultAddress                            resultAddress,
                                 RigContourMapCalculator::ResultAggregationType resultAggregation,
                                 int                                            viewerStepIndex );

    std::vector<double> generateResultsFromAddress( RigFemResultAddress                            resultAddress,
                                                    const std::vector<bool>&                       mapCellVisibility,
                                                    RigContourMapCalculator::ResultAggregationType resultAggregation,
                                                    int                                            viewerStepIndex ) const;

    static cvf::BoundingBox calculateExpandedPorBarBBox( RigGeoMechCaseData& caseData,
                                                         const std::string&  resultComponentName,
                                                         int                 timeStep,
                                                         int                 frameIndex,
                                                         double              padding );

    std::vector<bool> getMapCellVisibility( int viewStepIndex, RigContourMapCalculator::ResultAggregationType resultAggregation ) override;

    std::vector<bool> getMapCellVisibility( RigFemResultAddress                            resAddr,
                                            int                                            viewStepIndex,
                                            RigContourMapCalculator::ResultAggregationType resultAggregation );

    bool isCellActive( size_t globalCellIdx ) const override;

protected:
    // GeoMech implementation specific data generation methods
    std::vector<size_t> findIntersectingCells( const cvf::BoundingBox& bbox ) const override;
    size_t              kLayer( size_t globalCellIdx ) const override;
    size_t              kLayers() const override;
    double              calculateOverlapVolume( size_t globalCellIdx, const cvf::BoundingBox& bbox ) const override;
    double calculateRayLengthInCell( size_t globalCellIdx, const cvf::Vec3d& highestPoint, const cvf::Vec3d& lowestPoint ) const override;
    double getParameterWeightForCell( size_t globalCellIdx, const std::vector<double>& parameterWeights ) const override;
    std::vector<double> gridCellValues( RigFemResultAddress resAddr, std::vector<float>& resultValues ) const;

protected:
    /// Stores case data.
    RigGeoMechCaseData*       m_caseData;
    /// Stores limit to pore pressure regions.
    bool                      m_limitToPorePressureRegions;
    /// Stores padding around pore pressure region.
    double                    m_paddingAroundPorePressureRegion;
    /// Stores fem part.
    cvf::ref<RigFemPart>      m_femPart;
    /// Stores fem part grid.
    cvf::cref<RigFemPartGrid> m_femPartGrid;
    /// Stores current result addr.
    RigFemResultAddress       m_currentResultAddr;
    /// Stores k layers.
    size_t                    m_kLayers;
};
