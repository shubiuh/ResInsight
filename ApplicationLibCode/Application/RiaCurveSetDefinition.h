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
/// @brief Identifies a summary ensemble curve set by ensemble and address.

#pragma once

#include "RifEclipseSummaryAddress.h"

#include <QString>

#include <utility>
#include <vector>

class RimSummaryEnsemble;

//==================================================================================================
/// @brief Binds a summary ensemble to a summary address to identify a curve set.
///
/// RiaCurveSetDefinition pairs a RimSummaryEnsemble pointer with a RifEclipseSummaryAddress so
/// that curve sets can be compared, sorted, and looked up by their ensemble and address. The
/// ensemble pointer is non-owning and participates in ordering by pointer identity; callers must
/// therefore ensure the ensemble remains alive while a definition is used.
//==================================================================================================
class RiaCurveSetDefinition
{
public:
    /// Creates an empty definition with no ensemble and a default summary address.
    RiaCurveSetDefinition();

    /// Creates a definition for one ensemble/address pair.
    /// @param ensemble Non-owning ensemble pointer used as part of the identity.
    /// @param summaryAddress Summary vector address shared by the curve set.
    explicit RiaCurveSetDefinition( RimSummaryEnsemble* ensemble, const RifEclipseSummaryAddress& summaryAddress );

    /// @return The non-owning ensemble pointer, or nullptr for an empty definition.
    RimSummaryEnsemble*             ensemble() const;

    /// @return The summary address identifying the curves within the ensemble.
    const RifEclipseSummaryAddress& summaryAddress() const;

    /// Orders first by ensemble identity and then by summary address.
    bool operator<( const RiaCurveSetDefinition& other ) const;

private:
    RimSummaryEnsemble*      m_ensemble;       ///< Non-owning ensemble participating in the key.
    RifEclipseSummaryAddress m_summaryAddress; ///< Summary vector address participating in the key.
};
