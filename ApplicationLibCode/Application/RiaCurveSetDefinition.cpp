/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017     Statoil ASA
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
/// @brief Implements summary ensemble curve set identification.

#include "RiaCurveSetDefinition.h"

#include "RifSummaryReaderInterface.h"
#include "RimSummaryEnsemble.h"

//--------------------------------------------------------------------------------------------------
/// A null ensemble makes the default object a valid sentinel and sortable key.
//--------------------------------------------------------------------------------------------------
RiaCurveSetDefinition::RiaCurveSetDefinition()
    : m_ensemble( nullptr )
{
}

//--------------------------------------------------------------------------------------------------
/// Stores the non-owning ensemble identity together with a value copy of the address.
//--------------------------------------------------------------------------------------------------
RiaCurveSetDefinition::RiaCurveSetDefinition( RimSummaryEnsemble* ensemble, const RifEclipseSummaryAddress& summaryAddress )
    : m_ensemble( ensemble )
    , m_summaryAddress( summaryAddress )
{
}

//--------------------------------------------------------------------------------------------------
/// Returns the ensemble without transferring ownership.
//--------------------------------------------------------------------------------------------------
RimSummaryEnsemble* RiaCurveSetDefinition::ensemble() const
{
    return m_ensemble;
}

//--------------------------------------------------------------------------------------------------
/// Returning by reference avoids copying the structured summary address.
//--------------------------------------------------------------------------------------------------
const RifEclipseSummaryAddress& RiaCurveSetDefinition::summaryAddress() const
{
    return m_summaryAddress;
}

//--------------------------------------------------------------------------------------------------
/// Pointer ordering groups all addresses belonging to the same ensemble before address ordering.
//--------------------------------------------------------------------------------------------------
bool RiaCurveSetDefinition::operator<( const RiaCurveSetDefinition& other ) const
{
    if ( m_ensemble != other.ensemble() )
    {
        return ( m_ensemble < other.ensemble() );
    }

    return ( m_summaryAddress < other.summaryAddress() );
}
