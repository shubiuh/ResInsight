/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Master/dependent view synchronization group.
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

#include "RivCellSetEnum.h"

#include "cafPdmChildArrayField.h"
#include "cafPdmField.h"
#include "cafPdmPtrField.h"

#include "cvfVector3.h"

#include "RimNamedObject.h"

namespace caf
{
class IconProvider;
}

namespace cvf
{
class BoundingBox;
}

class RimViewController;
class RiuViewer;
class Rim3dView;
class RimCellFilter;
class RimPropertyFilter;

//==================================================================================================
/// @brief Propagates selected state from one master view to controller-managed dependent views.
///
/// The linker owns RimViewController children but only references the master and dependent views.
/// Each controller decides which properties participate. Update functions accept a source view so
/// feedback loops can be avoided when camera, time-step, scale, or cursor changes originate from a
/// linked dependent view.
//==================================================================================================
class RimViewLinker : public RimNamedObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes an empty linker with no master or dependent views.
    RimViewLinker();
    /// Removes dependent-view overrides and destroys owned controllers.
    ~RimViewLinker() override;

    /// @return Whether the enclosing linker collection is active.
    bool isActive() const;

    /// Sets the non-owning master view and refreshes dependent controls.
    void       setMasterView( Rim3dView* view );
    /// @return Current non-owning master view.
    Rim3dView* masterView() const;
    /// Creates an owned controller for @p view when it is eligible.
    void       addDependentView( Rim3dView* view );
    /// @return Whether @p firstView is controlled as dependent on @p secondView.
    bool       isFirstViewDependentOnSecondView( const Rim3dView* firstView, const Rim3dView* secondView ) const;
    /// Reapplies enabled synchronization policies to every dependent view.
    void       updateDependentViews();
    /// Detaches and deletes @p viewController from the owned controller array.
    void       removeViewController( RimViewController* viewController );
    /// @return First valid dependent view, or `nullptr` if none is controlled.
    Rim3dView* firstControlledView();

    /// @name Group-wide refresh operations
    /// @{
    void updateOverrides();
    void updateWindowTitles();
    void updateDuplicatedPropertyFilters();
    /// @}

    /// @name State propagation from a linked source view
    /// @{
    void updateCamera( Rim3dView* sourceView );
    void updateTimeStep( Rim3dView* sourceView, int timeStep );
    void updateScaleZ( Rim3dView* sourceView, double scaleZ );
    /// @}

    /// Propagates the master's active result and coloring configuration.
    void updateCellResult();

    /// Updates dependent visibility after a master cell filter changes.
    void updateCellFilters( const RimCellFilter* changedFilter );
    /// Resolves how original dependent cell filters are handled when linking changes.
    void applyCellFilterCollectionByUserChoice();

    /// Propagates or duplicates a changed master property filter.
    void updatePropertyFilters( RimPropertyFilter* changedPropertyFilter );

    /// Schedules @p geometryType regeneration for eligible dependent views.
    void scheduleGeometryRegenForDepViews( RivCellSetEnum geometryType );
    /// Schedules full display-model recreation for every dependent view.
    void scheduleCreateDisplayModelAndRedrawForDependentViews();

    /// @return Master followed by every valid dependent view.
    std::vector<Rim3dView*> allViews() const;

    /// Refreshes the linker tree label and icon from its master view.
    void updateUiNameAndIcon();

    /// Appends owned controllers to @p uiTreeOrdering.
    void addViewControllers( caf::PdmUiTreeOrdering& uiTreeOrdering ) const;

    /// Derives a display name and icon from @p view when it is valid.
    static void findNameAndIconFromView( QString* name, caf::IconProvider* icon, Rim3dView* view );

    /// Displays @p domainCoord in dependent views configured to show a linked cursor.
    void updateCursorPosition( const Rim3dView* sourceView, const cvf::Vec3d& domainCoord );

protected:
    /// Restores controller presentation and synchronization after deserialization.
    void initAfterRead() override;

    /// Supplies eligible master-view choices.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;
    /// Clears view references and overrides when an owned controller is deleted.
    void onChildDeleted( caf::PdmChildArrayFieldHandle* childArray, std::vector<caf::PdmObjectHandle*>& referringObjects ) override;

private:
    static QString displayNameForView( Rim3dView* view );

    void allViewsForCameraSync( const Rim3dView* source, std::vector<Rim3dView*>& views ) const;

    void removeOverrides();
    void updateScaleWidgetVisibility();

private:
    /// Owned synchronization policy for each dependent view.
    caf::PdmChildArrayField<RimViewController*> m_viewControllers;
    /// Non-owning view that supplies linked state.
    caf::PdmPtrField<Rim3dView*>                m_masterView;
};
