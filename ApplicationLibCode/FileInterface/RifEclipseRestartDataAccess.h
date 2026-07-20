/////////////////////////////////////////////////////////////////////////////////

/// @file
/// Declares the storage-neutral interface for Eclipse restart result access.
//
//  Copyright (C) 2011-     Statoil ASA
//  Copyright (C) 2013-     Ceetron Solutions AS
//  Copyright (C) 2011-2012 Ceetron AS
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

#include "cvfObject.h"

#include <QDateTime>
#include <QStringList>

#include <vector>

#include "ert/ecl_well/well_info.hpp"

#include "RifEclipseReportKeywords.h"

//==================================================================================================
//
/// Abstract access layer for unified and file-set Eclipse restart results.
///
/// The interface hides whether time steps reside in one unified file or several
/// restart files. Clients can enumerate time metadata and keywords, load one result,
/// import dynamic NNC fluxes, and populate the shared ERT well-information object.
//==================================================================================================
class RifEclipseRestartDataAccess : public cvf::Object
{
public:
    RifEclipseRestartDataAccess() {};
    ~RifEclipseRestartDataAccess() override {};

    /// Opens the configured restart source and prepares metadata access.
    virtual bool open()                                        = 0;
    /// Replaces the ordered restart files used by the implementation.
    virtual void setRestartFiles( const QStringList& fileSet ) = 0;
    /// Releases file handles and transient reader state.
    virtual void close()                                       = 0;

    /// Supplies externally selected time steps when supported by the backend.
    virtual void             setTimeSteps( const std::vector<QDateTime>& timeSteps ) {};
    /// @return Number of exposed restart time steps.
    virtual size_t           timeStepCount()                                                                               = 0;
    /// Appends timestamps and elapsed simulation days in matching order.
    virtual void             timeSteps( std::vector<QDateTime>* timeSteps, std::vector<double>* daysSinceSimulationStart ) = 0;
    /// @return Simulator report numbers corresponding to exposed time steps.
    virtual std::vector<int> reportNumbers()                                                                               = 0;

    /// @return Available restart keywords and their per-grid value counts.
    virtual std::vector<RifEclipseKeywordValueCount> keywordValueCounts()                                             = 0;
    /// Loads @p resultName for one time step, assembling values for @p gridCount grids.
    virtual bool results( const QString& resultName, size_t timeStep, size_t gridCount, std::vector<double>* values ) = 0;

    /// Loads phase fluxes for dynamic non-neighbor connections at one time step.
    virtual bool dynamicNNCResults( const ecl_grid_type* grid,
                                    size_t               timeStep,
                                    std::vector<double>* waterFlux,
                                    std::vector<double>* oilFlux,
                                    std::vector<double>* gasFlux ) = 0;

    /// Populates @p well_info, optionally including complete multisegment-well data.
    virtual void readWellData( well_info_type* well_info, bool importCompleteMswData ) = 0;
    /// @return Integer ERT unit-system identifier stored in the restart source.
    virtual int readUnitsType() = 0;

    /// @return Fluid phases detected in the restart results.
    virtual std::set<RiaDefines::PhaseType> availablePhases() const = 0;

    /// Allows backends to rebuild grid-dependent caches after grid discovery.
    virtual void updateFromGridCount( size_t gridCount ) {};
};
