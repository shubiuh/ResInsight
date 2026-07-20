/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Per-dependent-view synchronization policy for a linked-view group.
//
//  Copyright (C) 2015-     Statoil ASA
//  Copyright (C) 2015-     Ceetron Solutions AS
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

#include "cafIconProvider.h"
#include "cafPdmField.h"
#include "cafPdmPtrField.h"

#include "cvfObject.h"

#include "RimCheckableNamedObject.h"
#include "RivCellSetEnum.h"

class RimGridView;
class Rim3dView;
class RimEclipseView;
class RimGeoMechView;
class RimViewLinker;
class RigCaseToCaseCellMapper;
class RimCellFilter;
class RimPropertyFilter;

//==================================================================================================
/// @brief Controls which master-view properties are applied to one managed dependent view.
///
/// A RimViewLinker owns one controller for each dependent view. The controller stores a non-owning
/// managed-view pointer and independent switches for camera, cursor, time step, results, legends,
/// cell filters, and property filters. Cross-case cell mapping is created lazily when visibility or
/// filter state must be transferred between different grids.
//==================================================================================================
class RimViewController : public RimCheckableNamedObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes an enabled controller with default synchronization policies.
    RimViewController();
    /// Removes active overrides from the managed view before destruction.
    ~RimViewController() override;

    /// @return Whether this controller and its owning link collection are enabled.
    bool isActive() const;

    /// @return Non-owned dependent view managed by this controller.
    Rim3dView* managedView() const;
    /// Associates this controller with @p view and refreshes its presentation and overrides.
    void       setManagedView( Rim3dView* view );

    /// @return Master view supplied by the owning RimViewLinker.
    Rim3dView*     masterView() const;
    /// @return Ancestor linker that owns this controller.
    RimViewLinker* ownerViewLinker() const;

    /// @return Lazily initialized case-to-case mapper, or `nullptr` when mapping is inapplicable.
    const RigCaseToCaseCellMapper* cellMapper();

    /// @name Effective synchronization switches
    /// @{
    bool isCameraLinked() const;
    bool showCursor() const;
    bool isTimeStepLinked() const;

    bool isResultColorControlled() const;
    bool isLegendDefinitionsControlled() const;
    bool isCellFiltersControlled() const;

    bool isVisibleCellsOveridden() const;
    bool isPropertyFilterOveridden() const;
    bool isPropertyFilterDuplicationActive() const;
    /// @}

    /// Schedules display-model recreation for the managed dependent view.
    void scheduleCreateDisplayModelAndRedrawForDependentView() const;
    /// Schedules regeneration of @p geometryType for the managed view.
    void scheduleGeometryRegenForDepViews( RivCellSetEnum geometryType ) const;
    /// Applies every currently enabled master-view override.
    void updateOverrides();
    /// Enables controls only when synchronization is meaningful for the view pair.
    void updateOptionSensitivity();
    /// Restores the managed view's own result, legend, and filter state.
    void removeOverrides();
    /// Refreshes this controller's label and icon from the managed view.
    void updateDisplayNameAndIcon();
    /// Recreates duplicated property filters from the master when enabled.
    void updateDuplicatedPropertyFilters();

    /// Updates a mapped cell-filter override after @p changedFilter changes.
    void updateCellFilterOverrides( const RimCellFilter* changedFilter );
    /// Lets the user choose whether to restore or retain the dependent view's filters.
    void applyCellFilterCollectionByUserChoice();
    /// Updates controlled or duplicated property filters after a master filter changes.
    void updatePropertyFilterOverrides( RimPropertyFilter* changedPropertyFilter );

protected: // Pdm overridden methods
    /// Reapplies affected links and schedules dependent redraws after UI changes.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;
    /// Supplies candidate managed views not already used by the linker.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;
    /// Presents the managed view beneath the controller in project trees.
    void                          defineUiTreeOrdering( caf::PdmUiTreeOrdering& uiTreeOrdering, QString uiConfigName = "" ) override;
    /// Arranges synchronization switches and hides inapplicable controls.
    void                          defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;

private:
    void updateCameraLink();
    void updateTimeStepLink();
    void updateResultColorsControl();
    void updateLegendDefinitions();

    bool isMasterAndDepViewDifferentType() const;
    bool isPropertyFilterControlPossible() const;
    bool isCellFilterMappingApplicable() const;
    bool isCellResultControlAdvisable() const;
    bool isCellFilterControlAdvisable() const;
    bool isPropertyFilterControlAdvisable() const;

    RimEclipseView* managedEclipseView() const;
    RimGeoMechView* managedGeoView() const;

    static void removeOverrides( RimGridView* view );
    static bool askUserToRestoreOriginalCellFilterCollection( const QString& viewName );

private:
    /// Non-owning dependent view reference serialized by PDM.
    caf::PdmPtrField<Rim3dView*> m_managedView;

    /// Synchronizes camera pose and projection.
    caf::PdmField<bool> m_syncCamera;
    /// Displays the master's cursor position in this view.
    caf::PdmField<bool> m_showCursor;
    /// Synchronizes current time step.
    caf::PdmField<bool> m_syncTimeStep;

    /// Uses the master's cell result/color configuration.
    caf::PdmField<bool> m_syncCellResult;
    /// Uses the master's legend definitions.
    caf::PdmField<bool> m_syncLegendDefinitions;

    /// Uses mapped visibility derived from the master's cell filters.
    caf::PdmField<bool> m_syncCellFilters;
    /// Directly controls property filters from the master where compatible.
    caf::PdmField<bool> m_syncPropertyFilters;
    /// Maintains independent copies of master property filters for incompatible cases.
    caf::PdmField<bool> m_duplicatePropertyFilters;

    /// Lazily created mapping between master and managed case cells.
    cvf::ref<RigCaseToCaseCellMapper> m_caseToCaseCellMapper;
};
