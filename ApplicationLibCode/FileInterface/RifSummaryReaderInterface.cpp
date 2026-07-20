/////////////////////////////////////////////////////////////////////////////////

/// @file
/// Implements lazy summary-address discovery and reader-instance identity.
//
//  Copyright (C) 2017- Statoil ASA
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

#include "RifSummaryReaderInterface.h"

#include <string>

#include <QDateTime>

int RifSummaryReaderInterface::m_nextSerialNumber = 0;

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const std::set<RifEclipseSummaryAddress>& RifSummaryReaderInterface::allResultAddresses() const
{
    return m_allResultAddresses;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
const std::set<RifEclipseSummaryAddress>& RifSummaryReaderInterface::allErrorAddresses() const
{
    return m_allErrorAddresses;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RifEclipseSummaryAddress RifSummaryReaderInterface::errorAddress( const RifEclipseSummaryAddress& resultAddress ) const
{
    RifEclipseSummaryAddress errAddr = resultAddress;
    errAddr.setAsErrorResult();

    return m_allErrorAddresses.find( errAddr ) != m_allErrorAddresses.end() ? errAddr : RifEclipseSummaryAddress();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RifSummaryReaderInterface::createAndSetAddresses()
{
}

//--------------------------------------------------------------------------------------------------
/// Delays potentially expensive format-specific address enumeration until a client
/// actually needs the canonical address set.
//--------------------------------------------------------------------------------------------------
void RifSummaryReaderInterface::createAddressesIfRequired()
{
    if ( m_allResultAddresses.empty() )
    {
        createAndSetAddresses();
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
int RifSummaryReaderInterface::serialNumber() const
{
    return m_serialNumber;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
size_t RifSummaryReaderInterface::dataObjectCount() const
{
    if ( !m_allResultAddresses.empty() ) return m_allResultAddresses.size();

    // Fallback to keyword count if no addresses are available. This can happen if an ensemble creates addresses for only a single realization.
    return keywordCount();
}

//--------------------------------------------------------------------------------------------------
/// Serial assignment is protected because readers can be created concurrently
/// while ensembles are imported in parallel.
//--------------------------------------------------------------------------------------------------
void RifSummaryReaderInterface::increaseSerialNumber()
{
#pragma omp critical
    m_serialNumber = m_nextSerialNumber++;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RifSummaryReaderInterface::RifSummaryReaderInterface()
{
    increaseSerialNumber();
}

//--------------------------------------------------------------------------------------------------
/// Prefers the discovered address set for constant-time lookup. The value-query
/// fallback preserves compatibility with readers that have not built addresses yet,
/// but is intentionally unsuitable for performance-critical loops.
//--------------------------------------------------------------------------------------------------
bool RifSummaryReaderInterface::hasAddress( const RifEclipseSummaryAddress& resultAddress ) const
{
    static const RifEclipseSummaryAddress defaultAdr = RifEclipseSummaryAddress();
    if ( resultAddress == defaultAdr ) return true;

    if ( !m_allResultAddresses.empty() )
    {
        return ( m_allResultAddresses.count( resultAddress ) > 0 );
    }

    // This is a fallback for cases where no addresses are present, such as when the summary reader is not yet initialized.
    // This function should not be used in performance-critical code paths.
    const auto& [isOk, vals] = values( resultAddress );
    return isOk && !vals.empty();
}
