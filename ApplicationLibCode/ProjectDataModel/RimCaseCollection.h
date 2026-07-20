/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Owned collection of Eclipse cases within a group or ensemble context.
//
//  Copyright (C) 2011-2012 Statoil ASA, Ceetron AS
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

#include "cafPdmChildArrayField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"

class RimEclipseCase;
class RimReservoirGridEnsembleBase;
class RimIdenticalGridCaseGroup;
class RimReservoirGridEnsemble;
class RimCase;

/// @brief Stores Eclipse cases and exposes their enclosing grouping context.
///
/// The child array owns its cases. Ancestor queries let shared code determine whether the collection
/// belongs to an identical-grid case group, a concrete reservoir-grid ensemble, or another ensemble
/// base without encoding that hierarchy in every consumer.
class RimCaseCollection : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes the owned reservoir-case array.
    RimCaseCollection();

    /// @return Owned cases as base-class pointers in stored order.
    std::vector<RimCase*> cases() const;

    /// Owned Eclipse reservoir cases.
    caf::PdmChildArrayField<RimEclipseCase*> reservoirs;

    /// @return Enclosing identical-grid group, or `nullptr` when not grouped that way.
    RimIdenticalGridCaseGroup*    parentCaseGroup();
    /// @return Enclosing concrete grid ensemble, or `nullptr` when absent.
    RimReservoirGridEnsemble*     parentGridEnsemble();
    /// @return Enclosing grid-ensemble base of any supported subtype, or `nullptr`.
    RimReservoirGridEnsembleBase* parentGridEnsembleBase();
};
