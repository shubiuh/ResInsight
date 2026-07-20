/////////////////////////////////////////////////////////////////////////////////

/// @file
/// Declares the common query API for RFT and PLT well-log readers.
//
//  Copyright (C) 2019- Equinor ASA
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

#include "RifEclipseRftAddress.h"

#include <QDateTime>
#include <QString>

#include <set>
#include <vector>

namespace caf
{
class VecIjk0;
};

class RigEclipseWellLogExtractor;

/// Format-neutral access to repeat formation test and production logging data.
///
/// Results are addressed by well, timestamp, and channel. Implementations supply
/// the stored samples and optional grid-cell indices; the base class provides
/// address filtering and measured-depth reconstruction against a well path.
class RifReaderRftInterface
{
public:
    virtual ~RifReaderRftInterface();

    /// Returns all channels matching @p wellName and @p timeStep.
    std::set<RifEclipseRftAddress>         eclipseRftAddresses( const QString& wellName, const QDateTime& timeStep );
    /// @return Complete set of addresses exposed by this reader.
    virtual std::set<RifEclipseRftAddress> eclipseRftAddresses()                                                         = 0;
    /// Loads samples for @p rftAddress into @p values.
    virtual void                           values( const RifEclipseRftAddress& rftAddress, std::vector<double>* values ) = 0;

    /// Returns dates where the well contains at least one of @p relevantChannels.
    virtual std::set<QDateTime>                                   availableTimeSteps( const QString&                                               wellName,
                                                                                      const std::set<RifEclipseRftAddress::RftWellLogChannelType>& relevantChannels ) = 0;
    /// Returns all dates available for @p wellName.
    virtual std::set<QDateTime>                                   availableTimeSteps( const QString& wellName ) = 0;
    /// Returns dates containing the requested channel for @p wellName.
    virtual std::set<QDateTime>                                   availableTimeSteps( const QString&                                     wellName,
                                                                                      const RifEclipseRftAddress::RftWellLogChannelType& wellLogChannelName ) = 0;
    /// Returns all channels available for @p wellName across time steps.
    virtual std::set<RifEclipseRftAddress::RftWellLogChannelType> availableWellLogChannels( const QString& wellName ) = 0;
    /// @return Wells for which the reader has RFT or PLT data.
    virtual std::set<QString>                                     wellNames()                                         = 0;

    /// Computes per-cell measured depths, estimating missing intersections from TVD.
    ///
    /// Estimated values are forced to remain increasing along the returned cell sequence.
    std::vector<double> computeMeasuredDepth( const QString& wellName, const QDateTime& timeStep, RigEclipseWellLogExtractor* extractor );

    /// Returns zero-based grid-cell coordinates associated with a well/date.
    /// The default implementation returns no cells.
    virtual std::vector<caf::VecIjk0> cellIndices( const QString& wellName, const QDateTime& timeStep );
};
