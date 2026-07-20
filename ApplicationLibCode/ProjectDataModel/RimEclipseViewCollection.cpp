/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2024     Equinor ASA
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
/// Implements ownership, initialization, and case-provider propagation for
/// collections of Eclipse reservoir views.

#include "RimEclipseViewCollection.h"

#include "RiaLogging.h"
#include "RiaPreferences.h"

#include "RigCaseCellResultsData.h"

#include "QuickAccess/RimQuickAccessCollection.h"
#include "Rim3dView.h"
#include "RimCase.h"
#include "RimCellEdgeColors.h"
#include "RimEclipseCase.h"
#include "RimEclipseCellColors.h"
#include "RimEclipseView.h"
#include "RimFaultInViewCollection.h"
#include "RimGridView.h"
#include "RimProject.h"
#include "RimStimPlanColors.h"

CAF_PDM_SOURCE_INIT( RimEclipseViewCollection, "EclipseViewCollection", "EclipseViewCollection" );

//--------------------------------------------------------------------------------------------------
/// Constructs an initially empty, non-deletable view container.
//--------------------------------------------------------------------------------------------------
RimEclipseViewCollection::RimEclipseViewCollection()
{
    CAF_PDM_InitObject( "Views", ":/3DView16x16.png" );

    CAF_PDM_InitFieldNoDefault( &m_views, "Views", "Eclipse Views" );

    setDeletable( false );
}

//--------------------------------------------------------------------------------------------------
/// Destroys the collection and its framework-owned child views.
//--------------------------------------------------------------------------------------------------
RimEclipseViewCollection::~RimEclipseViewCollection()
{
}

//--------------------------------------------------------------------------------------------------
/// Returns the owned Eclipse views in collection order.
//--------------------------------------------------------------------------------------------------
std::vector<RimEclipseView*> RimEclipseViewCollection::views() const
{
    return m_views.childrenByType();
}

//--------------------------------------------------------------------------------------------------
/// Refreshes the owning case because this collection is hidden when embedded there.
//--------------------------------------------------------------------------------------------------
void RimEclipseViewCollection::onChildDeleted( caf::PdmChildArrayFieldHandle* childArray, std::vector<caf::PdmObjectHandle*>& referringObjects )
{
    // If a view is child of a case, the view collection object is hidden in the tree view. Find the parent case and update connected editors.
    if ( auto parentCase = firstAncestorOrThisOfType<RimCase>() )
    {
        parentCase->updateConnectedEditors();
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RimEclipseViewCollection::isEmpty()
{
    return !m_views.hasChildren();
}

//--------------------------------------------------------------------------------------------------
/// Creates a fully initialized view using application defaults and the case's
/// default result before exposing it to the UI.
//--------------------------------------------------------------------------------------------------
RimEclipseView* RimEclipseViewCollection::addView( RimEclipseCase* eclipseCase )
{
    RimEclipseView* view = new RimEclipseView();

    view->setEclipseCase( eclipseCase );

    // Configure case provider callback
    applyCallbackToView( view );

    auto prefs = RiaPreferences::current();
    view->faultCollection()->setActive( prefs->enableFaultsByDefault() );

    view->cellEdgeResult()->setResultVariable( "MULT" );
    view->cellEdgeResult()->setActive( false );
    view->fractureColors()->setDefaultResultName();

    caf::PdmDocument::updateUiIconStateRecursively( view );

    m_views.push_back( view );

    RimQuickAccessCollection::instance()->addQuickAccessFields( view );

    view->loadDataAndUpdate();

    // Set default values
    if ( view->currentGridCellResults() )
    {
        auto defaultResult = view->currentGridCellResults()->defaultResult();
        view->cellResult()->setFromEclipseResultAddress( defaultResult );
    }

    updateConnectedEditors();

    return view;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimEclipseViewCollection::addView( RimEclipseView* view )
{
    // Configure case provider callback
    applyCallbackToView( view );

    m_views.push_back( view );
    updateConnectedEditors();
}

//--------------------------------------------------------------------------------------------------
/// Detaches @p view without deleting it, allowing callers to transfer ownership.
//--------------------------------------------------------------------------------------------------
void RimEclipseViewCollection::removeView( RimEclipseView* view )
{
    m_views.removeChild( view );
    updateConnectedEditors();
}

//--------------------------------------------------------------------------------------------------
/// Stores the case-list callback and propagates it to existing views so case
/// selectors behave identically for both existing and subsequently added views.
//--------------------------------------------------------------------------------------------------
void RimEclipseViewCollection::setEclipseCaseProvider( std::function<std::vector<RimEclipseCase*>()> provider )
{
    // Store the callback for future views
    if ( provider )
    {
        m_eclipseCaseProvider = provider;
    }
    else
    {
        m_eclipseCaseProvider = nullptr;
    }

    // Apply the callback to all existing views in the collection
    for ( auto view : m_views )
    {
        applyCallbackToView( view );
    }
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimEclipseViewCollection::applyCallbackToView( RimEclipseView* view )
{
    if ( m_eclipseCaseProvider )
    {
        // Use the stored custom callback
        view->setEclipseCaseProvider( m_eclipseCaseProvider );
    }
}
