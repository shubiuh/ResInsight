/////////////////////////////////////////////////////////////////////////////////
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

/// @file
/// @brief Reservoir grid view with Eclipse results, wells, faults, filters, and completions.

#pragma once

#include "QuickAccess/RimFieldQuickAccessInterface.h"
#include "RimGridView.h"

#include "cafAppEnum.h"
#include "cafPdmChildField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"

#include "cvfArray.h"

#include "cvfObject.h"

// Includes to make Pdm work for cvf::Color and cvf:Mat
#include "cafPdmFieldCvfColor.h"
#include "cafPdmFieldCvfMat4d.h"

#include <functional>

class RigActiveCellInfo;
class RigCaseCellResultsData;
class RigGridBase;
class RigGridCellFaceVisibilityFilter;
class RigMainGrid;
class Rim3dOverlayInfoConfig;
class RimCellEdgeColors;
class RimCellRangeFilter;
class RimCellRangeFilterCollection;
class RimDataFilterInViewCollection;
class RimEclipseCase;
class RimEclipseCellColors;
class RimEclipseFaultColors;
class RimEclipsePropertyFilter;
class RimEclipsePropertyFilterCollection;
class RimFaultDistanceCollection;
class RimFaultInViewCollection;
class RimFilterInViewCollection;
class RimReservoirCellResultsStorage;
class RimReservoirCellResultsStorage;
class RimSimWellInViewCollection;
class RimStimPlanColors;
class RimVirtualPerforationResults;
class RiuViewer;
class RivReservoirSimWellsPartMgr;
class RivExtrudedCurveIntersectionPartMgr;
class RivReservoirViewPartMgr;
class RivStreamlinesPartMgr;
class RivRefinementRegionPartMgr;
class RimRegularLegendConfig;
class RimTernaryLegendConfig;
class RimEclipseResultDefinition;
class RimElementVectorResult;
class RimStreamlineInViewCollection;
class RimMultipleEclipseResults;
class RigEclipseResultAddress;
class RimFaultReactivationModelCollection;
class RimCameraPosition;
class RimRefinementRegionCollection;

namespace cvf
{
class Transform;
class ScalarMapperUniformLevels;
class ModelBasicList;
class OverlayItem;
} // namespace cvf

//==================================================================================================
/// @brief Implements RimGridView for Eclipse reservoir grids and result data.
///
/// The view owns result definitions, legends, filters, well and fault display collections,
/// streamlines, refinement regions, and runtime part managers. Its Eclipse case is referenced rather
/// than owned and is propagated to child objects. Linked views may install non-owning property-filter
/// overrides while the native owned collection remains available for later restoration.
//==================================================================================================
class RimEclipseView : public RimGridView, public RimFieldQuickAccessInterface
{
    CAF_PDM_HEADER_INIT;

public:
    /// Camera handling applied when the view is assigned a different Eclipse case.
    enum class RimCaseChangeBehaviour
    {
        ZOOM_TO_FIT,
        KEEP_CURRENT_SETTINGS,
        STORE_VIEW_SETTINGS_FOR_CASE
    };

public:
    /// Creates result/filter child objects and transient reservoir part managers.
    RimEclipseView();
    /// Detaches case relationships and releases runtime geometry managers.
    ~RimEclipseView() override;

    /// @return Eclipse-data content mask, augmented by visible auxiliary content.
    RiaDefines::View3dContent viewContent() const override;

    /// @name Owned result and geometry configuration
    /// @{
    RimEclipseCellColors*                cellResult() const;
    RimCellEdgeColors*                   cellEdgeResult() const;
    RimElementVectorResult*              elementVectorResult() const;
    RimEclipseFaultColors*               faultResultSettings() const;
    RimStimPlanColors*                   fractureColors() const;
    RimSimWellInViewCollection*          wellCollection() const;
    RimFaultInViewCollection*            faultCollection() const;
    RimFaultDistanceCollection*          faultDistanceCollection() const;
    RimVirtualPerforationResults*        virtualPerforationResult() const;
    RimStreamlineInViewCollection*       streamlineCollection() const;
    RimFaultReactivationModelCollection* faultReactivationModelCollection() const;
    RimRefinementRegionCollection*       refinementRegionCollection() const;
    /// @}

    /// @return Whether invalid cells are included in effective visibility.
    bool showInvalidCells() const;
    /// @return Whether inactive cells are included in effective visibility.
    bool showInactiveCells() const;

    /// @return Effective property filters, including a linked-view override when installed.
    const RimPropertyFilterCollection* propertyFilterCollection() const override;

    /// @name Native and overridden Eclipse property filters
    /// @{
    RimEclipsePropertyFilterCollection*       eclipsePropertyFilterCollection();
    const RimEclipsePropertyFilterCollection* eclipsePropertyFilterCollection() const;
    void                                      setOverridePropertyFilterCollection( RimEclipsePropertyFilterCollection* pfc );
    /// @}

    /// @return Owned selection of case-level data filters used in this view.
    RimDataFilterInViewCollection* dataFiltersInView() const;

    /// @return Whether any effective property or data filter is active.
    bool hasActivePropertyOrDataFilters() const;
    /// @return Whether active filters require time-step-dependent visibility.
    bool hasActiveDynamicPropertyOrDataFilters() const;

    /// @name Effective runtime result data
    /// @{
    RigCaseCellResultsData*  currentGridCellResults() const;
    const RigActiveCellInfo* currentActiveCellInfo() const;
    RimEclipseCellColors*    currentFaultResultColors();
    /// @}

    /// Assigns a non-owned case and propagates it to child configurations.
    void            setEclipseCase( RimEclipseCase* reservoir );
    /// @return Current non-owned Eclipse case.
    RimEclipseCase* eclipseCase() const;
    /// @return Current Eclipse case as RimCase.
    RimCase*        ownerCase() const override;

    /// Installs a runtime callback supplying cases eligible for this view.
    void setEclipseCaseProvider( std::function<std::vector<RimEclipseCase*>()> provider );

    /// @return Loaded main grid, or `nullptr` while the case is closed.
    RigMainGrid* mainGrid() const;

    /// @return Whether visible results, wells, streamlines, or filters vary by time step.
    bool isTimeStepDependentDataVisible() const override;

    /// @name Geometry invalidation and result updates
    /// @{
    void scheduleGeometryRegen( RivCellSetEnum geometryType ) override;
    void scheduleReservoirGridGeometryRegen();
    void scheduleSimWellGeometryRegen();
    void updateDisplayModelForWellResults();

    void calculateCompletionTypeAndRedrawIfRequired();
    /// @}

    /// @return Whether virtual connection-factor geometry contributes to the scene.
    bool isVirtualConnectionFactorGeometryVisible() const;
    /// @return Whether the main grid contributes visible geometry.
    bool isMainGridVisible() const;

    /// @return Grid-part categories selected for rendering.
    const std::vector<RivCellSetEnum>& visibleGridParts() const;
    /// @name Reservoir grid part manager
    /// @{
    const RivReservoirViewPartMgr*     reservoirGridPartManager() const;
    RivReservoirViewPartMgr*           reservoirGridPartManager();
    /// @}

    /// Adds visible well and fence cells for @p grid to @p visibleCells.
    void calculateVisibleWellCellsIncFence( cvf::UByteArray* visibleCells, RigGridBase* grid );

    /// Applies changed view fields to geometry, results, legends, and editors.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;
    /// Handles changes originating in owned result and filter objects.
    void childFieldChangedByUi( const caf::PdmFieldHandle* changedChildField ) override;
    /// Refreshes filter icons from effective active state.
    void updateIconStateForFilterCollections();

    void defineAxisLabels( cvf::String* xLabel, cvf::String* yLabel, cvf::String* zLabel ) override;

    bool isUsingFormationNames() const override;

    QString activeFiltersDisplayText() const override;

    /// Calculates the complete visibility mask for all visible grid parts.
    void calculateCurrentTotalCellVisibility( cvf::UByteArray* totalVisibility, int timeStep ) override;

    /// Calculates visibility contributed by @p geomTypes at @p timeStep.
    void calculateCellVisibility( cvf::UByteArray* visibility, std::vector<RivCellSetEnum> geomTypes, int timeStep = 0 ) override;

    /// @return Legends required by active result definitions.
    std::vector<RimLegendConfig*> legendConfigs() const override;
    /// @return Fallback category color for @p geometryType.
    cvf::Color4f                  colorFromCellCategory( RivCellSetEnum geometryType ) const;

    std::vector<RigEclipseResultAddress> additionalResultsForResultInfo() const;

    /// @return Named groups of commonly edited fields eligible for Quick Access.
    std::map<QString, std::vector<caf::PdmFieldHandle*>> quickAccessFields() override;

protected:
    /// Restores child relationships and migrates legacy custom-case state after reading.
    void                 initAfterRead() override;
    /// Defines result, filter, well, fault, and display property groups.
    void                 defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;
    /// Builds the view's project-tree hierarchy.
    void                 defineUiTreeOrdering( caf::PdmUiTreeOrdering& uiTreeOrdering, QString uiConfigName = "" ) override;
    /// Opens case data, initializes children, and builds the first display model.
    void                 onLoadDataAndUpdate() override;
    /// Uses the view-name field as the generic object description.
    caf::PdmFieldHandle* userDescriptionField() override;

    virtual std::vector<size_t> activeTimeStepIndices( bool propertyFiltersActive );

    bool isShowingActiveCellsOnly() override;
    void onUpdateDisplayModelForCurrentTimeStep() override;
    void updateVisibleCellColors();
    void updateVisibleGeometries();

    /// @name Scene construction helpers
    /// @{
    void appendWellsAndFracturesToModel();
    void appendElementVectorResultToModel();
    void appendStreamlinesToModel();
    /// @}

    void                             onCreateDisplayModel() override;
    RimPropertyFilterCollection*     nativePropertyFilterCollection();
    virtual std::set<RivCellSetEnum> allVisibleFaultGeometryTypes() const;

    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;
    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;

private:
    /// Builds the automatic view name from case, result, and filter state.
    QString createAutoName() const override;

    void onUpdateDisplayModelVisibility() override;

    std::vector<size_t> indicesToVisibleGrids() const;
    /// Applies current vertical scaling to runtime transforms and geometry.
    void                onUpdateScaleTransform() override;
    /// @return Runtime transform shared by reservoir part managers.
    cvf::Transform*     scaleTransform() override;

    void onUpdateStaticCellColors() override;
    void updateStaticCellColors( RivCellSetEnum geometryType );

    /// Updates all legends after result, range, or time-step changes.
    void onUpdateLegends() override;
    void updateLegendRangesTextAndVisibility( RimRegularLegendConfig*     legendConfig,
                                              RimTernaryLegendConfig*     ternaryLegendConfig,
                                              QString                     legendLabel,
                                              RimEclipseResultDefinition* eclResDef,
                                              int                         timeStepIndex );

    void updateVirtualConnectionLegendRanges();

    void updateFaultColors();

    void synchronizeWellsWithResults();

    void   onClampCurrentTimestep() override;
    size_t onTimeStepCountRequested() override;

    void onAnimationsUpdate( const caf::SignalEmitter* emitter );

    void setVisibleGridParts( const std::vector<RivCellSetEnum>& cellSets );
    void setVisibleGridPartsWatertight();

    /// Propagates the current case into every owned case-dependent child.
    void propagateEclipseCaseToChildObjects();
    /// Applies the configured camera policy while changing cases.
    void storeCurrentAndApplyNewCameraPosition( RimEclipseCase* previousCase, RimEclipseCase* newCase );

protected:
    /// Transient render model for fault-reactivation geometry.
    cvf::ref<cvf::ModelBasicList> m_faultReactVizModel;
    /// Transient render model for refinement-region geometry.
    cvf::ref<cvf::ModelBasicList> m_refinementRegionsVizModel;

    /// Non-owning current Eclipse case.
    caf::PdmPtrField<RimEclipseCase*>                   m_eclipseCase;
    /// Camera policy used when m_eclipseCase changes.
    caf::PdmField<caf::AppEnum<RimCaseChangeBehaviour>> m_caseChangeBehaviour;

private:
    /// Include geometrically invalid cells in visibility calculations.
    caf::PdmField<bool> m_showInvalidCells;
    /// Include inactive cells in visibility calculations.
    caf::PdmField<bool> m_showInactiveCells;

    /// @name Owned result visualization definitions
    /// @{
    caf::PdmChildField<RimEclipseCellColors*>         m_cellResult;
    caf::PdmChildField<RimCellEdgeColors*>            m_cellEdgeResult;
    caf::PdmChildField<RimElementVectorResult*>       m_elementVectorResult;
    caf::PdmChildField<RimEclipseFaultColors*>        m_faultResultSettings;
    caf::PdmChildField<RimStimPlanColors*>            m_fractureColors;
    caf::PdmChildField<RimVirtualPerforationResults*> m_virtualPerforationResult;
    /// @}

    /// @name Owned well, fault, reactivation, and streamline configuration
    /// @{
    caf::PdmChildField<RimSimWellInViewCollection*>          m_wellCollection;
    caf::PdmChildField<RimFaultInViewCollection*>            m_faultCollection;
    caf::PdmChildField<RimFaultReactivationModelCollection*> m_faultReactivationModelCollection;
    caf::PdmChildField<RimStreamlineInViewCollection*>       m_streamlineCollection;
    /// @}

    /// Owned native property filters.
    caf::PdmChildField<RimEclipsePropertyFilterCollection*> m_propertyFilterCollection;
    /// Non-owning property-filter override installed by linked views.
    caf::PdmPointer<RimEclipsePropertyFilterCollection>     m_overridePropertyFilterCollection;

    /// Owned selection of case-level data filters applied to this view.
    caf::PdmChildField<RimDataFilterInViewCollection*> m_dataFiltersInView;

    /// Owned local filter-in-view configuration.
    caf::PdmChildField<RimFilterInViewCollection*> m_filterInViewCollection;

    /// Legacy custom-case reference retained for migration.
    caf::PdmPtrField<RimEclipseCase*> m_customEclipseCase_OBSOLETE;

    /// @name Runtime scene-part managers
    /// @{
    cvf::ref<RivReservoirViewPartMgr>     m_reservoirGridPartManager;
    cvf::ref<RivReservoirSimWellsPartMgr> m_simWellsPartManager;
    cvf::ref<RivStreamlinesPartMgr>       m_streamlinesPartManager;
    cvf::ref<RivRefinementRegionPartMgr>  m_refinementRegionPartManager;
    /// @}

    /// Grid-part categories selected for the latest model build.
    std::vector<RivCellSetEnum> m_visibleGridParts;

    /// Owned additional addresses displayed by result-info inspection.
    caf::PdmChildField<RimMultipleEclipseResults*>     m_additionalResultsForResultInfo;
    /// Owned camera presets stored for case switching.
    caf::PdmChildArrayField<RimCameraPosition*>        m_cameraPositions;
    /// Owned local refinement regions.
    caf::PdmChildField<RimRefinementRegionCollection*> m_refinementRegions;

    /// Runtime callback used by specialized collections to constrain case choices.
    std::function<std::vector<RimEclipseCase*>()> m_eclipseCaseProvider;
};
