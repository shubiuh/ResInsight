/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2022- Equinor ASA
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
/// @brief Scheduler for deferred plot-collection data updates.

#pragma once

#include "cafScheduler.h"

class RimPlotCollection;

//--------------------------------------------------------------------------------------------------
/// @brief Singleton scheduler that defers and batches plot-collection data updates.
///
/// RiaPlotCollectionScheduler collects update requests for RimPlotCollection objects and
/// processes them together to avoid redundant work when multiple collections are invalidated
/// in quick succession. Scheduling uses a zero-delay timer inherited from caf::Scheduler, so
/// callers can finish a sequence of model changes before plot data is loaded and widgets update.
///
/// The scheduler stores non-owning pointers. A collection must therefore remain alive until the
/// scheduled event is processed.
//--------------------------------------------------------------------------------------------------
class RiaPlotCollectionScheduler : public caf::Scheduler
{
public:
    /// Constructs the scheduler. Prefer instance() for process-wide scheduling.
    RiaPlotCollectionScheduler();

    /// Destroys the scheduler without taking ownership of queued collections.
    ~RiaPlotCollectionScheduler() override;

    /// @return The process-wide plot-collection scheduler.
    static RiaPlotCollectionScheduler* instance();

    /// Queues collections for deferred data loading and plot updates.
    /// @param plotCollections Non-owning pointers to collections that must remain valid until processing.
    void schedulePlotCollectionUpdate( const std::vector<RimPlotCollection*> plotCollections );

    /// Loads data and updates all plots in each queued, non-null collection.
    void performScheduledUpdates() override;

private:
    /// Collections awaiting the next scheduler pass.
    std::vector<RimPlotCollection*> m_plotCollectionsToUpdate;
};
