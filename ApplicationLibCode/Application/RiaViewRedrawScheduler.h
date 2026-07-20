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
/// @brief Scheduler for deferred 3D view display-model updates and redraws.

#pragma once

#include "cafScheduler.h"

#include "cafPdmPointer.h"

#include <vector>

class Rim3dView;

//--------------------------------------------------------------------------------------------------
/// @brief Defers and orders expensive display-model rebuilds for 3D views.
///
/// Multiple model changes can request a redraw of the same view before Qt returns to its event
/// loop. The scheduler stores those requests using PDM-aware pointers, removes duplicates during
/// processing, and rebuilds independent views before views controlled by another view. That order
/// ensures dependent comparison or linked views observe an up-to-date source display model.
//--------------------------------------------------------------------------------------------------
class RiaViewRedrawScheduler : public caf::Scheduler
{
public:
    /// Constructs an empty redraw queue. Prefer instance() for process-wide scheduling.
    RiaViewRedrawScheduler();

    /// Destroys the scheduler without owning any queued view.
    ~RiaViewRedrawScheduler() override;

    /// @return The process-wide 3D-view redraw scheduler.
    static RiaViewRedrawScheduler* instance();

    /// Queues a display-model rebuild and redraw for @p resViewToUpdate.
    /// @param resViewToUpdate View to update; may be deleted before processing.
    void scheduleDisplayModelUpdateAndRedraw( Rim3dView* resViewToUpdate );

    /// Discards every pending view update.
    void clearViewsScheduledForUpdate();

    /// Immediately performs queued redraws, independent views first, and clears the queue.
    void updateAndRedrawScheduledViews();

    /// Handles the scheduler timer by calling updateAndRedrawScheduledViews().
    void performScheduledUpdates() override;

private:
    /// Pending non-owning view references that null themselves if their PDM objects are deleted.
    std::vector<caf::PdmPointer<Rim3dView>> m_resViewsToUpdate;
};
