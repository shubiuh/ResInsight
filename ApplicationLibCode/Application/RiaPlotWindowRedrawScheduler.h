/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2019-     Equinor ASA
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
/// @brief Scheduler for deferred plot-window and plot-widget redraws.
#pragma once

#include "RiaDefines.h"

#include "cafScheduler.h"

#include <QPointer>

#include <map>
#include <set>

class RiuMultiPlotPage;
class RiuMultiPlotBook;
class RiuPlotWidget;

//--------------------------------------------------------------------------------------------------
/// @brief Coalesces deferred updates for plot books, plot pages, and individual plot widgets.
///
/// Plot updates are frequently requested by several model notifications during one UI operation.
/// This scheduler postpones the work until control returns to the event loop and combines update
/// flags for the same book or page. A book-level plot update supersedes pending updates for its
/// child pages, preventing the same hierarchy from being rebuilt twice.
///
/// QPointer keys make queued requests safe when windows or widgets are destroyed before the timer
/// fires. Requests added while an update is running remain queued for a later scheduler pass.
//--------------------------------------------------------------------------------------------------
class RiaPlotWindowRedrawScheduler : public caf::Scheduler
{
    Q_OBJECT

public:
    /// @return The process-wide plot redraw scheduler.
    static RiaPlotWindowRedrawScheduler* instance();

    /// Queues an update for a multi-plot book and merges it with any existing update flags.
    /// @param plotWindow Book to update; ownership remains with the UI.
    /// @param updateType Parts of the book that must be refreshed.
    void scheduleMultiPlotBookUpdate( RiuMultiPlotBook*                   plotWindow,
                                      RiaDefines::MultiPlotPageUpdateType updateType = RiaDefines::MultiPlotPageUpdateType::ALL );

    /// Queues an update for a multi-plot page and merges it with any existing update flags.
    /// @param plotWindow Page to update; ownership remains with the UI.
    /// @param updateType Parts of the page that must be refreshed.
    void scheduleMultiPlotPageUpdate( RiuMultiPlotPage*                   plotWindow,
                                      RiaDefines::MultiPlotPageUpdateType updateType = RiaDefines::MultiPlotPageUpdateType::ALL );

    /// Queues a lightweight replot of one plot widget.
    /// @param plotWidget Widget to replot; duplicate requests are collapsed.
    void schedulePlotWidgetReplot( RiuPlotWidget* plotWidget );

    /// Discards all pending book, page, and widget updates.
    void clearAllScheduledUpdates();

    /// Applies queued updates in book, page, then widget order.
    void performScheduledUpdates() override;

private:
    /// Pending page updates and their accumulated update flags.
    std::map<QPointer<RiuMultiPlotPage>, RiaDefines::MultiPlotPageUpdateType> m_plotPagesToUpdate;

    /// Pending book updates and their accumulated update flags.
    std::map<QPointer<RiuMultiPlotBook>, RiaDefines::MultiPlotPageUpdateType> m_plotBooksToUpdate;

    /// Widgets requiring only a final replot after structural updates are complete.
    std::set<QPointer<RiuPlotWidget>> m_plotWidgetsToReplot;
};
