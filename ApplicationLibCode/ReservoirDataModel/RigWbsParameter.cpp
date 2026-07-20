/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2019- Ceetron Solutions AS
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
/// @brief Implements wbs parameter reservoir-data functionality.
#include "RigWbsParameter.h"

#include "RiaResultNames.h"
#include "RiaWellLogUnitTools.h"
#include "RigFemAddressDefines.h"

#include "cafAssert.h"

namespace caf
{
template <>
void RigWbsParameter::SourceEnum::setUp()
{
    addItem( RigWbsParameter::GRID, "GRID", "Grid" );
    addItem( RigWbsParameter::LAS_FILE, "LAS_FILE", "LAS File" );
    addItem( RigWbsParameter::ELEMENT_PROPERTY_TABLE, "ELEMENT_PROPERTY_TABLE", "Property Table" );
    addItem( RigWbsParameter::USER_DEFINED, "USER_DEFINED", "User Defined" );
    addItem( RigWbsParameter::HYDROSTATIC, "HYDROSTATIC", "Hydrostatic" );
    addItem( RigWbsParameter::DERIVED_FROM_K0FG, "DERIVED_FROM_K0FG", "FG derived from K0_FG" );
    addItem( RigWbsParameter::PROPORTIONAL_TO_SH, "PROPORTIONAL_TO_SH", "Proportional to SH" );
    addItem( RigWbsParameter::UNDEFINED, "UNDEFINED", "Undefined" );
    setDefault( RigWbsParameter::UNDEFINED );
}
} // End namespace caf

//--------------------------------------------------------------------------------------------------
/// Creates a RigWbsParameter instance.
//--------------------------------------------------------------------------------------------------
RigWbsParameter::RigWbsParameter( const QString& name, bool normalizeByHydroStaticPP, const SourceVector& sources, bool exclusiveOption )
    : m_name( name )
    , m_sources( sources )
    , m_normalizeByHydroStaticPP( normalizeByHydroStaticPP )
    , m_exclusiveOptions( false )
{
}

//--------------------------------------------------------------------------------------------------
/// Creates a RigWbsParameter instance.
//--------------------------------------------------------------------------------------------------
RigWbsParameter::RigWbsParameter( const RigWbsParameter& rhs )
    : m_name( rhs.m_name )
    , m_sources( rhs.m_sources )
    , m_normalizeByHydroStaticPP( rhs.m_normalizeByHydroStaticPP )
    , m_exclusiveOptions( rhs.m_exclusiveOptions )
{
}

//--------------------------------------------------------------------------------------------------
/// Implements the operator= operation.
//--------------------------------------------------------------------------------------------------
RigWbsParameter& RigWbsParameter::operator=( const RigWbsParameter& rhs )
{
    m_name                     = rhs.m_name;
    m_sources                  = rhs.m_sources;
    m_normalizeByHydroStaticPP = rhs.m_normalizeByHydroStaticPP;
    m_exclusiveOptions         = rhs.m_exclusiveOptions;
    return *this;
}

//--------------------------------------------------------------------------------------------------
/// Returns the name.
//--------------------------------------------------------------------------------------------------
const QString& RigWbsParameter::name() const
{
    return m_name;
}

//--------------------------------------------------------------------------------------------------
/// Returns the sources.
//--------------------------------------------------------------------------------------------------
std::vector<RigWbsParameter::Source> RigWbsParameter::sources() const
{
    std::vector<RigWbsParameter::Source> allValidSources;
    for ( auto sourceEntryPair : m_sources )
    {
        allValidSources.push_back( sourceEntryPair.first );
    }
    return allValidSources;
}

//--------------------------------------------------------------------------------------------------
/// Returns whether external source is available.
//--------------------------------------------------------------------------------------------------
bool RigWbsParameter::hasExternalSource() const
{
    for ( auto sourcePair : m_sources )
    {
        if ( sourcePair.first != GRID ) return true;
    }
    return false;
}

//--------------------------------------------------------------------------------------------------
/// Adds address string.
//--------------------------------------------------------------------------------------------------
QString RigWbsParameter::addressString( Source source ) const
{
    SourceAddress sourceAddress;
    if ( address( source, &sourceAddress ) )
    {
        return sourceAddress.primary;
    }
    return "";
}

//--------------------------------------------------------------------------------------------------
/// Returns the units.
//--------------------------------------------------------------------------------------------------
QString RigWbsParameter::units( Source source ) const
{
    SourceAddress sourceAddress;
    if ( address( source, &sourceAddress ) )
    {
        return sourceAddress.units;
    }
    // Try the units of the first available source
    if ( !m_sources.empty() )
    {
        return m_sources.front().second.units;
    }
    return RiaWellLogUnitTools<double>::noUnitString();
}

//--------------------------------------------------------------------------------------------------
/// Returns the fem address.
//--------------------------------------------------------------------------------------------------
RigFemResultAddress RigWbsParameter::femAddress( Source source ) const
{
    RigFemResultPosEnum posType = RIG_WELLPATH_DERIVED;
    if ( source == GRID )
        posType = RIG_ELEMENT_NODAL;
    else if ( source == ELEMENT_PROPERTY_TABLE )
        posType = RIG_ELEMENT;

    SourceAddress addr;
    if ( address( source, &addr ) )
    {
        return RigFemResultAddress( posType, addr.primary.toStdString(), addr.secondary.toStdString() );
    }
    return RigFemResultAddress();
}

//--------------------------------------------------------------------------------------------------
/// Returns the normalize by hydrostatic pp.
//--------------------------------------------------------------------------------------------------
bool RigWbsParameter::normalizeByHydrostaticPP() const
{
    return m_normalizeByHydroStaticPP;
}

//--------------------------------------------------------------------------------------------------
/// Returns the exclusive options.
//--------------------------------------------------------------------------------------------------
bool RigWbsParameter::exclusiveOptions() const
{
    return m_exclusiveOptions;
}

//--------------------------------------------------------------------------------------------------
/// Set options to be exclusive rather than an order of preference
//--------------------------------------------------------------------------------------------------
void RigWbsParameter::setOptionsExclusive( bool exclusive )
{
    m_exclusiveOptions = exclusive;
}

//--------------------------------------------------------------------------------------------------
/// Returns the all source ui labels.
//--------------------------------------------------------------------------------------------------
std::vector<QString> RigWbsParameter::allSourceUiLabels( const QString& delimiter /*= " "*/,
                                                         double         userDefinedValue /*= std::numeric_limits<double>::infinity() */ )
{
    std::vector<QString> allLabels;
    for ( size_t i = 0; i < SourceEnum::size(); ++i )
    {
        allLabels.push_back( sourceUiLabel( SourceEnum::fromIndex( i ), delimiter, userDefinedValue ) );
    }
    return allLabels;
}

//--------------------------------------------------------------------------------------------------
/// Returns the source ui label.
//--------------------------------------------------------------------------------------------------
QString RigWbsParameter::sourceUiLabel( Source         currentSource,
                                        const QString& delimiter,
                                        double         userDefinedValue /*= std::numeric_limits<double>::infinity() */ )
{
    QString sourceLabel;
    if ( currentSource == USER_DEFINED )
    {
        sourceLabel = QString( "%1 = %2" ).arg( m_name ).arg( userDefinedValue );
    }
    else
    {
        sourceLabel = QString( "%1 = %2" ).arg( m_name ).arg( SourceEnum::uiText( currentSource ) );
    }
    sourceLabel.replace( " ", delimiter );
    return sourceLabel;
}

//--------------------------------------------------------------------------------------------------
/// Implements the operator== operation.
//--------------------------------------------------------------------------------------------------
bool RigWbsParameter::operator==( const RigWbsParameter& rhs ) const
{
    return m_name == rhs.m_name;
}

//--------------------------------------------------------------------------------------------------
/// Implements the operator< operation.
//--------------------------------------------------------------------------------------------------
bool RigWbsParameter::operator<( const RigWbsParameter& rhs ) const
{
    return m_name < rhs.m_name;
}

//--------------------------------------------------------------------------------------------------
/// Returns the pp reservoir.
//--------------------------------------------------------------------------------------------------
RigWbsParameter RigWbsParameter::PP_Reservoir()
{
    SourceVector sources =
        { { GRID, SourceAddress( QString::fromStdString( RigFemAddressDefines::porBar() ), "", RiaWellLogUnitTools<double>::barUnitString() ) },
          { LAS_FILE, SourceAddress( "PP_INP", "", RiaWellLogUnitTools<double>::sg_emwUnitString() ) },
          { LAS_FILE, SourceAddress( "PP_RES_INP", "", RiaWellLogUnitTools<double>::sg_emwUnitString() ) },
          { LAS_FILE, SourceAddress( "POR_RES_INP", "", RiaWellLogUnitTools<double>::gPerCm3UnitString() ) },
          { ELEMENT_PROPERTY_TABLE, SourceAddress( "POR_INP", "", RiaWellLogUnitTools<double>::pascalUnitString() ) },
          { ELEMENT_PROPERTY_TABLE, SourceAddress( "PP_INP", "", RiaWellLogUnitTools<double>::sg_emwUnitString() ) } };
    return RigWbsParameter( "PP_Reservoir", true, sources );
}

//--------------------------------------------------------------------------------------------------
/// Returns the pp non reservoir.
//--------------------------------------------------------------------------------------------------
RigWbsParameter RigWbsParameter::PP_NonReservoir()
{
    SourceVector sources = { { LAS_FILE, SourceAddress( "PP_NONRES_INP", "", RiaWellLogUnitTools<double>::sg_emwUnitString() ) },
                             { LAS_FILE, SourceAddress( "POR_NONRES_INP", "", RiaWellLogUnitTools<double>::gPerCm3UnitString() ) },
                             { ELEMENT_PROPERTY_TABLE, SourceAddress( "POR_NONRES_INP", "", RiaWellLogUnitTools<double>::pascalUnitString() ) },
                             { ELEMENT_PROPERTY_TABLE, SourceAddress( "PP_NONRES_INP", "", RiaWellLogUnitTools<double>::sg_emwUnitString() ) },
                             { HYDROSTATIC, SourceAddress( "Hydrostatic", "", RiaWellLogUnitTools<double>::barUnitString() ) } };

    return RigWbsParameter( "PP_Non-Reservoir", true, sources );
}

//--------------------------------------------------------------------------------------------------
/// Returns the pp min.
//--------------------------------------------------------------------------------------------------
RigWbsParameter RigWbsParameter::PP_Min()
{
    SourceVector sources = { { LAS_FILE, SourceAddress( RiaResultNames::wbsPPMinResult(), "", RiaWellLogUnitTools<double>::barUnitString() ) } };

    return RigWbsParameter( "PP_MIN", true, sources );
}

//--------------------------------------------------------------------------------------------------
/// Returns the pp max.
//--------------------------------------------------------------------------------------------------
RigWbsParameter RigWbsParameter::PP_Max()
{
    SourceVector sources = { { LAS_FILE, SourceAddress( RiaResultNames::wbsPPMaxResult(), "", RiaWellLogUnitTools<double>::barUnitString() ) } };

    return RigWbsParameter( "PP_MAX", true, sources );
}

//--------------------------------------------------------------------------------------------------
/// Returns the pp exp.
//--------------------------------------------------------------------------------------------------
RigWbsParameter RigWbsParameter::PP_Exp()
{
    SourceVector sources = { { LAS_FILE, SourceAddress( RiaResultNames::wbsPPExpResult(), "", RiaWellLogUnitTools<double>::barUnitString() ) } };

    return RigWbsParameter( "PP_EXP", true, sources );
}

//--------------------------------------------------------------------------------------------------
/// Returns the pp initial.
//--------------------------------------------------------------------------------------------------
RigWbsParameter RigWbsParameter::PP_Initial()
{
    SourceVector sources = {
        { LAS_FILE, SourceAddress( RiaResultNames::wbsPPInitialResult(), "", RiaWellLogUnitTools<double>::barUnitString() ) } };

    return RigWbsParameter( "PP_INIT", true, sources );
}

//--------------------------------------------------------------------------------------------------
/// Returns the poisson ratio.
//--------------------------------------------------------------------------------------------------
RigWbsParameter RigWbsParameter::poissonRatio()
{
    return RigWbsParameter( "Poisson_Ratio",
                            false,
                            { { LAS_FILE, SourceAddress( "POISSON_RATIO_INP" ) },
                              { ELEMENT_PROPERTY_TABLE, SourceAddress( "POISSON_RATIO_INP" ) },
                              { USER_DEFINED, SourceAddress() } } );
}

//--------------------------------------------------------------------------------------------------
/// Returns the ucs.
//--------------------------------------------------------------------------------------------------
RigWbsParameter RigWbsParameter::UCS()
{
    return RigWbsParameter( "UCS",
                            false,
                            { { LAS_FILE, SourceAddress( "UCS_INP", "", RiaWellLogUnitTools<double>::MPaUnitString() ) },
                              { ELEMENT_PROPERTY_TABLE, SourceAddress( "UCS_INP", "", RiaWellLogUnitTools<double>::pascalUnitString() ) },
                              { USER_DEFINED, SourceAddress( "", "", RiaWellLogUnitTools<double>::barUnitString() ) } } );
}

//--------------------------------------------------------------------------------------------------
/// Returns the obg.
//--------------------------------------------------------------------------------------------------
RigWbsParameter RigWbsParameter::OBG()
{
    std::vector<std::pair<Source, SourceAddress>> sources =
        { { GRID, SourceAddress( "ST", "S33", RiaWellLogUnitTools<double>::barUnitString() ) },
          { LAS_FILE, SourceAddress( "OBG_INP", "", RiaWellLogUnitTools<double>::barUnitString() ) } };
    return RigWbsParameter( "OBG_Input", true, sources );
}

//--------------------------------------------------------------------------------------------------
/// Returns the obg0.
//--------------------------------------------------------------------------------------------------
RigWbsParameter RigWbsParameter::OBG0()
{
    std::vector<std::pair<Source, SourceAddress>> sources =
        { { GRID, SourceAddress( "ST", "S33", RiaWellLogUnitTools<double>::barUnitString() ) },
          { LAS_FILE, SourceAddress( "OBG0_INP", "", RiaWellLogUnitTools<double>::barUnitString() ) } };
    return RigWbsParameter( "OBG0", true, sources );
}

//--------------------------------------------------------------------------------------------------
/// Returns the sh.
//--------------------------------------------------------------------------------------------------
RigWbsParameter RigWbsParameter::SH()
{
    std::vector<std::pair<Source, SourceAddress>> sources = {
        { GRID, SourceAddress( "ST", "S3", RiaWellLogUnitTools<double>::barUnitString() ) } };
    return RigWbsParameter( "SH_Input", true, sources );
}

//--------------------------------------------------------------------------------------------------
/// Returns the df.
//--------------------------------------------------------------------------------------------------
RigWbsParameter RigWbsParameter::DF()
{
    return RigWbsParameter( "DF",
                            false,
                            { { LAS_FILE, SourceAddress( "DF_INP" ) },
                              { ELEMENT_PROPERTY_TABLE, SourceAddress( "DF_INP" ) },
                              { USER_DEFINED, SourceAddress() } } );
}

//--------------------------------------------------------------------------------------------------
/// Returns the k0 fg.
//--------------------------------------------------------------------------------------------------
RigWbsParameter RigWbsParameter::K0_FG()
{
    return RigWbsParameter( "K0_FG",
                            false,
                            { { LAS_FILE, SourceAddress( "K0_FG_INP", "", RiaWellLogUnitTools<double>::sg_emwUnitString() ) },
                              { ELEMENT_PROPERTY_TABLE, SourceAddress( "K0_FG_INP" ) },
                              { USER_DEFINED, SourceAddress() } } );
}

//--------------------------------------------------------------------------------------------------
/// Returns the k0 sh.
//--------------------------------------------------------------------------------------------------
RigWbsParameter RigWbsParameter::K0_SH()
{
    return RigWbsParameter( "K0_SH",
                            false,
                            { { LAS_FILE, SourceAddress( "K0_SH_INP", "", RiaWellLogUnitTools<double>::sg_emwUnitString() ) },
                              { ELEMENT_PROPERTY_TABLE, SourceAddress( "K0_SH_INP" ) },
                              { USER_DEFINED, SourceAddress() } } );
}

//--------------------------------------------------------------------------------------------------
/// Returns the fg shale.
//--------------------------------------------------------------------------------------------------
RigWbsParameter RigWbsParameter::FG_Shale()
{
    RigWbsParameter param( "FG_Shale",
                           false,
                           { { DERIVED_FROM_K0FG, SourceAddress() },
                             { PROPORTIONAL_TO_SH, SourceAddress() },
                             { LAS_FILE, SourceAddress( "FG_SHALE_INP", "", RiaWellLogUnitTools<double>::sg_emwUnitString() ) } } );
    param.setOptionsExclusive( true );
    return param;
}

//--------------------------------------------------------------------------------------------------
/// Returns the fg mk min.
//--------------------------------------------------------------------------------------------------
RigWbsParameter RigWbsParameter::FG_MkMin()
{
    RigWbsParameter param( "FG_MK_MIN", false, { { DERIVED_FROM_K0FG, SourceAddress() }, { PROPORTIONAL_TO_SH, SourceAddress() } } );
    param.setOptionsExclusive( true );
    return param;
}

//--------------------------------------------------------------------------------------------------
/// Returns the fg mk exp.
//--------------------------------------------------------------------------------------------------
RigWbsParameter RigWbsParameter::FG_MkExp()
{
    RigWbsParameter param( "FG_MK_EXP", false, { { DERIVED_FROM_K0FG, SourceAddress() }, { PROPORTIONAL_TO_SH, SourceAddress() } } );
    param.setOptionsExclusive( true );
    return param;
}

//--------------------------------------------------------------------------------------------------
/// Returns the water density.
//--------------------------------------------------------------------------------------------------
RigWbsParameter RigWbsParameter::waterDensity()
{
    RigWbsParameter param( "WaterDensity",
                           false,
                           { { USER_DEFINED, SourceAddress() },
                             { LAS_FILE, SourceAddress( "RHO_INP", "", RiaWellLogUnitTools<double>::gPerCm3UnitString() ) } } );
    return param;
}

//--------------------------------------------------------------------------------------------------
/// Returns the all parameters.
//--------------------------------------------------------------------------------------------------
std::set<RigWbsParameter> RigWbsParameter::allParameters()
{
    return { PP_Reservoir(),
             PP_NonReservoir(),
             PP_Min(),
             PP_Max(),
             PP_Exp(),
             PP_Initial(),
             poissonRatio(),
             UCS(),
             OBG(),
             OBG0(),
             SH(),
             DF(),
             K0_FG(),
             K0_SH(),
             FG_Shale(),
             FG_MkMin(),
             FG_MkExp(),
             waterDensity() };
}

//--------------------------------------------------------------------------------------------------
/// Finds parameter.
//--------------------------------------------------------------------------------------------------
bool RigWbsParameter::findParameter( QString parameterName, RigWbsParameter* foundParam /* = nullptr*/ )
{
    auto params = allParameters();
    auto it     = params.find( parameterName );
    if ( it != params.end() )
    {
        if ( foundParam ) *foundParam = *it;
        return true;
    }
    return false;
}

//--------------------------------------------------------------------------------------------------
/// Adds address.
//--------------------------------------------------------------------------------------------------
bool RigWbsParameter::address( Source source, RigWbsParameter::SourceAddress* sourceAddress ) const
{
    for ( auto sourceEntryPair : m_sources )
    {
        if ( sourceEntryPair.first == source )
        {
            *sourceAddress = sourceEntryPair.second;
            return true;
        }
    }
    return false;
}
