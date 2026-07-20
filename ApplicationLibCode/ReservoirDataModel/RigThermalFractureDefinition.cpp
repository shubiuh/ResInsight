/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2022 -     Equinor ASA
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
/// @brief Implements thermal fracture definition reservoir-data functionality.

#include "RigThermalFractureDefinition.h"

#include "RiaEclipseUnitTools.h"
#include "RiaLogging.h"

#include "cafAssert.h"

#include <cmath>

//--------------------------------------------------------------------------------------------------
/// Creates a RigThermalFractureDefinition instance.
//--------------------------------------------------------------------------------------------------
RigThermalFractureDefinition::RigThermalFractureDefinition()
    : m_unitSystem( RiaDefines::EclipseUnitSystem::UNITS_UNKNOWN )
{
}

//--------------------------------------------------------------------------------------------------
/// Destroys the RigThermalFractureDefinition instance.
//--------------------------------------------------------------------------------------------------
RigThermalFractureDefinition::~RigThermalFractureDefinition()
{
}

//--------------------------------------------------------------------------------------------------
/// Sets name.
//--------------------------------------------------------------------------------------------------
void RigThermalFractureDefinition::setName( const QString& name )
{
    m_name = name;
}

//--------------------------------------------------------------------------------------------------
/// Returns the name.
//--------------------------------------------------------------------------------------------------
QString RigThermalFractureDefinition::name() const
{
    return m_name;
}

//--------------------------------------------------------------------------------------------------
/// Sets unit system.
//--------------------------------------------------------------------------------------------------
void RigThermalFractureDefinition::setUnitSystem( RiaDefines::EclipseUnitSystem unitSystem )
{
    m_unitSystem = unitSystem;
}

//--------------------------------------------------------------------------------------------------
/// Returns the unit system.
//--------------------------------------------------------------------------------------------------
RiaDefines::EclipseUnitSystem RigThermalFractureDefinition::unitSystem() const
{
    return m_unitSystem;
}

//--------------------------------------------------------------------------------------------------
/// Returns the num nodes.
//--------------------------------------------------------------------------------------------------
size_t RigThermalFractureDefinition::numNodes() const
{
    if ( m_results.empty() ) return 0u;

    return m_results[0].numNodes();
}

//--------------------------------------------------------------------------------------------------
/// Returns the time steps.
//--------------------------------------------------------------------------------------------------
const std::vector<double>& RigThermalFractureDefinition::timeSteps() const
{
    return m_timeSteps;
}

//--------------------------------------------------------------------------------------------------
/// Adds time step.
//--------------------------------------------------------------------------------------------------
void RigThermalFractureDefinition::addTimeStep( double timeStep )
{
    m_timeSteps.push_back( timeStep );
}

//--------------------------------------------------------------------------------------------------
/// Returns the num time steps.
//--------------------------------------------------------------------------------------------------
size_t RigThermalFractureDefinition::numTimeSteps() const
{
    return m_timeSteps.size();
}

//--------------------------------------------------------------------------------------------------
/// Adds property.
//--------------------------------------------------------------------------------------------------
void RigThermalFractureDefinition::addProperty( const QString& name, const QString& unit )
{
    m_results.push_back( RigThermalFractureResult( name, unit ) );
}

//--------------------------------------------------------------------------------------------------
/// Returns property names units.
//--------------------------------------------------------------------------------------------------
std::vector<std::pair<QString, QString>> RigThermalFractureDefinition::getPropertyNamesUnits() const
{
    std::vector<std::pair<QString, QString>> namesAndUnits;
    for ( auto r : m_results )
        namesAndUnits.push_back( std::make_pair( r.name(), r.unit() ) );

    return namesAndUnits;
}

//--------------------------------------------------------------------------------------------------
/// Appends property value.
//--------------------------------------------------------------------------------------------------
void RigThermalFractureDefinition::appendPropertyValue( int propertyIndex, int nodeIndex, double value )
{
    CAF_ASSERT( propertyIndex >= 0 );
    CAF_ASSERT( propertyIndex < static_cast<int>( m_results.size() ) );

    m_results[propertyIndex].appendValue( nodeIndex, value );
}

//--------------------------------------------------------------------------------------------------
/// Returns property value.
//--------------------------------------------------------------------------------------------------
double RigThermalFractureDefinition::getPropertyValue( int propertyIndex, int nodeIndex, int timeStepIndex ) const
{
    return m_results[propertyIndex].getValue( nodeIndex, timeStepIndex );
}

//--------------------------------------------------------------------------------------------------
/// Returns property index.
//--------------------------------------------------------------------------------------------------
int RigThermalFractureDefinition::getPropertyIndex( const QString& name ) const
{
    for ( size_t i = 0; i < m_results.size(); i++ )
        if ( name == m_results[i].name() ) return static_cast<int>( i );

    return -1;
};

//--------------------------------------------------------------------------------------------------
/// Returns the relative coordinates.
//--------------------------------------------------------------------------------------------------
std::vector<cvf::Vec3d> RigThermalFractureDefinition::relativeCoordinates( int timeStepIndex ) const
{
    std::vector<cvf::Vec3d> relCoords;

    int xIndex = getPropertyIndex( "XCoord" );
    int yIndex = getPropertyIndex( "YCoord" );
    int zIndex = getPropertyIndex( "ZCoord" );
    if ( xIndex == -1 || yIndex == -1 || zIndex == -1 )
    {
        return relCoords;
    }

    // The first node is the center node
    int        centerNodeIndex = 0;
    cvf::Vec3d centerNode( getPropertyValue( xIndex, centerNodeIndex, timeStepIndex ),
                           getPropertyValue( yIndex, centerNodeIndex, timeStepIndex ),
                           getPropertyValue( zIndex, centerNodeIndex, timeStepIndex ) );

    for ( size_t nodeIndex = 0; nodeIndex < numNodes(); nodeIndex++ )
    {
        cvf::Vec3d nodePos( getPropertyValue( xIndex, static_cast<int>( nodeIndex ), timeStepIndex ),
                            getPropertyValue( yIndex, static_cast<int>( nodeIndex ), timeStepIndex ),
                            getPropertyValue( zIndex, static_cast<int>( nodeIndex ), timeStepIndex ) );
        relCoords.push_back( nodePos - centerNode );
    }

    return relCoords;
}

//--------------------------------------------------------------------------------------------------
/// Returns the center position.
//--------------------------------------------------------------------------------------------------
cvf::Vec3d RigThermalFractureDefinition::centerPosition() const
{
    int xIndex = getPropertyIndex( "XCoord" );
    int yIndex = getPropertyIndex( "YCoord" );
    int zIndex = getPropertyIndex( "ZCoord" );
    if ( xIndex == -1 || yIndex == -1 || zIndex == -1 )
    {
        return cvf::Vec3d::UNDEFINED;
    }

    // The first node is the center node
    int        centerNodeIndex = 0;
    int        timeStepIndex   = 0;
    cvf::Vec3d centerNode( getPropertyValue( xIndex, centerNodeIndex, timeStepIndex ),
                           getPropertyValue( yIndex, centerNodeIndex, timeStepIndex ),
                           getPropertyValue( zIndex, centerNodeIndex, timeStepIndex ) );
    return centerNode;
}

//--------------------------------------------------------------------------------------------------
/// Returns bounding box.
//--------------------------------------------------------------------------------------------------
cvf::BoundingBox RigThermalFractureDefinition::getBoundingBox( int timeStepIndex ) const
{
    std::vector<cvf::Vec3d> coords;

    cvf::BoundingBox bb;

    int xIndex = getPropertyIndex( "XCoord" );
    int yIndex = getPropertyIndex( "YCoord" );
    int zIndex = getPropertyIndex( "ZCoord" );
    if ( xIndex == -1 || yIndex == -1 || zIndex == -1 )
    {
        return bb;
    }

    for ( size_t nodeIndex = 0; nodeIndex < numNodes(); nodeIndex++ )
    {
        cvf::Vec3d nodePos( getPropertyValue( xIndex, static_cast<int>( nodeIndex ), timeStepIndex ),
                            getPropertyValue( yIndex, static_cast<int>( nodeIndex ), timeStepIndex ),
                            getPropertyValue( zIndex, static_cast<int>( nodeIndex ), timeStepIndex ) );
        bb.add( nodePos );
    }

    return bb;
}

//--------------------------------------------------------------------------------------------------
/// Returns the min depth.
//--------------------------------------------------------------------------------------------------
double RigThermalFractureDefinition::minDepth( int timeStepIndex ) const
{
    return getBoundingBox( timeStepIndex ).min().z();
}

//--------------------------------------------------------------------------------------------------
/// Returns the max depth.
//--------------------------------------------------------------------------------------------------
double RigThermalFractureDefinition::maxDepth( int timeStepIndex ) const
{
    return getBoundingBox( timeStepIndex ).max().z();
}
