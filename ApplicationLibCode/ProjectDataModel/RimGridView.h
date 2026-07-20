/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Abstract 3D view for grid cells, filters, intersections, surfaces, and overlays.
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

#pragma once

#include "Rim3dView.h"

#include "cvfArray.h"

class RimEclipseContourMapProjection;
class Rim3dOverlayInfoConfig;
class RimIntersectionCollection;
class RimIntersectionResultsDefinitionCollection;
class RimPropertyFilterCollection;
class RimGridCollection;
class RimCellFilterCollection;
class RimWellMeasurementInViewCollection;
class RimSurfaceInViewCollection;
class RimSeismicSectionCollection;
class RimPolygonInViewCollection;

/// @brief Extends Rim3dView with cell visibility and grid-specific auxiliary geometry.
///
/// Concrete Eclipse and geo-mechanical views calculate cell visibility for their respective data
/// models. RimGridView owns shared filters, intersections, surfaces, polygons, seismic sections,
/// measurements, and overlay configuration and maintains transient render models for those objects.
/// A second cell-filter collection can temporarily override the view's own filters when views are
/// linked; ownership is transferred through its PDM child field.
class RimGridView : public Rim3dView
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes shared grid-view collections and empty transient visualization models.
    RimGridView();

    /// Emitted whenever the effective per-cell visibility mask changes.
    caf::Signal<> cellVisibilityChanged;

    /// Enables or disables rendering of reservoir grid cells.
    void showGridCells( bool enableGridCells );

    /// @return Owned configuration for text and statistics shown over the 3D view.
    Rim3dOverlayInfoConfig* overlayInfoConfig() const;

    /// @return Cached effective cell mask, calculating it for the current time step when necessary.
    cvf::ref<cvf::UByteArray> currentTotalCellVisibility();

    /// @name Owned auxiliary-geometry collections
    /// @{
    RimIntersectionCollection*                  intersectionCollection() const;
    virtual RimSurfaceInViewCollection*         surfaceInViewCollection() const;
    RimIntersectionResultsDefinitionCollection* separateIntersectionResultsCollection() const;
    RimIntersectionResultsDefinitionCollection* separateSurfaceResultsCollection() const;
    RimWellMeasurementInViewCollection*         measurementCollection() const;
    RimSeismicSectionCollection*                seismicSectionCollection() const;
    RimPolygonInViewCollection*                 polygonInViewCollection() const;
    /// @}

    /// @return Concrete view's active property-filter collection.
    virtual const RimPropertyFilterCollection* propertyFilterCollection() const = 0;

    /// Invalidates visibility and refreshes geometry after cell-filter changes.
    void                           cellFiltersUpdated();
    /// @return Effective mutable cell filters, preferring an installed override.
    RimCellFilterCollection*       cellFilterCollection();
    /// @return Effective const cell filters, preferring an installed override.
    const RimCellFilterCollection* cellFilterCollection() const;

    /// @return Whether linked-view logic has installed an override filter collection.
    bool hasOverriddenCellFilterCollection();
    /// Replaces the owned override with @p rfc, taking ownership through the child field.
    void setOverrideCellFilterCollection( RimCellFilterCollection* rfc );
    /// Makes the override collection the view's regular collection and clears override state.
    void replaceCellFilterCollectionWithOverride();

    /// @return `true`; this view renders grid cells rather than only non-grid geometry.
    bool isGridVisualizationMode() const override;

    /// @return User-facing description of active cell and property filters.
    QString activeFiltersDisplayText() const override;

    /// Rebuilds displayed well-measurement geometry.
    void updateWellMeasurements();
    /// Refreshes project-tree items for the requested auxiliary object categories.
    void updateViewTreeItems( RiaDefines::ItemIn3dView itemType ) override;

    /// @return Owned configuration controlling which grids are shown.
    RimGridCollection* gridCollection() const;

protected:
    /// Hook for concrete views to update geometry and dependent state after filter changes.
    virtual void updateViewFollowingCellFilterUpdates();
    /// Clears cached visibility when base-class state invalidates reservoir geometry.
    void         onClearReservoirCellVisibilitiesIfNecessary() override;
    /// Calculates the concrete view's complete cell mask for @p timeStep.
    virtual void calculateCurrentTotalCellVisibility( cvf::UByteArray* totalVisibility, int timeStep ) = 0;
    /// Makes the grid overlay configuration the selected overlay settings object.
    void         selectOverlayInfoConfig() override;
    /// Drops the cached total-visibility mask.
    void         clearReservoirCellVisibilities();
    /// Appends mandatory owned grid-view collections to project-tree ordering.
    void         addRequiredUiTreeObjects( caf::PdmUiTreeOrdering& uiTreeOrdering );
    /// Adds visible polygon render parts clipped and transformed for this view.
    void         appendPolygonPartsToModel( caf::DisplayCoordTransform* scaleTransform, const cvf::BoundingBox& boundingBox );

    /// Responds to grid-display and auxiliary-collection property changes.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;
    /// Restores mandatory collections and transient state after project loading.
    void initAfterRead() override;

    /// Builds intersection geometry for the current time step.
    void appendIntersectionsForCurrentTimeStep();
    /// Adds intersections using the current filter-activation state.
    void appendIntersectionsToModel( bool cellFiltersActive, bool propertyFiltersActive );

    /// Calculates visibility contributed by @p geomTypes for @p timeStep.
    virtual void calculateCellVisibility( cvf::UByteArray* visibility, std::vector<RivCellSetEnum> geomTypes, int timeStep = 0 ) = 0;

protected:
    /// Transient render model containing visible surfaces.
    cvf::ref<cvf::ModelBasicList> m_surfaceVizModel;
    /// Transient render model containing intersection geometry.
    cvf::ref<cvf::ModelBasicList> m_intersectionVizModel;
    /// Transient render model containing polygon geometry.
    cvf::ref<cvf::ModelBasicList> m_polygonVizModel;

    /// Owned intersection definitions displayed in this view.
    caf::PdmChildField<RimIntersectionCollection*> m_intersectionCollection;

    /// Owned result definitions applied specifically to intersections.
    caf::PdmChildField<RimIntersectionResultsDefinitionCollection*> m_intersectionResultDefCollection;
    /// Owned result definitions applied specifically to surfaces.
    caf::PdmChildField<RimIntersectionResultsDefinitionCollection*> m_surfaceResultDefCollection;

    /// Owned overlay information settings.
    caf::PdmChildField<Rim3dOverlayInfoConfig*>             m_overlayInfoConfig;
    /// Owned grid-visibility settings.
    caf::PdmChildField<RimGridCollection*>                  m_gridCollection;
    /// Owned well measurements displayed in this view.
    caf::PdmChildField<RimWellMeasurementInViewCollection*> m_wellMeasurementCollection;
    /// Owned surfaces displayed in this view.
    caf::PdmChildField<RimSurfaceInViewCollection*>         m_surfaceCollection;
    /// Owned regular cell-filter collection.
    caf::PdmChildField<RimCellFilterCollection*>            m_cellFilterCollection;
    /// Owned temporary cell-filter override installed by view linking.
    caf::PdmChildField<RimCellFilterCollection*>            m_overrideCellFilterCollection;
    /// Owned seismic sections displayed in this view.
    caf::PdmChildField<RimSeismicSectionCollection*>        m_seismicSectionCollection;
    /// Owned polygons displayed in this view.
    caf::PdmChildField<RimPolygonInViewCollection*>         m_polygonInViewCollection;

private:
    /// Creates render parts for the current selection when requested by the viewer.
    void onCreatePartCollectionFromSelection( cvf::Collection<cvf::Part>* parts ) override;

private:
    /// Lazily computed effective cell visibility for the current frame.
    cvf::ref<cvf::UByteArray> m_currentReservoirCellVisibility;
    /// Remembers fault-only mesh mode while grid-cell display is temporarily disabled.
    bool                      m_previousGridModeMeshLinesWasFaults;
};
