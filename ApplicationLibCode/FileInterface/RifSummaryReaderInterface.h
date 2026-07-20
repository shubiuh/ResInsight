/////////////////////////////////////////////////////////////////////////////////

/// @file
/// Declares the format-neutral interface used to query summary time series.
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

#pragma once

#include "RiaDefines.h"
#include "RifEclipseSummaryAddress.h"

#include <map>
#include <set>
#include <string>
#include <vector>

class QDateTime;

//==================================================================================================
/// Common read interface for simulator summary vectors.
///
/// Implementations expose results through canonical RifEclipseSummaryAddress
/// keys regardless of their underlying storage format. Address discovery is lazy,
/// while a monotonically assigned serial number lets clients distinguish reader
/// instances when caching data derived from them.
//==================================================================================================
class RifSummaryReaderInterface
{
public:
    /// Assigns a process-local serial number to the reader instance.
    RifSummaryReaderInterface();
    virtual ~RifSummaryReaderInterface() = default;

    /// Returns whether @p resultAddress is available, with a slow value-query fallback before address discovery.
    bool hasAddress( const RifEclipseSummaryAddress& resultAddress ) const;

    /// @return Discovered result addresses, including error vectors.
    const std::set<RifEclipseSummaryAddress>& allResultAddresses() const;
    /// @return Discovered addresses representing uncertainty or error vectors.
    const std::set<RifEclipseSummaryAddress>& allErrorAddresses() const;

    /// Returns the matching error address, or an invalid address if none exists.
    RifEclipseSummaryAddress errorAddress( const RifEclipseSummaryAddress& resultAddress ) const;

    /// Returns timestamps for @p resultAddress in chronological storage order.
    virtual std::vector<time_t> timeSteps( const RifEclipseSummaryAddress& resultAddress ) const = 0;

    /// Returns success and values for @p resultAddress.
    virtual std::pair<bool, std::vector<double>> values( const RifEclipseSummaryAddress& resultAddress ) const = 0;

    /// @return Unit label associated with @p resultAddress.
    virtual std::string                   unitName( const RifEclipseSummaryAddress& resultAddress ) const = 0;
    /// @return Unit convention used by the summary source.
    virtual RiaDefines::EclipseUnitSystem unitSystem() const                                              = 0;

    /// Discovers canonical addresses; implementations override this hook.
    virtual void createAndSetAddresses();
    /// Invokes address discovery only when the address set is empty.
    void         createAddressesIfRequired();

    /// @return Process-local identity used to invalidate reader-dependent caches.
    int serialNumber() const;

    /// Returns discovered address count, falling back to raw keyword count before discovery.
    size_t dataObjectCount() const;

    // The keywordCount is considered a internal method, not to be used by clients. Having it as a protected method will not work well, as
    // this class is also used as a contained object in addition to deriving from this interface.
    /// @return Raw number of vectors known by the underlying format.
    virtual size_t keywordCount() const = 0;

protected:
    /// Assigns the next reader serial number inside an OpenMP critical section.
    void increaseSerialNumber();

    std::set<RifEclipseSummaryAddress> m_allResultAddresses; ///< Result and error addresses exposed by the reader.
    std::set<RifEclipseSummaryAddress> m_allErrorAddresses;  ///< Subset containing error/uncertainty addresses.

private:
    static int m_nextSerialNumber; ///< Next process-local reader identity.
    int        m_serialNumber;     ///< Identity assigned to this reader.
};
