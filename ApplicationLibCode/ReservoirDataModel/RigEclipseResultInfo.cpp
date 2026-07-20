/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017  Statoil ASA
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
/// @brief Implements eclipse result info reservoir-data functionality.

#include "RigEclipseResultInfo.h"

#include "cvfAssert.h"

//--------------------------------------------------------------------------------------------------
/// Creates a RigEclipseTimeStepInfo instance.
//--------------------------------------------------------------------------------------------------
RigEclipseTimeStepInfo::RigEclipseTimeStepInfo( const QDateTime& date, int reportNumber, double daysSinceSimulationStart )
    : m_date( date )
    , m_reportNumber( reportNumber )
    , m_daysSinceSimulationStart( daysSinceSimulationStart )
{
}

//--------------------------------------------------------------------------------------------------
/// Creates time step infos.
//--------------------------------------------------------------------------------------------------
std::vector<RigEclipseTimeStepInfo> RigEclipseTimeStepInfo::createTimeStepInfos( std::vector<QDateTime> dates,
                                                                                 std::vector<int>       reportNumbers,
                                                                                 std::vector<double>    daysSinceSimulationStarts )
{
    CVF_ASSERT( dates.size() == reportNumbers.size() );
    CVF_ASSERT( dates.size() == daysSinceSimulationStarts.size() );

    std::vector<RigEclipseTimeStepInfo> timeStepInfos;

    for ( size_t i = 0; i < dates.size(); i++ )
    {
        timeStepInfos.push_back( RigEclipseTimeStepInfo( dates[i], reportNumbers[i], daysSinceSimulationStarts[i] ) );
    }

    return timeStepInfos;
}

//--------------------------------------------------------------------------------------------------
/// Creates a RigEclipseResultInfo instance.
//--------------------------------------------------------------------------------------------------
RigEclipseResultInfo::RigEclipseResultInfo( const RigEclipseResultAddress& resultAddress,
                                            bool                           needsToBeStored,
                                            bool                           mustBeCalculated,
                                            size_t                         gridScalarResultIndex )
    : m_resultAddress( resultAddress )
    , m_needsToBeStored( needsToBeStored )
    , m_mustBeCalculated( mustBeCalculated )
    , m_gridScalarResultIndex( gridScalarResultIndex )
{
}

//--------------------------------------------------------------------------------------------------
/// Returns the result type.
//--------------------------------------------------------------------------------------------------
RiaDefines::ResultCatType RigEclipseResultInfo::resultType() const
{
    return m_resultAddress.resultCatType();
}

//--------------------------------------------------------------------------------------------------
/// Sets result type.
//--------------------------------------------------------------------------------------------------
void RigEclipseResultInfo::setResultType( RiaDefines::ResultCatType newType )
{
    m_resultAddress.setResultCatType( newType );
}

//--------------------------------------------------------------------------------------------------
/// Returns the result name.
//--------------------------------------------------------------------------------------------------
const QString& RigEclipseResultInfo::resultName() const
{
    return m_resultAddress.resultName();
}

//--------------------------------------------------------------------------------------------------
/// Sets result name.
//--------------------------------------------------------------------------------------------------
void RigEclipseResultInfo::setResultName( const QString& name )
{
    m_resultAddress.setResultName( name );
}

//--------------------------------------------------------------------------------------------------
/// Returns the needs to be stored.
//--------------------------------------------------------------------------------------------------
bool RigEclipseResultInfo::needsToBeStored() const
{
    return m_needsToBeStored;
}

//--------------------------------------------------------------------------------------------------
/// Returns the must be calculated.
//--------------------------------------------------------------------------------------------------
bool RigEclipseResultInfo::mustBeCalculated() const
{
    return m_mustBeCalculated;
}

//--------------------------------------------------------------------------------------------------
/// Sets must be calculated.
//--------------------------------------------------------------------------------------------------
void RigEclipseResultInfo::setMustBeCalculated( bool mustCalculate )
{
    m_mustBeCalculated = mustCalculate;
}

//--------------------------------------------------------------------------------------------------
/// Returns the grid scalar result index.
//--------------------------------------------------------------------------------------------------
size_t RigEclipseResultInfo::gridScalarResultIndex() const
{
    return m_gridScalarResultIndex;
}

//--------------------------------------------------------------------------------------------------
/// Returns the eclipse result address.
//--------------------------------------------------------------------------------------------------
const RigEclipseResultAddress& RigEclipseResultInfo::eclipseResultAddress() const
{
    return m_resultAddress;
}

//--------------------------------------------------------------------------------------------------
/// Returns the time step infos.
//--------------------------------------------------------------------------------------------------
const std::vector<RigEclipseTimeStepInfo>& RigEclipseResultInfo::timeStepInfos() const
{
    return m_timeStepInfos;
}

//--------------------------------------------------------------------------------------------------
/// Sets time step infos.
//--------------------------------------------------------------------------------------------------
void RigEclipseResultInfo::setTimeStepInfos( const std::vector<RigEclipseTimeStepInfo>& timeSteps )
{
    m_timeStepInfos = timeSteps;
}

//--------------------------------------------------------------------------------------------------
/// Returns the dates.
//--------------------------------------------------------------------------------------------------
std::vector<QDateTime> RigEclipseResultInfo::dates() const
{
    std::vector<QDateTime> values;

    for ( auto v : m_timeStepInfos )
    {
        values.push_back( v.m_date );
    }

    return values;
}

//--------------------------------------------------------------------------------------------------
/// Returns the days since simulation starts.
//--------------------------------------------------------------------------------------------------
std::vector<double> RigEclipseResultInfo::daysSinceSimulationStarts() const
{
    std::vector<double> values;

    for ( auto v : m_timeStepInfos )
    {
        values.push_back( v.m_daysSinceSimulationStart );
    }

    return values;
}

//--------------------------------------------------------------------------------------------------
/// Returns the report numbers.
//--------------------------------------------------------------------------------------------------
std::vector<int> RigEclipseResultInfo::reportNumbers() const
{
    std::vector<int> values;

    for ( auto v : m_timeStepInfos )
    {
        values.push_back( v.m_reportNumber );
    }

    return values;
}

//--------------------------------------------------------------------------------------------------
/// Ordering operator for set storage. Just the type and name are used to find unique addresses.
//--------------------------------------------------------------------------------------------------
bool RigEclipseResultInfo::operator<( const RigEclipseResultInfo& rhs ) const
{
    return m_resultAddress < rhs.m_resultAddress;
}
