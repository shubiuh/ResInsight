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
/// @brief Implements storage and handling of completion data.

#include "RigCompletionData.h"

#include "RiaLogging.h"

#include "cvfAssert.h"

#include <QString>

#include <cmath>
#include <limits>

//==================================================================================================
/// Creates a RigCompletionData instance.
//==================================================================================================
RigCompletionData::RigCompletionData( const QString& wellName, const RigCompletionDataGridCell& cellIndex, double orderingValue )
    : m_wellName( wellName )
    , m_cellIndex( cellIndex )
    , m_saturation( std::numeric_limits<double>::infinity() )
    , m_transmissibility( std::numeric_limits<double>::infinity() )
    , m_diameter( std::numeric_limits<double>::infinity() )
    , m_kh( std::numeric_limits<double>::infinity() )
    , m_skinFactor( std::numeric_limits<double>::infinity() )
    , m_dFactor( std::numeric_limits<double>::infinity() )
    , m_direction( CellDirection::DIR_UNDEF )
    , m_wpimult( std::numeric_limits<double>::infinity() )
    , m_isMainBore( false )
    , m_completionType( CompletionType::CT_UNDEFINED )
    , m_firstOrderingValue( orderingValue )
    , m_secondOrderingValue( std::numeric_limits<double>::infinity() )
    , m_startMD( std::nullopt )
    , m_endMD( std::nullopt )
    , m_completionNumber( std::nullopt )
{
}

//==================================================================================================
/// Destroys the RigCompletionData instance.
//==================================================================================================
RigCompletionData::~RigCompletionData()
{
}

//==================================================================================================
/// Creates a RigCompletionData instance.
//==================================================================================================
RigCompletionData::RigCompletionData( const RigCompletionData& other )
{
    copy( *this, other );
}

//==================================================================================================
/// Implements the operator< operation.
//==================================================================================================
bool RigCompletionData::operator<( const RigCompletionData& other ) const
{
    if ( m_wellName != other.m_wellName )
    {
        return ( m_wellName < other.m_wellName );
    }

    if ( m_completionType != other.m_completionType )
    {
        return ( m_completionType < other.m_completionType );
    }

    if ( m_firstOrderingValue != other.m_firstOrderingValue )
    {
        return ( m_firstOrderingValue < other.m_firstOrderingValue );
    }

    if ( m_secondOrderingValue != other.m_secondOrderingValue )
    {
        return ( m_secondOrderingValue < other.m_secondOrderingValue );
    }

    return m_cellIndex < other.m_cellIndex;
}

//==================================================================================================
/// Implements the operator= operation.
//==================================================================================================
RigCompletionData& RigCompletionData::operator=( const RigCompletionData& other )
{
    if ( this != &other )
    {
        copy( *this, other );
    }
    return *this;
}

//--------------------------------------------------------------------------------------------------
/// Returns whether perforation valve.
//--------------------------------------------------------------------------------------------------
bool RigCompletionData::isPerforationValve( CompletionType type )
{
    return type == CompletionType::PERFORATION_AICD || type == CompletionType::PERFORATION_ICD || type == CompletionType::PERFORATION_ICV ||
           type == CompletionType::PERFORATION_SICD;
}

//--------------------------------------------------------------------------------------------------
/// Returns whether valve.
//--------------------------------------------------------------------------------------------------
bool RigCompletionData::isValve( CompletionType type )
{
    return isPerforationValve( type ) || type == CompletionType::FISHBONES_ICD;
}

//--------------------------------------------------------------------------------------------------
/// Returns whether wseg valve types.
//--------------------------------------------------------------------------------------------------
bool RigCompletionData::isWsegValveTypes( CompletionType type )
{
    return type == CompletionType::FISHBONES_ICD || type == CompletionType::PERFORATION_ICD || type == CompletionType::PERFORATION_ICV;
}

//==================================================================================================
/// Sets from fracture.
//==================================================================================================
void RigCompletionData::setFromFracture( double transmissibility, double skinFactor, double diameter )
{
    m_completionType   = CompletionType::FRACTURE;
    m_transmissibility = transmissibility;
    m_skinFactor       = skinFactor;
    m_diameter         = diameter;
}

//--------------------------------------------------------------------------------------------------
/// Sets second ordering value.
//--------------------------------------------------------------------------------------------------
void RigCompletionData::setSecondOrderingValue( double orderingValue )
{
    m_secondOrderingValue = orderingValue;
}

//--------------------------------------------------------------------------------------------------
/// Sets diameter.
//--------------------------------------------------------------------------------------------------
void RigCompletionData::setDiameter( double diameter )
{
    m_diameter = diameter;
}

//--------------------------------------------------------------------------------------------------
/// Sets transmissibility.
//--------------------------------------------------------------------------------------------------
void RigCompletionData::setTransmissibility( double transmissibility )
{
    m_transmissibility = transmissibility;
}

//==================================================================================================
/// Sets trans and wp imult background data from fishbone.
//==================================================================================================
void RigCompletionData::setTransAndWPImultBackgroundDataFromFishbone( double        transmissibility,
                                                                      double        skinFactor,
                                                                      double        diameter,
                                                                      double        kh,
                                                                      CellDirection direction,
                                                                      bool          isMainBore )
{
    m_completionType   = CompletionType::FISHBONES;
    m_transmissibility = transmissibility;
    m_skinFactor       = skinFactor;
    m_diameter         = diameter;
    m_kh               = kh;
    m_direction        = direction;
    m_isMainBore       = isMainBore;
}

//==================================================================================================
/// Sets trans and wp imult background data from perforation.
//==================================================================================================
void RigCompletionData::setTransAndWPImultBackgroundDataFromPerforation( double        transmissibility,
                                                                         double        skinFactor,
                                                                         double        diameter,
                                                                         double        dFactor,
                                                                         double        kh,
                                                                         CellDirection direction )
{
    m_completionType   = CompletionType::PERFORATION;
    m_transmissibility = transmissibility;
    m_skinFactor       = skinFactor;
    m_diameter         = diameter;
    m_dFactor          = dFactor;
    m_direction        = direction;
    m_kh               = kh;
    m_isMainBore       = true;
}

//==================================================================================================
/// Sets combined values explicit trans.
//==================================================================================================
void RigCompletionData::setCombinedValuesExplicitTrans( double         transmissibility,
                                                        double         kh,
                                                        double         dFactor,
                                                        double         skinFactor,
                                                        double         diameter,
                                                        CellDirection  celldirection,
                                                        CompletionType completionType )
{
    m_transmissibility = transmissibility;
    m_kh               = kh;
    m_dFactor          = dFactor;
    m_skinFactor       = skinFactor;
    m_diameter         = diameter;
    m_direction        = celldirection;
    m_completionType   = completionType;
}

//==================================================================================================
/// Sets combined values implicit trans wp imult.
//==================================================================================================
void RigCompletionData::setCombinedValuesImplicitTransWPImult( double         wpimult,
                                                               double         kh,
                                                               double         dFactor,
                                                               double         skinFactor,
                                                               double         diameter,
                                                               CellDirection  celldirection,
                                                               CompletionType completionType )
{
    m_wpimult        = wpimult;
    m_kh             = kh;
    m_dFactor        = dFactor;
    m_direction      = celldirection;
    m_completionType = completionType;
    m_skinFactor     = skinFactor;
    m_diameter       = diameter;
}

//--------------------------------------------------------------------------------------------------
/// Returns whether non darcy flow.
//--------------------------------------------------------------------------------------------------
bool RigCompletionData::isNonDarcyFlow() const
{
    if ( !isDefaultValue( m_kh ) ) return true;
    if ( !isDefaultValue( m_dFactor ) ) return true;

    return false;
}

//--------------------------------------------------------------------------------------------------
/// Sets d factor.
//--------------------------------------------------------------------------------------------------
void RigCompletionData::setDFactor( double dFactor )
{
    m_dFactor = dFactor;
}

//--------------------------------------------------------------------------------------------------
/// Sets per connection dfactor.
//--------------------------------------------------------------------------------------------------
void RigCompletionData::setPerConnectionDfactor()
{
    if ( isDefaultValue( m_dFactor ) ) return;

    // We use per connection dfactors, they have negative values, ref OPM Flow manual - COMPDAT kw
    m_dFactor = std::abs( m_dFactor ) * -1.0;
}

//--------------------------------------------------------------------------------------------------
/// Sets kh.
//--------------------------------------------------------------------------------------------------
void RigCompletionData::setKh( double kh )
{
    m_kh = kh;
}

//==================================================================================================
/// Adds metadata.
//==================================================================================================
void RigCompletionData::addMetadata( const QString& name, const QString& comment )
{
    m_metadata.push_back( RigCompletionMetaData( name, comment ) );
}

//==================================================================================================
/// Returns the default value.
//==================================================================================================
double RigCompletionData::defaultValue()
{
    return std::numeric_limits<double>::infinity();
}

//--------------------------------------------------------------------------------------------------
/// Returns whether default value.
//--------------------------------------------------------------------------------------------------
bool RigCompletionData::isDefaultValue( double num )
{
    return num == defaultValue();
}

//--------------------------------------------------------------------------------------------------
/// Returns the metadata.
//--------------------------------------------------------------------------------------------------
const std::vector<RigCompletionMetaData>& RigCompletionData::metadata() const
{
    return m_metadata;
}

//--------------------------------------------------------------------------------------------------
/// Returns the well name.
//--------------------------------------------------------------------------------------------------
const QString& RigCompletionData::wellName() const
{
    return m_wellName;
}

//--------------------------------------------------------------------------------------------------
/// Returns the completion data grid cell.
//--------------------------------------------------------------------------------------------------
const RigCompletionDataGridCell& RigCompletionData::completionDataGridCell() const
{
    return m_cellIndex;
}

//--------------------------------------------------------------------------------------------------
/// Returns the saturation.
//--------------------------------------------------------------------------------------------------
double RigCompletionData::saturation() const
{
    return m_saturation;
}

//--------------------------------------------------------------------------------------------------
/// Returns the transmissibility.
//--------------------------------------------------------------------------------------------------
double RigCompletionData::transmissibility() const
{
    return m_transmissibility;
}

//--------------------------------------------------------------------------------------------------
/// Returns the diameter.
//--------------------------------------------------------------------------------------------------
double RigCompletionData::diameter() const
{
    return m_diameter;
}

//--------------------------------------------------------------------------------------------------
/// Returns the kh.
//--------------------------------------------------------------------------------------------------
double RigCompletionData::kh() const
{
    return m_kh;
}

//--------------------------------------------------------------------------------------------------
/// Returns the skin factor.
//--------------------------------------------------------------------------------------------------
double RigCompletionData::skinFactor() const
{
    return m_skinFactor;
}

//--------------------------------------------------------------------------------------------------
/// Returns the d factor.
//--------------------------------------------------------------------------------------------------
double RigCompletionData::dFactor() const
{
    return m_dFactor;
}

//--------------------------------------------------------------------------------------------------
/// Returns the start md.
//--------------------------------------------------------------------------------------------------
std::optional<double> RigCompletionData::startMD() const
{
    return m_startMD;
}

//--------------------------------------------------------------------------------------------------
/// Returns the end md.
//--------------------------------------------------------------------------------------------------
std::optional<double> RigCompletionData::endMD() const
{
    return m_endMD;
}

//--------------------------------------------------------------------------------------------------
/// Returns the completion number.
//--------------------------------------------------------------------------------------------------
std::optional<int> RigCompletionData::completionNumber() const
{
    return m_completionNumber;
}

//--------------------------------------------------------------------------------------------------
/// Returns the direction string ijk.
//--------------------------------------------------------------------------------------------------
QString RigCompletionData::directionStringIJK() const
{
    switch ( m_direction )
    {
        case CellDirection::DIR_I:
            return "I";
        case CellDirection::DIR_J:
            return "J";
        case CellDirection::DIR_K:
            return "K";
        case CellDirection::DIR_UNDEF:
        default:
            return "Undefined";
    }
}

//--------------------------------------------------------------------------------------------------
/// Returns the direction string xyz.
//--------------------------------------------------------------------------------------------------
QString RigCompletionData::directionStringXYZ() const
{
    switch ( m_direction )
    {
        case CellDirection::DIR_I:
            return "X";
        case CellDirection::DIR_J:
            return "Y";
        case CellDirection::DIR_K:
        default:
            return "Z";
    }
}

//--------------------------------------------------------------------------------------------------
/// Returns the meta data string.
//--------------------------------------------------------------------------------------------------
QString RigCompletionData::metaDataString() const
{
    QStringList metadataList;
    for ( const auto& meta : m_metadata )
    {
        metadataList.append( meta.name + ": " + meta.comment );
    }
    return metadataList.join( "\n" );
}

//--------------------------------------------------------------------------------------------------
/// Returns the direction.
//--------------------------------------------------------------------------------------------------
RigCompletionData::CellDirection RigCompletionData::direction() const
{
    return m_direction;
}

//--------------------------------------------------------------------------------------------------
/// Returns the wpimult.
//--------------------------------------------------------------------------------------------------
double RigCompletionData::wpimult() const
{
    return m_wpimult;
}

//--------------------------------------------------------------------------------------------------
/// Returns the completion type.
//--------------------------------------------------------------------------------------------------
RigCompletionData::CompletionType RigCompletionData::completionType() const
{
    return m_completionType;
}

//--------------------------------------------------------------------------------------------------
/// Returns whether main bore.
//--------------------------------------------------------------------------------------------------
bool RigCompletionData::isMainBore() const
{
    return m_isMainBore;
}

//--------------------------------------------------------------------------------------------------
/// Returns the first ordering value.
//--------------------------------------------------------------------------------------------------
double RigCompletionData::firstOrderingValue() const
{
    return m_firstOrderingValue;
}

//--------------------------------------------------------------------------------------------------
/// Returns the second ordering value.
//--------------------------------------------------------------------------------------------------
double RigCompletionData::secondOrderingValue() const
{
    return m_secondOrderingValue;
}

//--------------------------------------------------------------------------------------------------
/// Sets source pdm object.
//--------------------------------------------------------------------------------------------------
void RigCompletionData::setSourcePdmObject( const caf::PdmObject* object )
{
    m_sourcePdmObject = const_cast<caf::PdmObject*>( object );
}

//--------------------------------------------------------------------------------------------------
/// Returns the source pdm object.
//--------------------------------------------------------------------------------------------------
const caf::PdmObject* RigCompletionData::sourcePdmObject() const
{
    return m_sourcePdmObject;
}

//--------------------------------------------------------------------------------------------------
/// Returns the copy.
//--------------------------------------------------------------------------------------------------
void RigCompletionData::copy( RigCompletionData& target, const RigCompletionData& from )
{
    target.m_metadata            = from.m_metadata;
    target.m_wellName            = from.m_wellName;
    target.m_cellIndex           = from.m_cellIndex;
    target.m_saturation          = from.m_saturation;
    target.m_transmissibility    = from.m_transmissibility;
    target.m_diameter            = from.m_diameter;
    target.m_kh                  = from.m_kh;
    target.m_skinFactor          = from.m_skinFactor;
    target.m_dFactor             = from.m_dFactor;
    target.m_direction           = from.m_direction;
    target.m_isMainBore          = from.m_isMainBore;
    target.m_wpimult             = from.m_wpimult;
    target.m_completionType      = from.m_completionType;
    target.m_startMD             = from.m_startMD;
    target.m_endMD               = from.m_endMD;
    target.m_firstOrderingValue  = from.m_firstOrderingValue;
    target.m_secondOrderingValue = from.m_secondOrderingValue;
    target.m_sourcePdmObject     = from.m_sourcePdmObject;
    target.m_completionNumber    = from.m_completionNumber;
}

//--------------------------------------------------------------------------------------------------
/// Sets depth range.
//--------------------------------------------------------------------------------------------------
void RigCompletionData::setDepthRange( double startMD, double endMD )
{
    m_startMD = startMD;
    m_endMD   = endMD;
}

//--------------------------------------------------------------------------------------------------
/// Sets completion number.
//--------------------------------------------------------------------------------------------------
void RigCompletionData::setCompletionNumber( int completionNumber )
{
    m_completionNumber = completionNumber;
}
