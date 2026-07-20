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
/// @brief Implements deferred 3D view display-model updates and redraws.

#include "RiaViewRedrawScheduler.h"

#include "Rim3dView.h"

#include <set>

//--------------------------------------------------------------------------------------------------
/// Construction leaves the queue empty; caf::Scheduler supplies timer management.
//--------------------------------------------------------------------------------------------------
RiaViewRedrawScheduler::RiaViewRedrawScheduler()
{
}

//--------------------------------------------------------------------------------------------------
/// Queued views are non-owning and are not destroyed by this object.
//--------------------------------------------------------------------------------------------------
RiaViewRedrawScheduler::~RiaViewRedrawScheduler()
{
}

//--------------------------------------------------------------------------------------------------
/// Uses a function-local static so the scheduler is created on first use.
//--------------------------------------------------------------------------------------------------
RiaViewRedrawScheduler* RiaViewRedrawScheduler::instance()
{
    static RiaViewRedrawScheduler theInstance;

    return &theInstance;
}

//--------------------------------------------------------------------------------------------------
/// Queues the view and starts a zero-delay timer so rebuilding occurs on the next event-loop pass.
//--------------------------------------------------------------------------------------------------
void RiaViewRedrawScheduler::scheduleDisplayModelUpdateAndRedraw( Rim3dView* resViewToUpdate )
{
    m_resViewsToUpdate.push_back( resViewToUpdate );

    startTimer( 0 );
}

//--------------------------------------------------------------------------------------------------
/// Removes pending work when project or UI state is being reset.
//--------------------------------------------------------------------------------------------------
void RiaViewRedrawScheduler::clearViewsScheduledForUpdate()
{
    m_resViewsToUpdate.clear();
}

//--------------------------------------------------------------------------------------------------
/// Sets both deduplicate requests and separate dependency order before rebuilding display models.
//--------------------------------------------------------------------------------------------------
void RiaViewRedrawScheduler::updateAndRedrawScheduledViews()
{
    // Compress to remove duplicates
    // and update dependent views after independent views

    std::set<Rim3dView*> independent3DViewsToUpdate;
    std::set<Rim3dView*> dependent3DViewsToUpdate;

    for ( size_t i = 0; i < m_resViewsToUpdate.size(); ++i )
    {
        if ( !m_resViewsToUpdate[i] ) continue;

        if ( m_resViewsToUpdate[i]->viewController() )
            dependent3DViewsToUpdate.insert( m_resViewsToUpdate[i] );
        else
            independent3DViewsToUpdate.insert( m_resViewsToUpdate[i] );
    }

    for ( std::set<Rim3dView*>::iterator it = independent3DViewsToUpdate.begin(); it != independent3DViewsToUpdate.end(); ++it )
    {
        if ( *it )
        {
            ( *it )->createDisplayModelAndRedraw();
        }
    }

    for ( std::set<Rim3dView*>::iterator it = dependent3DViewsToUpdate.begin(); it != dependent3DViewsToUpdate.end(); ++it )
    {
        if ( *it )
        {
            ( *it )->createDisplayModelAndRedraw();
        }
    }

    m_resViewsToUpdate.clear();
}

//--------------------------------------------------------------------------------------------------
/// The timer callback delegates to the synchronous update routine for reuse during project loading.
//--------------------------------------------------------------------------------------------------
void RiaViewRedrawScheduler::performScheduledUpdates()
{
    updateAndRedrawScheduledViews();
}
