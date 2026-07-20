/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) Statoil ASA
//  Copyright (C) Ceetron Solutions AS
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
/// @brief Implements eclipse multi property stat calc reservoir-data functionality.

#include "RigEclipseMultiPropertyStatCalc.h"
#include "RigEclipseNativeStatCalc.h"

//--------------------------------------------------------------------------------------------------
/// Creates a RigEclipseMultiPropertyStatCalc instance.
//--------------------------------------------------------------------------------------------------
RigEclipseMultiPropertyStatCalc::RigEclipseMultiPropertyStatCalc()
{
}

//--------------------------------------------------------------------------------------------------
/// Adds statistics calculator.
//--------------------------------------------------------------------------------------------------
void RigEclipseMultiPropertyStatCalc::addStatisticsCalculator( RigStatisticsCalculator* statisticsCalculator )
{
    if ( statisticsCalculator )
    {
        m_nativeStatisticsCalculators.push_back( statisticsCalculator );
    }
}

//--------------------------------------------------------------------------------------------------
/// Returns the min max cell scalar values.
//--------------------------------------------------------------------------------------------------
void RigEclipseMultiPropertyStatCalc::minMaxCellScalarValues( size_t timeStepIndex, double& min, double& max )
{
    for ( size_t i = 0; i < m_nativeStatisticsCalculators.size(); i++ )
    {
        if ( m_nativeStatisticsCalculators.at( i ) )
        {
            m_nativeStatisticsCalculators.at( i )->minMaxCellScalarValues( timeStepIndex, min, max );
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// Returns the pos neg closest to zero.
//--------------------------------------------------------------------------------------------------
void RigEclipseMultiPropertyStatCalc::posNegClosestToZero( size_t timeStepIndex, double& pos, double& neg )
{
    for ( size_t i = 0; i < m_nativeStatisticsCalculators.size(); i++ )
    {
        if ( m_nativeStatisticsCalculators.at( i ) )
        {
            m_nativeStatisticsCalculators.at( i )->posNegClosestToZero( timeStepIndex, pos, neg );
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// Returns the value sum and sample count.
//--------------------------------------------------------------------------------------------------
void RigEclipseMultiPropertyStatCalc::valueSumAndSampleCount( size_t timeStepIndex, double& valueSum, size_t& sampleCount )
{
    for ( size_t i = 0; i < m_nativeStatisticsCalculators.size(); i++ )
    {
        if ( m_nativeStatisticsCalculators.at( i ) )
        {
            m_nativeStatisticsCalculators.at( i )->valueSumAndSampleCount( timeStepIndex, valueSum, sampleCount );
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// Adds data to histogram calculator.
//--------------------------------------------------------------------------------------------------
void RigEclipseMultiPropertyStatCalc::addDataToHistogramCalculator( size_t timeStepIndex, RigHistogramCalculator& histogramCalculator )
{
    for ( size_t i = 0; i < m_nativeStatisticsCalculators.size(); i++ )
    {
        if ( m_nativeStatisticsCalculators.at( i ) )
        {
            m_nativeStatisticsCalculators.at( i )->addDataToHistogramCalculator( timeStepIndex, histogramCalculator );
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// Returns the unique values.
//--------------------------------------------------------------------------------------------------
void RigEclipseMultiPropertyStatCalc::uniqueValues( size_t timeStepIndex, std::set<int>& values )
{
    for ( size_t i = 0; i < m_nativeStatisticsCalculators.size(); i++ )
    {
        if ( m_nativeStatisticsCalculators.at( i ) )
        {
            m_nativeStatisticsCalculators.at( i )->uniqueValues( timeStepIndex, values );
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// Returns the time step count.
//--------------------------------------------------------------------------------------------------
size_t RigEclipseMultiPropertyStatCalc::timeStepCount()
{
    if ( !m_nativeStatisticsCalculators.empty() )
    {
        return m_nativeStatisticsCalculators[0]->timeStepCount();
    }

    return 0;
}

//--------------------------------------------------------------------------------------------------
/// Adds native statistics calculator.
//--------------------------------------------------------------------------------------------------
void RigEclipseMultiPropertyStatCalc::addNativeStatisticsCalculator( RigCaseCellResultsData*        cellResultsData,
                                                                     const RigEclipseResultAddress& eclipseResultAddress )
{
    if ( eclipseResultAddress.isValid() )
    {
        addStatisticsCalculator( new RigEclipseNativeStatCalc( cellResultsData, eclipseResultAddress ) );
    }
}
