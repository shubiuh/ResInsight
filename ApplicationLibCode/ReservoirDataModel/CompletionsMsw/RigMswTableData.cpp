/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2025 Equinor ASA
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
/// @brief Implements storage and handling of msw table data.

#include "RigMswTableData.h"

#include <algorithm>

//--------------------------------------------------------------------------------------------------
/// Creates a RigMswTableData instance.
//--------------------------------------------------------------------------------------------------
RigMswTableData::RigMswTableData( const std::string& wellName, RiaDefines::EclipseUnitSystem unitSystem )
    : m_wellName( wellName )
    , m_unitSystem( unitSystem )
{
}

//--------------------------------------------------------------------------------------------------
/// Sets welsegs header.
//--------------------------------------------------------------------------------------------------
void RigMswTableData::setWelsegsHeader( const WelsegsHeader& header )
{
    m_welsegsHeader = header;
}

//--------------------------------------------------------------------------------------------------
/// Adds welsegs row.
//--------------------------------------------------------------------------------------------------
void RigMswTableData::addWelsegsRow( const WelsegsRow& row )
{
    m_welsegsData.push_back( row );
}

//--------------------------------------------------------------------------------------------------
/// Adds compsegs row.
//--------------------------------------------------------------------------------------------------
void RigMswTableData::addCompsegsRow( const CompsegsRow& row )
{
    m_compsegsData.push_back( row );
}

//--------------------------------------------------------------------------------------------------
/// Adds wsegvalv row.
//--------------------------------------------------------------------------------------------------
void RigMswTableData::addWsegvalvRow( const WsegvalvRow& row )
{
    m_wsegvalvData.push_back( row );
}

//--------------------------------------------------------------------------------------------------
/// Adds wsegaicd row.
//--------------------------------------------------------------------------------------------------
void RigMswTableData::addWsegaicdRow( const WsegaicdRow& row )
{
    m_wsegaicdData.push_back( row );
}

//--------------------------------------------------------------------------------------------------
/// Adds wsegsicd row.
//--------------------------------------------------------------------------------------------------
void RigMswTableData::addWsegsicdRow( const WsegsicdRow& row )
{
    m_wsegsicdData.push_back( row );
}

//--------------------------------------------------------------------------------------------------
/// Adds msw branch.
//--------------------------------------------------------------------------------------------------
void RigMswTableData::addMswBranch( RigMswBranch branch )
{
    m_mswBranches.push_back( std::move( branch ) );
}

//--------------------------------------------------------------------------------------------------
/// Returns the msw branches.
//--------------------------------------------------------------------------------------------------
const std::vector<RigMswBranch>& RigMswTableData::mswBranches() const
{
    return m_mswBranches;
}

//--------------------------------------------------------------------------------------------------
/// Returns whether lgr data is available.
//--------------------------------------------------------------------------------------------------
bool RigMswTableData::hasLgrData() const
{
    return std::any_of( m_compsegsData.begin(), m_compsegsData.end(), []( const CompsegsRow& row ) { return row.isLgrGrid(); } );
}

//--------------------------------------------------------------------------------------------------
/// Returns whether empty.
//--------------------------------------------------------------------------------------------------
bool RigMswTableData::isEmpty() const
{
    return m_welsegsData.empty() && m_compsegsData.empty() && m_wsegvalvData.empty() && m_wsegaicdData.empty();
}

//--------------------------------------------------------------------------------------------------
/// Returns the main grid compsegs data.
//--------------------------------------------------------------------------------------------------
std::vector<CompsegsRow> RigMswTableData::mainGridCompsegsData() const
{
    std::vector<CompsegsRow> mainGridRows;
    std::copy_if( m_compsegsData.begin(),
                  m_compsegsData.end(),
                  std::back_inserter( mainGridRows ),
                  []( const CompsegsRow& row ) { return row.isMainGrid(); } );
    return mainGridRows;
}

//--------------------------------------------------------------------------------------------------
/// Returns the lgr compsegs data.
//--------------------------------------------------------------------------------------------------
std::vector<CompsegsRow> RigMswTableData::lgrCompsegsData() const
{
    std::vector<CompsegsRow> lgrRows;
    std::copy_if( m_compsegsData.begin(),
                  m_compsegsData.end(),
                  std::back_inserter( lgrRows ),
                  []( const CompsegsRow& row ) { return row.isLgrGrid(); } );
    return lgrRows;
}

//--------------------------------------------------------------------------------------------------
/// Returns whether valid.
//--------------------------------------------------------------------------------------------------
bool RigMswTableData::isValid() const
{
    return validationErrors().empty();
}

//--------------------------------------------------------------------------------------------------
/// Returns the validation errors.
//--------------------------------------------------------------------------------------------------
std::vector<QString> RigMswTableData::validationErrors() const
{
    std::vector<QString> errors;

    if ( m_wellName.empty() )
    {
        errors.push_back( "Well name is empty" );
    }

    if ( m_welsegsData.empty() )
    {
        errors.push_back( "No WELSEGS data" );
    }

    // Check for duplicate segment numbers in WELSEGS
    std::vector<int> segmentNumbers;
    for ( const auto& row : m_welsegsData )
    {
        segmentNumbers.push_back( row.segment1 );
    }
    std::sort( segmentNumbers.begin(), segmentNumbers.end() );
    auto it = std::adjacent_find( segmentNumbers.begin(), segmentNumbers.end() );
    if ( it != segmentNumbers.end() )
    {
        errors.push_back( QString( "Duplicate segment number: %1" ).arg( *it ) );
    }

    return errors;
}
