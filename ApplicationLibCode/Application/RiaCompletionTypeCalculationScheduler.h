/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2018-     Equinor ASA
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
/// @brief Scheduler for deferred completion-type recalculation.

#pragma once

#include "cafPdmPointer.h"
#include "cafScheduler.h"

#include <vector>

class RimEclipseCase;

//--------------------------------------------------------------------------------------------------
/// @brief Singleton scheduler that defers and batches completion-type recalculations.
///
/// RiaCompletionTypeCalculationScheduler collects recalculate requests for RimEclipseCase
/// objects and processes them together in a single update pass to avoid repeated expensive
/// recomputation when multiple cases are modified in quick succession. Requests are deferred to
/// the Qt event loop through caf::Scheduler, duplicate cases are collapsed before processing, and
/// PDM-aware pointers prevent deleted cases from being dereferenced.
///
/// Clearing and recalculation are intentionally separate operations. Completion-type results and
/// their virtual-perforation inputs are invalidated immediately, while view recalculation and
/// redraw are deferred until the model has reached a consistent state.
//--------------------------------------------------------------------------------------------------
class RiaCompletionTypeCalculationScheduler : public caf::Scheduler
{
    Q_OBJECT;

public:
    /// @return The process-wide scheduler instance.
    static RiaCompletionTypeCalculationScheduler* instance();

    /// Invalidates and schedules completion-type recalculation for every Eclipse case in the project.
    void                                          scheduleRecalculateCompletionTypeAndRedrawAllViews();

    /// Clears completion-type results for every Eclipse case and invalidates affected view geometry.
    void                                          clearCompletionTypeResultsInAllCases();

    /// Invalidates and schedules completion-type recalculation for @p eclipseCases.
    /// @param eclipseCases Cases whose completion-derived results and views may have changed.
    void scheduleRecalculateCompletionTypeAndRedrawAllViews( const std::vector<RimEclipseCase*>& eclipseCases );

    /// Removes cached completion-type results and their virtual-perforation input data.
    /// @param eclipseCases Cases to invalidate; null or unloaded cases are ignored.
    void clearCompletionTypeResults( const std::vector<RimEclipseCase*>& eclipseCases );

    /// Recalculates each queued case once and redraws views that require completion-type data.
    void performScheduledUpdates() override;

private:
    /// Constructs the singleton scheduler.
    RiaCompletionTypeCalculationScheduler();

    /// Destroys the singleton after the Qt application has finished.
    ~RiaCompletionTypeCalculationScheduler() override;

    /// This process-wide scheduler is neither copyable nor assignable.
    RiaCompletionTypeCalculationScheduler( const RiaCompletionTypeCalculationScheduler& o ) = delete;
    void operator=( const RiaCompletionTypeCalculationScheduler& o )                        = delete;

private:
    /// Cases awaiting recalculation; PDM pointers become null if a queued case is deleted.
    std::vector<caf::PdmPointer<RimEclipseCase>> m_eclipseCasesToRecalculate;
};
