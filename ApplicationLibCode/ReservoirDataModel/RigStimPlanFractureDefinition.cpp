/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017 -     Statoil ASA
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
/// @brief Implements stim plan fracture definition reservoir-data functionality.

#include "RigStimPlanFractureDefinition.h"

#include "RiaEclipseUnitTools.h"
#include "RiaFractureDefines.h"
#include "RiaLogging.h"

#include "RigFractureCell.h"
#include "RigFractureGrid.h"
#include "RigStatisticsMath.h"

#include "RivWellFracturePartMgr.h"

#include "cafAssert.h"

#include <QStringList>

#include <cmath>

//--------------------------------------------------------------------------------------------------
/// Internal functions
//--------------------------------------------------------------------------------------------------
size_t findMirrorXIndex( std::vector<double> xs );

//--------------------------------------------------------------------------------------------------
/// Numeric tolerance used when comparing fracture result values.
//--------------------------------------------------------------------------------------------------
const double RigStimPlanFractureDefinition::THRESHOLD_VALUE = 1e-5;

//--------------------------------------------------------------------------------------------------
/// Creates a RigStimPlanFractureDefinition instance.
//--------------------------------------------------------------------------------------------------
RigStimPlanFractureDefinition::RigStimPlanFractureDefinition()
    : m_unitSet( RiaDefines::EclipseUnitSystem::UNITS_UNKNOWN )
    , m_xMirrorMode( false )
    , m_topPerfTvd( HUGE_VAL )
    , m_bottomPerfTvd( HUGE_VAL )
    , m_topPerfMd( HUGE_VAL )
    , m_bottomPerfMd( HUGE_VAL )
    , m_formationDip( HUGE_VAL )
    , m_orientation( Orientation::UNDEFINED )
{
}

//--------------------------------------------------------------------------------------------------
/// Destroys the RigStimPlanFractureDefinition instance.
//--------------------------------------------------------------------------------------------------
RigStimPlanFractureDefinition::~RigStimPlanFractureDefinition()
{
}

//--------------------------------------------------------------------------------------------------
/// Returns the unit set.
//--------------------------------------------------------------------------------------------------
RiaDefines::EclipseUnitSystem RigStimPlanFractureDefinition::unitSet() const
{
    return m_unitSet;
}

//--------------------------------------------------------------------------------------------------
/// Returns the x count.
//--------------------------------------------------------------------------------------------------
size_t RigStimPlanFractureDefinition::xCount() const
{
    return m_Xs.size();
}

//--------------------------------------------------------------------------------------------------
/// Returns the y count.
//--------------------------------------------------------------------------------------------------
size_t RigStimPlanFractureDefinition::yCount() const
{
    return m_Ys.size();
}

//--------------------------------------------------------------------------------------------------
/// Returns the min depth.
//--------------------------------------------------------------------------------------------------
double RigStimPlanFractureDefinition::minDepth() const
{
    return -minY();
}

//--------------------------------------------------------------------------------------------------
/// Returns the max depth.
//--------------------------------------------------------------------------------------------------
double RigStimPlanFractureDefinition::maxDepth() const
{
    return -maxY();
}

//--------------------------------------------------------------------------------------------------
/// Returns the top perf tvd.
//--------------------------------------------------------------------------------------------------
double RigStimPlanFractureDefinition::topPerfTvd() const
{
    return m_topPerfTvd;
}

//--------------------------------------------------------------------------------------------------
/// Returns the bottom perf tvd.
//--------------------------------------------------------------------------------------------------
double RigStimPlanFractureDefinition::bottomPerfTvd() const
{
    return m_bottomPerfTvd;
}

//--------------------------------------------------------------------------------------------------
/// Returns the min y.
//--------------------------------------------------------------------------------------------------
double RigStimPlanFractureDefinition::minY() const
{
    CAF_ASSERT( !m_Ys.empty() );
    return m_Ys[0];
}

//--------------------------------------------------------------------------------------------------
/// Returns the max y.
//--------------------------------------------------------------------------------------------------
double RigStimPlanFractureDefinition::maxY() const
{
    CAF_ASSERT( !m_Ys.empty() );
    return m_Ys.back();
}

//--------------------------------------------------------------------------------------------------
/// Returns the ys.
//--------------------------------------------------------------------------------------------------
const std::vector<double>& RigStimPlanFractureDefinition::ys() const
{
    return m_Ys;
}

//--------------------------------------------------------------------------------------------------
/// Returns the xs.
//--------------------------------------------------------------------------------------------------
const std::vector<double>& RigStimPlanFractureDefinition::xs() const
{
    return m_Xs;
}

//--------------------------------------------------------------------------------------------------
/// Computes scaled xs.
//--------------------------------------------------------------------------------------------------
std::vector<double> RigStimPlanFractureDefinition::computeScaledXs( const std::vector<double>& xs, double scaleFactor )
{
    std::vector<double> scaledXs;

    // Scale using 0 as scaling anchor
    for ( double x : xs )
    {
        if ( scaleFactor != 1.0 ) x *= scaleFactor;
        scaledXs.push_back( x );
    }

    return scaledXs;
}

//--------------------------------------------------------------------------------------------------
/// Computes scaled ys.
//--------------------------------------------------------------------------------------------------
std::vector<double>
    RigStimPlanFractureDefinition::computeScaledYs( const std::vector<double>& ys, double scaleFactor, double wellPathIntersectionY )
{
    std::vector<double> scaledYs;

    // Scale using wellPathIntersectionY as scaling anchor
    for ( double y : ys )
    {
        if ( scaleFactor != 1.0 ) y = ( y - wellPathIntersectionY ) * scaleFactor + wellPathIntersectionY;
        scaledYs.push_back( y );
    }

    return scaledYs;
}

//--------------------------------------------------------------------------------------------------
/// Sets tvd to top perf.
//--------------------------------------------------------------------------------------------------
void RigStimPlanFractureDefinition::setTvdToTopPerf( double topPerfTvd )
{
    m_topPerfTvd = topPerfTvd;
}

//--------------------------------------------------------------------------------------------------
/// Sets tvd to bottom perf.
//--------------------------------------------------------------------------------------------------
void RigStimPlanFractureDefinition::setTvdToBottomPerf( double bottomPerfTvd )
{
    m_bottomPerfTvd = bottomPerfTvd;
}

//--------------------------------------------------------------------------------------------------
/// Returns the top perf md.
//--------------------------------------------------------------------------------------------------
double RigStimPlanFractureDefinition::topPerfMd() const
{
    return m_topPerfMd;
}

//--------------------------------------------------------------------------------------------------
/// Returns the bottom perf md.
//--------------------------------------------------------------------------------------------------
double RigStimPlanFractureDefinition::bottomPerfMd() const
{
    return m_bottomPerfMd;
}

//--------------------------------------------------------------------------------------------------
/// Sets md to top perf.
//--------------------------------------------------------------------------------------------------
void RigStimPlanFractureDefinition::setMdToTopPerf( double topPerfMd )
{
    m_topPerfMd = topPerfMd;
}

//--------------------------------------------------------------------------------------------------
/// Sets md to bottom perf.
//--------------------------------------------------------------------------------------------------
void RigStimPlanFractureDefinition::setMdToBottomPerf( double bottomPerfMd )
{
    m_bottomPerfMd = bottomPerfMd;
}

//--------------------------------------------------------------------------------------------------
/// Generates xs from file xs.
//--------------------------------------------------------------------------------------------------
void RigStimPlanFractureDefinition::generateXsFromFileXs( bool xMirrorMode )
{
    m_xMirrorMode = xMirrorMode;
    m_Xs.clear();

    if ( m_xMirrorMode )
    {
        size_t            mirrorIndex = findMirrorXIndex( m_fileXs );
        std::list<double> xs;

        // Mirror positive X values
        xs.push_back( m_fileXs[mirrorIndex] );
        for ( size_t i = mirrorIndex + 1; i < m_fileXs.size(); i++ )
        {
            xs.push_front( -m_fileXs[i] );
            xs.push_back( m_fileXs[i] );
        }
        m_Xs = std::vector<double>( xs.begin(), xs.end() );
    }
    else
    {
        m_Xs = m_fileXs;
    }
}

//--------------------------------------------------------------------------------------------------
/// Generates data layout from file data layout.
//--------------------------------------------------------------------------------------------------
std::vector<std::vector<double>>
    RigStimPlanFractureDefinition::generateDataLayoutFromFileDataLayout( std::vector<std::vector<double>> fileXYData ) const
{
    if ( m_xMirrorMode )
    {
        std::vector<std::vector<double>> xyData;
        size_t                           mirrorIndex = findMirrorXIndex( m_fileXs );

        for ( const auto& yData : fileXYData )
        {
            std::list<double> xValues;

            // Mirror positive X values
            xValues.push_back( yData[mirrorIndex] );
            for ( size_t x = mirrorIndex + 1; x < yData.size(); x++ )
            {
                xValues.push_front( yData[x] );
                xValues.push_back( yData[x] );
            }
            xyData.push_back( std::vector<double>( xValues.begin(), xValues.end() ) );
        }
        return xyData;
    }
    else
    {
        return fileXYData;
    }
}

//--------------------------------------------------------------------------------------------------
/// Returns the number of parameter values ok.
//--------------------------------------------------------------------------------------------------
bool RigStimPlanFractureDefinition::numberOfParameterValuesOK( std::vector<std::vector<double>> propertyValuesAtTimestep ) const
{
    size_t xCount = m_Xs.size();

    if ( propertyValuesAtTimestep.size() != yCount() ) return false;
    for ( const std::vector<double>& valuesAtDepthVector : propertyValuesAtTimestep )
    {
        if ( valuesAtDepthVector.size() != xCount ) return false;
    }

    return true;
}

//--------------------------------------------------------------------------------------------------
/// Returns the adjusted y coords around well path position.
//--------------------------------------------------------------------------------------------------
std::vector<double> RigStimPlanFractureDefinition::adjustedYCoordsAroundWellPathPosition( const std::vector<double>& ys,
                                                                                          double wellPathIntersectionAtFractureDepth )
{
    std::vector<double> yRelativeToWellPath;

    for ( double y : ys )
    {
        double adjustedDepth = y + wellPathIntersectionAtFractureDepth;
        yRelativeToWellPath.push_back( adjustedDepth );
    }
    return yRelativeToWellPath;
}

//--------------------------------------------------------------------------------------------------
/// Returns stim plan property names units.
//--------------------------------------------------------------------------------------------------
std::vector<std::pair<QString, QString>> RigStimPlanFractureDefinition::getStimPlanPropertyNamesUnits() const
{
    std::vector<std::pair<QString, QString>> propertyNamesUnits;
    {
        for ( const RigStimPlanResultFrames& stimPlanDataEntry : m_stimPlanResults )
        {
            propertyNamesUnits.push_back( std::make_pair( stimPlanDataEntry.resultName, stimPlanDataEntry.unit ) );
        }
    }
    return propertyNamesUnits;
}

//--------------------------------------------------------------------------------------------------
/// Returns the conductivity values at time step.
//--------------------------------------------------------------------------------------------------
std::vector<std::vector<double>> RigStimPlanFractureDefinition::conductivityValuesAtTimeStep( const QString& resultName,
                                                                                              int            activeTimeStepIndex,
                                                                                              RiaDefines::EclipseUnitSystem requiredUnitSet ) const
{
    std::vector<std::vector<double>> conductivityValues;

    QString conductivityUnitTextOnFile;

    std::vector<std::pair<QString, QString>> propertyNamesUnitsOnFile = getStimPlanPropertyNamesUnits();
    for ( auto properyNameUnit : propertyNamesUnitsOnFile )
    {
        if ( resultName == properyNameUnit.first )
        {
            conductivityUnitTextOnFile = properyNameUnit.second;
        }
    }

    if ( conductivityUnitTextOnFile.isEmpty() )
    {
        RiaLogging::error( "Did not find unit for conductivity on file" );

        return conductivityValues;
    }

    conductivityValues = getDataAtTimeIndex( resultName, conductivityUnitTextOnFile, activeTimeStepIndex );

    // Check that the data is in the required unit system.
    // Convert if not the case.
    if ( requiredUnitSet != unitSet() )
    {
        // Convert to the conductivity unit system used by the fracture template
        // The conductivity value is used in the computations of transmissibility when exporting COMPDAT, and has unit
        // md-m or md-ft This unit must match the unit used to represent coordinates of the grid used for export

        for ( auto& yValues : conductivityValues )
        {
            for ( auto& xVal : yValues )
            {
                if ( requiredUnitSet == RiaDefines::EclipseUnitSystem::UNITS_FIELD )
                {
                    xVal = RiaEclipseUnitTools::convertToFeet( xVal, conductivityUnitTextOnFile );
                }
                else if ( requiredUnitSet == RiaDefines::EclipseUnitSystem::UNITS_METRIC )
                {
                    xVal = RiaEclipseUnitTools::convertToMeter( xVal, conductivityUnitTextOnFile );
                }
            }
        }
    }

    return conductivityValues;
}

//--------------------------------------------------------------------------------------------------
/// Creates fracture grid.
//--------------------------------------------------------------------------------------------------
cvf::cref<RigFractureGrid> RigStimPlanFractureDefinition::createFractureGrid( const QString& resultName,
                                                                              int            activeTimeStepIndex,
                                                                              double         xScaleFactor,
                                                                              double         yScaleFactor,
                                                                              double         wellPathIntersectionAtFractureDepth,
                                                                              RiaDefines::EclipseUnitSystem requiredUnitSet ) const
{
    std::vector<std::vector<double>> conductivityValues = conductivityValuesAtTimeStep( resultName, activeTimeStepIndex, requiredUnitSet );
    if ( conductivityValues.empty() )
    {
        return nullptr;
    }

    std::vector<RigFractureCell> stimPlanCells;
    std::pair<size_t, size_t>    wellCenterStimPlanCellIJ = std::make_pair( 0, 0 );

    bool wellCenterStimPlanCellFound = false;

    std::vector<double> scaledXs = computeScaledXs( m_Xs, xScaleFactor );
    std::vector<double> scaledYs = computeScaledYs( m_Ys, yScaleFactor, -wellPathIntersectionAtFractureDepth );

    std::vector<double> yCoordsAtNodes = adjustedYCoordsAroundWellPathPosition( scaledYs, wellPathIntersectionAtFractureDepth );

    std::vector<double> xCoordsAtNodes = scaledXs;

    std::vector<double> xCoords;
    for ( int i = 0; i < static_cast<int>( xCoordsAtNodes.size() ) - 1; i++ )
        xCoords.push_back( ( xCoordsAtNodes[i] + xCoordsAtNodes[i + 1] ) / 2 );
    std::vector<double> depthCoords;
    for ( int i = 0; i < static_cast<int>( yCoordsAtNodes.size() ) - 1; i++ )
        depthCoords.push_back( ( yCoordsAtNodes[i] + yCoordsAtNodes[i + 1] ) / 2 );

    for ( int i = 0; i < static_cast<int>( xCoords.size() ) - 1; i++ )
    {
        for ( int j = 0; j < static_cast<int>( depthCoords.size() ) - 1; j++ )
        {
            std::vector<cvf::Vec3d> cellPolygon;
            cellPolygon.push_back( cvf::Vec3d( xCoords[i], depthCoords[j], 0.0 ) );
            cellPolygon.push_back( cvf::Vec3d( xCoords[i + 1], depthCoords[j], 0.0 ) );
            cellPolygon.push_back( cvf::Vec3d( xCoords[i + 1], depthCoords[j + 1], 0.0 ) );
            cellPolygon.push_back( cvf::Vec3d( xCoords[i], depthCoords[j + 1], 0.0 ) );

            RigFractureCell stimPlanCell( cellPolygon, i, j );
            if ( !conductivityValues.empty() ) // Assuming vector to be of correct length, or no values
            {
                stimPlanCell.setConductivityValue( conductivityValues[j + 1][i + 1] );
            }
            else
            {
                stimPlanCell.setConductivityValue( cvf::UNDEFINED_DOUBLE );
            }

            // The well path is intersecting the fracture at origo in the fracture coordinate system
            // Find the Stimplan cell where the well path is intersecting

            if ( cellPolygon[0].x() <= 0.0 && cellPolygon[1].x() >= 0.0 )
            {
                if ( cellPolygon[1].y() >= 0.0 && cellPolygon[2].y() <= 0.0 )
                {
                    wellCenterStimPlanCellIJ    = std::make_pair( stimPlanCell.getI(), stimPlanCell.getJ() );
                    wellCenterStimPlanCellFound = true;
                }
            }

            stimPlanCells.push_back( stimPlanCell );
        }
    }

    if ( !wellCenterStimPlanCellFound )
    {
        RiaLogging::error( "Did not find stim plan cell at well crossing!" );
    }

    cvf::ref<RigFractureGrid> fractureGrid = new RigFractureGrid;
    fractureGrid->setFractureCells( stimPlanCells );
    fractureGrid->setWellCenterFractureCellIJ( wellCenterStimPlanCellIJ );
    fractureGrid->setICellCount( m_Xs.size() - 2 );
    fractureGrid->setJCellCount( m_Ys.size() - 2 );
    fractureGrid->ensureCellSearchTreeIsBuilt();

    return cvf::cref<RigFractureGrid>( fractureGrid.p() );
}

//--------------------------------------------------------------------------------------------------
/// Returns the fracture grid results.
//--------------------------------------------------------------------------------------------------
std::vector<double>
    RigStimPlanFractureDefinition::fractureGridResults( const QString& resultName, const QString& unitName, size_t timeStepIndex ) const
{
    std::vector<double>                     fractureGridResults;
    const std::vector<std::vector<double>>& resultValuesAtTimeStep = getDataAtTimeIndex( resultName, unitName, timeStepIndex );

    for ( int i = 0; i < static_cast<int>( xCount() ) - 2; i++ )
    {
        for ( int j = 0; j < static_cast<int>( yCount() ) - 2; j++ )
        {
            if ( j + 1 < static_cast<int>( resultValuesAtTimeStep.size() ) && i + 1 < static_cast<int>( resultValuesAtTimeStep[j + 1].size() ) )
            {
                fractureGridResults.push_back( resultValuesAtTimeStep[j + 1][i + 1] );
            }
            else
            {
                fractureGridResults.push_back( HUGE_VAL );
            }
        }
    }

    return fractureGridResults;
}

//--------------------------------------------------------------------------------------------------
/// Creates fracture triangle geometry.
//--------------------------------------------------------------------------------------------------
void RigStimPlanFractureDefinition::createFractureTriangleGeometry( double                   xScaleFactor,
                                                                    double                   yScaleFactor,
                                                                    double                   wellPathIntersectionAtFractureDepth,
                                                                    const QString&           fractureUserName,
                                                                    std::vector<cvf::Vec3f>* vertices,
                                                                    std::vector<cvf::uint>*  triangleIndices ) const
{
    std::vector<double> scaledXs = computeScaledXs( m_Xs, xScaleFactor );
    std::vector<double> scaledYs = computeScaledYs( m_Ys, yScaleFactor, -wellPathIntersectionAtFractureDepth );

    std::vector<double> xCoords    = scaledXs;
    cvf::uint           lenXcoords = static_cast<cvf::uint>( xCoords.size() );

    std::vector<double> adjustedYs = adjustedYCoordsAroundWellPathPosition( scaledYs, wellPathIntersectionAtFractureDepth );

    for ( cvf::uint k = 0; k < adjustedYs.size(); k++ )
    {
        for ( cvf::uint i = 0; i < lenXcoords; i++ )
        {
            cvf::Vec3f node = cvf::Vec3f( xCoords[i], adjustedYs[k], 0 );
            vertices->push_back( node );

            if ( i < lenXcoords - 1 && k < adjustedYs.size() - 1 )
            {
                if ( xCoords[i] < THRESHOLD_VALUE )
                {
                    // Upper triangle
                    triangleIndices->push_back( i + k * lenXcoords );
                    triangleIndices->push_back( ( i + 1 ) + k * lenXcoords );
                    triangleIndices->push_back( ( i + 1 ) + ( k + 1 ) * lenXcoords );
                    // Lower triangle
                    triangleIndices->push_back( i + k * lenXcoords );
                    triangleIndices->push_back( ( i + 1 ) + ( k + 1 ) * lenXcoords );
                    triangleIndices->push_back( ( i ) + ( k + 1 ) * lenXcoords );
                }
                else
                {
                    // Upper triangle
                    triangleIndices->push_back( i + k * lenXcoords );
                    triangleIndices->push_back( ( i + 1 ) + k * lenXcoords );
                    triangleIndices->push_back( ( i ) + ( k + 1 ) * lenXcoords );
                    // Lower triangle
                    triangleIndices->push_back( ( i + 1 ) + k * lenXcoords );
                    triangleIndices->push_back( ( i + 1 ) + ( k + 1 ) * lenXcoords );
                    triangleIndices->push_back( ( i ) + ( k + 1 ) * lenXcoords );
                }
            }
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// Returns the time steps.
//--------------------------------------------------------------------------------------------------
const std::vector<double>& RigStimPlanFractureDefinition::timeSteps() const
{
    return m_timeSteps;
}

//--------------------------------------------------------------------------------------------------
/// Adds time step.
//--------------------------------------------------------------------------------------------------
void RigStimPlanFractureDefinition::addTimeStep( double time )
{
    if ( !timeStepExists( time ) ) m_timeSteps.push_back( time );
}

//--------------------------------------------------------------------------------------------------
/// Returns the time step exists.
//--------------------------------------------------------------------------------------------------
bool RigStimPlanFractureDefinition::timeStepExists( double timeStepValueToCheck ) const
{
    for ( double timeStep : m_timeSteps )
    {
        if ( fabs( timeStepValueToCheck - timeStep ) < THRESHOLD_VALUE ) return true;
    }
    return false;
}

//--------------------------------------------------------------------------------------------------
/// Returns time step index.
//--------------------------------------------------------------------------------------------------
size_t RigStimPlanFractureDefinition::getTimeStepIndex( double timeStepValue ) const
{
    size_t index = 0;
    while ( index < m_timeSteps.size() )
    {
        if ( fabs( m_timeSteps[index] - timeStepValue ) < 1e-4 )
        {
            return index;
        }
        index++;
    }
    return -1; // returns -1 if not found
}

//--------------------------------------------------------------------------------------------------
/// Returns the total number time steps.
//--------------------------------------------------------------------------------------------------
size_t RigStimPlanFractureDefinition::totalNumberTimeSteps() const
{
    return m_timeSteps.size();
}

//--------------------------------------------------------------------------------------------------
/// Returns the result index.
//--------------------------------------------------------------------------------------------------
size_t RigStimPlanFractureDefinition::resultIndex( const QString& resultName, const QString& unit ) const
{
    for ( size_t i = 0; i < m_stimPlanResults.size(); i++ )
    {
        if ( m_stimPlanResults[i].resultName == resultName && m_stimPlanResults[i].unit == unit )
        {
            return i;
        }
    }

    return cvf::UNDEFINED_SIZE_T;
}

//--------------------------------------------------------------------------------------------------
/// Sets data at time value.
//--------------------------------------------------------------------------------------------------
void RigStimPlanFractureDefinition::setDataAtTimeValue( const QString&                          resultName,
                                                        const QString&                          unit,
                                                        const std::vector<std::vector<double>>& data,
                                                        double                                  timeStepValue )
{
    size_t resIndex = resultIndex( resultName, unit );

    if ( resIndex != cvf::UNDEFINED_SIZE_T )
    {
        m_stimPlanResults[resIndex].parameterValues[getTimeStepIndex( timeStepValue )] = data;
    }
    else
    {
        RigStimPlanResultFrames resultData;

        resultData.resultName = resultName;
        resultData.unit       = unit;

        std::vector<std::vector<std::vector<double>>> values( m_timeSteps.size() );
        resultData.parameterValues                                    = values;
        resultData.parameterValues[getTimeStepIndex( timeStepValue )] = data;

        m_stimPlanResults.push_back( resultData );
    }
}

//--------------------------------------------------------------------------------------------------
/// Returns data at time index.
//--------------------------------------------------------------------------------------------------
const std::vector<std::vector<double>>&
    RigStimPlanFractureDefinition::getDataAtTimeIndex( const QString& resultName, const QString& unit, size_t timeStepIndex ) const
{
    size_t resIndex = resultIndex( resultName, unit );

    if ( resIndex != cvf::UNDEFINED_SIZE_T )
    {
        if ( timeStepIndex < m_stimPlanResults[resIndex].parameterValues.size() )
        {
            return m_stimPlanResults[resIndex].parameterValues[timeStepIndex];
        }
    }

    static std::vector<std::vector<double>> emptyVector;
    return emptyVector;
}

//--------------------------------------------------------------------------------------------------
/// Appends data to result statistics.
//--------------------------------------------------------------------------------------------------
void RigStimPlanFractureDefinition::appendDataToResultStatistics( const QString&     resultName,
                                                                  const QString&     unit,
                                                                  MinMaxAccumulator& minMaxAccumulator,
                                                                  PosNegAccumulator& posNegAccumulator ) const
{
    size_t resIndex = resultIndex( resultName, unit );
    if ( resIndex == cvf::UNDEFINED_SIZE_T ) return;

    for ( const auto& timeValues : m_stimPlanResults[resIndex].parameterValues )
    {
        for ( const auto& values : timeValues )
        {
            minMaxAccumulator.addData( values );
            posNegAccumulator.addData( values );
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// Returns the conductivity result names.
//--------------------------------------------------------------------------------------------------
QStringList RigStimPlanFractureDefinition::conductivityResultNames() const
{
    QStringList resultNames;

    for ( const auto& stimPlanResult : m_stimPlanResults )
    {
        if ( stimPlanResult.resultName.contains( RiaDefines::conductivityResultName(), Qt::CaseInsensitive ) )
        {
            resultNames.push_back( stimPlanResult.resultName );
        }
    }

    return resultNames;
}

//--------------------------------------------------------------------------------------------------
/// Finds mirror x index.
//--------------------------------------------------------------------------------------------------
size_t findMirrorXIndex( std::vector<double> xs )
{
    size_t mirrorIndex = cvf::UNDEFINED_SIZE_T;

    for ( size_t i = 0; i < xs.size(); i++ )
    {
        if ( xs[i] > -RigStimPlanFractureDefinition::THRESHOLD_VALUE )
        {
            mirrorIndex = i;
            break;
        }
    }

    return mirrorIndex;
}

//--------------------------------------------------------------------------------------------------
/// Returns the formation dip.
//--------------------------------------------------------------------------------------------------
double RigStimPlanFractureDefinition::formationDip() const
{
    return m_formationDip;
}

//--------------------------------------------------------------------------------------------------
/// Sets formation dip.
//--------------------------------------------------------------------------------------------------
void RigStimPlanFractureDefinition::setFormationDip( double formationDip )
{
    m_formationDip = formationDip;
}

//--------------------------------------------------------------------------------------------------
/// Returns the orientation.
//--------------------------------------------------------------------------------------------------
RigStimPlanFractureDefinition::Orientation RigStimPlanFractureDefinition::orientation() const
{
    return m_orientation;
}

//--------------------------------------------------------------------------------------------------
/// Sets orientation.
//--------------------------------------------------------------------------------------------------
void RigStimPlanFractureDefinition::setOrientation( RigStimPlanFractureDefinition::Orientation orientation )
{
    m_orientation = orientation;
}
