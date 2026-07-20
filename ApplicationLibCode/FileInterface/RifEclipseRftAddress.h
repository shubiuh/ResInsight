/////////////////////////////////////////////////////////////////////////////////

/// @file
/// Defines the ordered key used to identify RFT and PLT result channels.
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

#pragma once

#include "RiaRftDefines.h"

#include <set>
#include <string>
#include <vector>

#include <QDateTime>

//==================================================================================================
/// Immutable-style address for one well-log channel at a well and timestamp.
///
/// Segment-result addresses additionally carry a result name, branch index, and
/// branch type. Equality and ordering make addresses suitable as keys in ordered
/// sets used throughout the reader interfaces.
//==================================================================================================
class RifEclipseRftAddress
{
public:
    /// Supported formation-test, production-log, statistical, and segment channels.
    enum class RftWellLogChannelType
    {
        NONE,
        TVD,
        PRESSURE,
        SWAT,
        SOIL,
        SGAS,
        WRAT,
        ORAT,
        GRAT,
        MD,
        PRESSURE_P10,
        PRESSURE_P50,
        PRESSURE_P90,
        PRESSURE_MEAN,
        PRESSURE_ERROR,
        SEGMENT_VALUES
    };

public:
    /// Creates a conventional channel address without segment metadata.
    static RifEclipseRftAddress createAddress( const QString& wellName, const QDateTime& timeStep, RftWellLogChannelType wellLogChannel );

    /// Creates a segment-result address not restricted to a particular branch.
    static RifEclipseRftAddress createSegmentAddress( const QString& wellName, const QDateTime& dateTime, const QString& resultName );

    /// Creates a segment-result address for one branch and branch representation.
    static RifEclipseRftAddress createBranchSegmentAddress( const QString&            wellName,
                                                            const QDateTime&          dateTime,
                                                            const QString&            resultName,
                                                            int                       segmentBranchIndex,
                                                            RiaDefines::RftBranchType segmentBranchType );

    QString                   segmentResultName() const;  ///< @return Segment keyword, or empty for conventional channels.
    int                       segmentBranchIndex() const; ///< @return Branch index, or `-1` when not branch-specific.
    RiaDefines::RftBranchType segmentBranchType() const;  ///< @return Branch representation used for segment results.

    const QString&               wellName() const;       ///< @return Well identifier.
    QDateTime                    timeStep() const;       ///< @return Result timestamp.
    const RftWellLogChannelType& wellLogChannel() const; ///< @return Addressed channel.

    /// @return Pressure and pressure-statistics channels normally shown on RFT plots.
    static std::set<RftWellLogChannelType> rftPlotChannelTypes();
    /// @return Phase-rate channels normally shown on PLT plots.
    static std::set<RftWellLogChannelType> pltPlotChannelTypes();

private:
    RifEclipseRftAddress( const QString&            wellName,
                          const QDateTime&          timeStep,
                          RftWellLogChannelType     wellLogChannel,
                          const QString&            segmentResultName,
                          int                       segmentBranchIndex,
                          RiaDefines::RftBranchType segmentBranchType );

private:
    QString               m_wellName;       ///< Well component of the address.
    QDateTime             m_timeStep;       ///< Timestamp component of the address.
    RftWellLogChannelType m_wellLogChannel; ///< Channel component of the address.

    QString                   m_segmentResultName;  ///< Keyword for segment-valued results.
    int                       m_segmentBranchIndex; ///< Optional branch discriminator.
    RiaDefines::RftBranchType m_segmentBranchType;  ///< Segment branch representation.
};

/// Compares every logical address component for equality.
bool operator==( const RifEclipseRftAddress& first, const RifEclipseRftAddress& second );

/// Provides lexicographic ordering for use in `std::set` and map keys.
bool operator<( const RifEclipseRftAddress& first, const RifEclipseRftAddress& second );
