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
/// @brief Implements deferred plot-collection data update scheduling.

#include "RiaPlotCollectionScheduler.h"

#include "RimAbstractPlotCollection.h"
#include "RimViewWindow.h"

#include <QTimer>

//--------------------------------------------------------------------------------------------------
/// Construction performs no eager work; the base scheduler owns the timer behavior.
//--------------------------------------------------------------------------------------------------
RiaPlotCollectionScheduler::RiaPlotCollectionScheduler()
{
}

//--------------------------------------------------------------------------------------------------
/// Queued collections are non-owning and are not deleted by the scheduler.
//--------------------------------------------------------------------------------------------------
RiaPlotCollectionScheduler::~RiaPlotCollectionScheduler()
{
}

//--------------------------------------------------------------------------------------------------
/// Uses a function-local static to provide lazy process-wide construction.
//--------------------------------------------------------------------------------------------------
RiaPlotCollectionScheduler* RiaPlotCollectionScheduler::instance()
{
    static RiaPlotCollectionScheduler theInstance;

    return &theInstance;
}

//--------------------------------------------------------------------------------------------------
/// Appends requests and starts a zero-delay timer so updates occur after the current model change.
//--------------------------------------------------------------------------------------------------
void RiaPlotCollectionScheduler::schedulePlotCollectionUpdate( const std::vector<RimPlotCollection*> plotCollections )
{
    m_plotCollectionsToUpdate.insert( m_plotCollectionsToUpdate.end(), plotCollections.begin(), plotCollections.end() );

    startTimer( 0 );
}

//--------------------------------------------------------------------------------------------------
/// Null entries are tolerated because callers may build request lists conditionally.
//--------------------------------------------------------------------------------------------------
void RiaPlotCollectionScheduler::performScheduledUpdates()
{
    for ( auto p : m_plotCollectionsToUpdate )
    {
        if ( p == nullptr ) continue;

        p->loadDataAndUpdateAllPlots();
    }
}
