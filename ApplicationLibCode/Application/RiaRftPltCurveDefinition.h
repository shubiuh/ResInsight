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
/// @brief Identifies an RFT/PLT curve by data source, well name, and time step.

#pragma once

#include "RifDataSourceForRftPlt.h"

#include <QString>

#include <utility>
#include <vector>

class RimSummaryCase;

//==================================================================================================
/// @brief Binds a data source address, well name, and time step to identify an RFT or PLT curve.
///
/// RiaRftPltCurveDefinition uniquely identifies a curve by its RifDataSourceForRftPlt (which
/// describes whether the data comes from an ensemble case, a grid case, or a file), the well
/// name, and the reporting time step. Comparison uses the three-way spaceship operator.
/// Ensemble-backed sources deliberately sort before single-case sources so ensemble curves are
/// constructed and plotted first. Within the same source, definitions sort by well and time step.
//==================================================================================================
class RiaRftPltCurveDefinition
{
public:
    /// Creates an immutable curve identity.
    /// @param address RFT/PLT data source and source-specific address.
    /// @param wellName Well represented by the curve.
    /// @param timeStep Report time represented by the curve.
    explicit RiaRftPltCurveDefinition( const RifDataSourceForRftPlt& address, const QString& wellName, const QDateTime& timeStep );

    /// @return The curve data-source address.
    const RifDataSourceForRftPlt& address() const;

    /// @return The well name used by the curve.
    const QString&                wellName() const;

    /// @return The curve report time.
    const QDateTime&              timeStep() const;

    /// Provides a stable ordering suitable for ordered containers and deterministic curve creation.
    auto operator<=>( const RiaRftPltCurveDefinition& rhs ) const -> std::strong_ordering;

private:
    RifDataSourceForRftPlt m_curveAddress; ///< Data source and source-specific address.
    QString                m_wellName;     ///< Well represented by the curve.
    QDateTime              m_timeStep;     ///< Report time represented by the curve.
};
