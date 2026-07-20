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
/// @brief Implements deferred quick-access data update scheduling.

#include "RiaQuickAccessScheduler.h"

#include "QuickAccess/RimQuickAccessCollection.h"

//--------------------------------------------------------------------------------------------------
/// Construction performs no eager editor work.
//--------------------------------------------------------------------------------------------------
RiaQuickAccessScheduler::RiaQuickAccessScheduler()
{
}

//--------------------------------------------------------------------------------------------------
/// Uses a function-local static to make the scheduler available without explicit ownership.
//--------------------------------------------------------------------------------------------------
RiaQuickAccessScheduler* RiaQuickAccessScheduler::instance()
{
    static RiaQuickAccessScheduler theInstance;

    return &theInstance;
}

//--------------------------------------------------------------------------------------------------
/// A zero-delay timer coalesces all calls made during the current event-loop turn.
//--------------------------------------------------------------------------------------------------
void RiaQuickAccessScheduler::scheduleDisplayModelUpdateAndRedraw()
{
    startTimer( 0 );
}

//--------------------------------------------------------------------------------------------------
/// The collection determines which connected editors are stale and need refreshing.
//--------------------------------------------------------------------------------------------------
void RiaQuickAccessScheduler::performScheduledUpdates()
{
    auto quickColl = RimQuickAccessCollection::instance();
    if ( quickColl )
    {
        quickColl->updateAllRequiredEditors();
    }
}
