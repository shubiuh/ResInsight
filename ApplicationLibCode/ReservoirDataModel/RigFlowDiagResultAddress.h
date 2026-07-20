/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2016-     Statoil ASA
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
/// @brief Declares flow diag result address reservoir-data functionality.
#pragma once

#include "RigFlowDiagDefines.h"

#include "cafAppEnum.h"

#include <set>
#include <string>

/// @brief Models flow diag result address for reservoir-data processing.
class RigFlowDiagResultAddress
{
public:
    /// Enumerates the supported phase selection values.
    enum PhaseSelection
    {
        PHASE_ALL = 0b111,
        PHASE_OIL = 0b001,
        PHASE_GAS = 0b010,
        PHASE_WAT = 0b100,
    };

    /// Type alias used for phase selection enum.
    using PhaseSelectionEnum = caf::AppEnum<PhaseSelection>;

    /// Returns or processes flow diag result address.
    RigFlowDiagResultAddress( const std::string& aVariableName, PhaseSelection phaseSelection, const std::set<std::string>& someSelectedTracerNames )
        : variableName( aVariableName )
        , selectedTracerNames( someSelectedTracerNames )
        , phaseSelection( phaseSelection )
    {
    }

    /// Returns or processes flow diag result address.
    RigFlowDiagResultAddress( const std::string& aVariableName, PhaseSelection phaseSelection, const std::string& tracerName )
        : variableName( aVariableName )
        , phaseSelection( phaseSelection )
    {
        selectedTracerNames.insert( tracerName );
    }

    bool isNativeResult() const;

    std::string uiText() const;
    std::string uiShortText() const;

    /// Stores variable name.
    std::string           variableName;
    /// Stores selected tracer names.
    std::set<std::string> selectedTracerNames;
    /// Stores phase selection.
    PhaseSelection        phaseSelection;

    /// Implements the operator< operation.
    bool operator<( const RigFlowDiagResultAddress& other ) const
    {
        if ( selectedTracerNames != other.selectedTracerNames )
        {
            return selectedTracerNames < other.selectedTracerNames;
        }
        if ( phaseSelection != other.phaseSelection )
        {
            return phaseSelection < other.phaseSelection;
        }

        return variableName < other.variableName;
    }
};
