/////////////////////////////////////////////////////////////////////////////////
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
/// @file
/// @brief Abstract persistent model and runtime bridge for a ResInsight 3D view.
#pragma once

#include "RiaDefines.h"

#include "RimNameConfig.h"
#include "RimViewWindow.h"

#include "RiuViewerToViewInterface.h"

#include "RivAnnotationTools.h"
#include "RivCellSetEnum.h"

#include "cafAppEnum.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPtrField.h"
#include "cafSignal.h"

#include "cafFontTools.h"
#include "cafPdmFieldCvfColor.h"
#include "cafPdmFieldCvfMat4d.h"
#include "cafPdmFieldCvfVec3d.h"

#include "cvfCollection.h"
#include "cvfObject.h"

#include <QPointer>
#include <QTimer>

class RimCase;
class RimLegendConfig;
class RimWellPathCollection;
class RimAnnotationInViewCollection;
class RiuViewer;
class RivAnnotationsPartMgr;
class RivMeasurementPartMgr;
class RivWellPathsPartMgr;
class RiuMainWindowBase;
class RimViewNameConfig;

namespace cvf
{
class BoundingBox;
class ModelBasicList;
class Scene;
class String;
class Transform;
class Part;
} // namespace cvf

namespace caf
{
class DisplayCoordTransform;
}

/// Scene-part masks used to enable surface, mesh, fault, and intersection rendering independently.
enum PartRenderMaskEnum
{
    surfaceBit               = 1,
    meshSurfaceBit           = 2,
    faultBit                 = 4,
    meshFaultBit             = 8,
    intersectionCellFaceBit  = 16,
    intersectionCellMeshBit  = 32,
    intersectionFaultMeshBit = 64
};

//==================================================================================================
/// @brief Coordinates persistent view settings, runtime viewer state, and generated scene models.
///
/// Rim3dView is the common base for reservoir, geo-mechanical, contour, and specialized 3D views.
/// It owns persistent camera, draw-style, naming, annotation, scaling, comparison-view, and animation
/// settings while concrete subclasses create domain-specific geometry, visibility, colors, legends,
/// and time-step data. RiuViewer pointers and cvf render models are runtime-only and non-persistent.
///
/// Display updates are normally scheduled through RiaViewRedrawScheduler. Comparison views may share
/// an override viewer, so an explicit recursion guard prevents two mutually referencing views from
/// repeatedly updating one another.
//==================================================================================================
class Rim3dView : public RimViewWindow, public RiuViewerToViewInterface, public RimNameConfigHolderInterface
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes persistent fields, transient render models, annotations, and animation timing.
    Rim3dView();
    /// Stops animation, detaches runtime viewers, and releases transient scene resources.
    ~Rim3dView() override;

    /// @return Stable project-local view identifier.
    int id() const final;

    /// Selects perspective instead of orthographic projection.
    caf::PdmField<bool> isPerspectiveView;
    /// Upper frame-rate limit applied during animation and interaction.
    caf::PdmField<int>  maximumFrameRate;

    // Draw style

    /// Controls which filled cell faces are rendered.
    enum SurfaceModeType
    {
        SURFACE,
        FAULTS,
        NO_SURFACE
    };

    /// Controls grid-line rendering for cells and faults.
    caf::PdmField<caf::AppEnum<RiaDefines::MeshModeType>> meshMode;
    /// Controls filled surface rendering for cells and faults.
    caf::PdmField<caf::AppEnum<SurfaceModeType>>          surfaceMode;

    /// @return Non-owned case supplying data to this view.
    virtual RimCase* ownerCase() const = 0;
    /// @return Native viewer, or `nullptr` before its widget is created.
    RiuViewer*       viewer() const;

    /// @name View naming
    /// @{
    void               setName( const QString& name );
    QString            name() const;
    QString            autoName() const;
    RimViewNameConfig* nameConfig() const;
    /// @}

    /// @return Bit mask describing the domain content rendered by this view.
    virtual RiaDefines::View3dContent viewContent() const = 0;

    /// @name Draw-style presets
    /// @{
    void           setMeshOnlyDrawstyle();
    void           setMeshSurfDrawstyle();
    void           setSurfOnlyDrawstyle();
    void           setFaultMeshSurfDrawstyle();
    void           setSurfaceDrawstyle();
    void           setShowGridBox( bool showGridBox );
    /// @}
    /// @return Whether the effective view restricts display to active cells.
    virtual bool   isShowingActiveCellsOnly();
    /// @return Whether grid-cell geometry is currently enabled.
    virtual bool   isGridVisualizationMode() const = 0;
    /// @return Representative cell size used for visual tolerances and line widths.
    virtual double characteristicCellSize() const;

    /// Sets the viewer clear color and schedules the necessary refresh.
    void         setBackgroundColor( const cvf::Color3f& newBackgroundColor );
    /// @return Current viewer clear color.
    cvf::Color3f backgroundColor() const override; // Implementation of RiuViewerToViewInterface

    /// @return Effective scene font size in points.
    int  fontSize() const override;
    /// Recreates font-dependent viewer resources.
    void updateFonts() override;
    /// Applies font changes to annotations, legends, and overlays.
    void applyFontChanges();

    /// Enables or disables scene lighting.
    void disableLighting( bool disable );
    /// @return Whether scene lighting is disabled.
    bool isLightingDisabled() const;

    /// @return Whether this view currently colors or annotates using formations.
    virtual bool                          isUsingFormationNames() const = 0;
    /// @return Coordinate transform incorporating domain offset and vertical scale.
    cvf::ref<caf::DisplayCoordTransform>  displayCoordTransform() const override;
    /// @return Legend configurations currently used by the view.
    virtual std::vector<RimLegendConfig*> legendConfigs() const = 0;

    /// Captures the view at an explicit pixel size.
    QImage captureSnapshot( int width, int height ) override;
    /// Captures the current view-widget content.
    QImage snapshotWindowContent() override;
    /// Frames all visible geometry in the viewer.
    void   zoomAll() override;
    /// Ensures the containing top-level window and this view are visible.
    void   forceShowWindowOn();

    /// @name Time-step control
    /// @{
    int                 currentTimeStep() const;
    void                setCurrentTimeStep( int frameIdx );
    void                setCurrentTimeStepAndUpdate( int frameIdx ) override;
    bool                isTimeStepDependentDataVisibleInThisOrComparisonView() const;
    size_t              timeStepCount();
    QString             timeStepName( int frameIdx ) const override;
    virtual QStringList timeStepStrings() const;
    /// @}

    /// Emitted when animation controls or available frames change.
    caf::Signal<> updateAnimations;
    /// Adds one user of the shared animation timer, starting it on the first request.
    void          requestAnimationTimer();
    /// Removes one timer user, stopping it when the count reaches zero.
    void          releaseAnimationTimer();

    /// Queues full display-model recreation on the application redraw scheduler.
    void         scheduleCreateDisplayModelAndRedraw();
    /// Queues regeneration of concrete geometry category @p geometryType.
    virtual void scheduleGeometryRegen( RivCellSetEnum geometryType ) = 0;

    /// @name Immediate display-model updates
    /// @{
    void createDisplayModelAndRedraw();
    void updateDisplayModelForCurrentTimeStepAndRedraw();
    void createHighlightAndGridBoxDisplayModelAndRedraw();
    void createMeasurementDisplayModelAndRedraw();
    void updateAnnotationItems();
    void resetLegends();
    /// @}

    /// Refreshes the grid-box bounds and labels from current data.
    virtual void             updateGridBoxData();
    /// @return Bounding box in unscaled domain coordinates.
    virtual cvf::BoundingBox domainBoundingBox();

    /// @name Vertical scaling
    /// @{
    void   setScaleZ( double scaleZ );
    void   setScaleZAndUpdate( double scaleZ );
    void   updateScaling();
    void   updateZScaleLabel();
    bool   isScaleZEditable();
    double scaleZ() const;
    /// @}

    /// @return Description of active filters for the viewer overlay.
    virtual QString activeFiltersDisplayText() const;
    /// Updates the filter text displayed in the viewer.
    void            updateFilterLabel();

    /// @name View linking and comparison
    /// @{
    bool                    isMasterView() const;
    Rim3dView*              activeComparisonView() const;
    void                    setComparisonView( Rim3dView* compView );
    std::set<Rim3dView*>    viewsUsingThisAsComparisonView();
    void                    updateWindowTitle() override;
    std::vector<Rim3dView*> validComparisonViews() const;

    RimViewLinker*     assosiatedViewLinker() const override;
    RimViewController* viewController() const override;
    /// @}

    /// Refreshes project-tree items for selected auxiliary object categories.
    virtual void updateViewTreeItems( RiaDefines::ItemIn3dView itemType );

    /// @return Owned per-view annotation-selection collection.
    RimAnnotationInViewCollection* annotationCollection() const;
    /// Mirrors eligible global annotations into this view's local collection.
    void                           synchronizeLocalAnnotationsFromGlobal();

    /// Moves the view widget into the main 3D window.
    void dockInMainWindow();
    /// Moves the view widget into the plot window.
    void dockInPlotWindow();

protected:
    /// Removes the scene model named @p modelName when present.
    static void removeModelByName( cvf::Scene* scene, const cvf::String& modelName );

    /// Restores the concrete view's preferred initial camera.
    virtual void setDefaultView();
    /// @return Live viewer camera matrix, falling back to the persisted value without a viewer.
    cvf::Mat4d   cameraPosition() const;
    /// @return Live viewer point of interest, falling back to the persisted value.
    cvf::Vec3d   cameraPointOfInterest() const;

    /// Hides and disables grid-box configuration for view types that do not support it.
    void disableGridBoxField();
    /// Hides and disables perspective projection for incompatible view types.
    void disablePerspectiveProjectionField();
    /// Applies current field visibility to already-created scene models.
    void updateDisplayModelVisibility();

    /// @return Whether visible 3D well-log curves require per-time-step updates.
    bool hasVisibleTimeStepDependent3dWellLogCurves() const;

    /// @return Active oil field's well-path collection, or `nullptr` when unavailable.
    RimWellPathCollection* wellPathCollection() const;

    /// @name Auxiliary scene-model construction
    /// @{
    void addWellPathsToModel( cvf::ModelBasicList*    wellPathModelBasicList,
                              const cvf::BoundingBox& wellPathClipBoundingBox,
                              double                  characteristicCellSize );
    void addDynamicWellPathsToModel( cvf::ModelBasicList*    wellPathModelBasicList,
                                     const cvf::BoundingBox& wellPathClipBoundingBox,
                                     double                  characteristicCellSize );
    void addAnnotationsToModel( cvf::ModelBasicList* annotationsModel );
    void addMeasurementToModel( cvf::ModelBasicList* measureModel );
    /// @}

    /// @return Comparison override viewer when installed, otherwise the native viewer.
    RiuViewer* nativeOrOverrideViewer() const;
    /// @return Whether rendering currently targets an override comparison viewer.
    bool       isUsingOverrideViewer() const;

    /// Hides comparison selection for view types that cannot participate.
    void hideComparisonViewField();

    /// @name Concrete-view display hooks
    /// @{
    virtual void onUpdateDisplayModelVisibility() {};
    virtual void onClearReservoirCellVisibilitiesIfNecessary() {};
    virtual void onResetLegendsInViewer();
    virtual void onUpdateScaleTransform();

    virtual void   onCreateDisplayModel()                   = 0;
    virtual void   onUpdateDisplayModelForCurrentTimeStep() = 0;
    virtual void   onClampCurrentTimestep()                 = 0;
    virtual size_t onTimeStepCountRequested()               = 0;

    virtual bool isTimeStepDependentDataVisible() const                                            = 0;
    virtual void defineAxisLabels( cvf::String* xLabel, cvf::String* yLabel, cvf::String* zLabel ) = 0;
    virtual void onCreatePartCollectionFromSelection( cvf::Collection<cvf::Part>* parts )          = 0;
    virtual void onUpdateStaticCellColors()                                                        = 0;
    virtual void onUpdateLegends()                                                                 = 0;

    virtual cvf::Transform* scaleTransform() = 0;
    /// @}

    /// Persists and propagates viewer camera changes to linked views.
    void onViewNavigationChanged() override;

    /// Adds view-specific context commands to @p menuBuilder.
    void appendMenuItems( caf::CmdFeatureMenuBuilder& menuBuilder ) const override;

protected:
    /// Uses the configured view name as the generic object description.
    caf::PdmFieldHandle* userDescriptionField() override;
    /// @return Field backing the viewer background-color editor.
    caf::PdmFieldHandle* backgroundColorField();

    /// Applies changed draw style, camera, comparison, scale, annotation, and lighting settings.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;
    /// Arranges general view, rendering, camera, and annotation settings.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;

    /// Supplies eligible comparison views and other runtime-dependent choices.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;

    /// Configures object-editor sizing and presentation attributes.
    void defineObjectEditorAttribute( QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;

    /// Captures the live camera state immediately before project serialization.
    void setupBeforeSave() override;

    /// Applies persistent settings after the runtime viewer widget has been created.
    void     updateViewWidgetAfterCreation() override;
    /// Creates and connects the native RiuViewer widget.
    QWidget* createViewWidget( QWidget* mainWindowParent ) override;

    /// Applies and persists @p cameraPosition.
    void setCameraPosition( const cvf::Mat4d& cameraPosition ) override;
    /// Applies and persists @p cameraPointOfInterest.
    void setCameraPointOfInterest( const cvf::Vec3d& cameraPointOfInterest ) override;

protected:
    /// Current frame index; concrete subclasses define clamping behavior.
    caf::PdmField<int> m_currentTimeStep;

    /// Transient render model for static well-path pipes.
    cvf::ref<cvf::ModelBasicList> m_wellPathPipeVizModel;
    /// Transient render model for seismic geometry.
    cvf::ref<cvf::ModelBasicList> m_seismicVizModel;
    /// Runtime manager that builds well-path parts.
    cvf::ref<RivWellPathsPartMgr> m_wellPathsPartManager;
    /// Transient selection-highlight render model.
    cvf::ref<cvf::ModelBasicList> m_highlightVizModel;
    /// Transient screen-space annotations and overlay render model.
    cvf::ref<cvf::ModelBasicList> m_screenSpaceModel;

    /// Vertical exaggeration factor applied by the display transform.
    caf::PdmField<double> m_scaleZ;

    /// Owned per-view annotation visibility and configuration.
    caf::PdmChildField<RimAnnotationInViewCollection*> m_annotationCollection;

private:
    friend class RimProject;

    /// Assigns a project-local identifier; intended only for RimProject.
    void setId( int id );
    /// Requests an identifier from RimProject when the current value is unassigned.
    void assignIdIfNecessary() final;

    /// Detaches and deletes the runtime viewer widget.
    void     deleteViewWidget() override;
    /// @return Current runtime viewer widget.
    QWidget* viewWidget() override;

    /// Recomputes the displayed name from RimViewNameConfig.
    void performAutoNameUpdate() final;

    /// Stops timer-driven animation and releases its timer request.
    void endAnimation() override;

    /// @return This PDM object for the viewer-to-view interface.
    caf::PdmObjectHandle* implementingPdmObject() override;

    /// @name Private auxiliary-model rebuilds
    /// @{
    void createHighlightAndGridBoxDisplayModel();
    void appendMeasurementToModel();
    void appendAnnotationsToModel();
    void updateScreenSpaceModel();
    /// @}

    /// Installs a non-owned viewer used while rendering as a comparison view.
    void       setOverrideViewer( RiuViewer* overrideViewer );
    /// Prepares the selected comparison view for rendering in the comparison viewer.
    Rim3dView* prepareComparisonView();
    /// Restores a prepared comparison view to its native viewer.
    void       restoreComparisonView();

    /// @return Associated linker only when this view is its master.
    RimViewLinker* viewLinkerIfMasterView() const;

private:
    /// Native viewer created with this view's widget.
    QPointer<RiuViewer> m_viewer;
    /// Optional comparison viewer temporarily used instead of m_viewer.
    QPointer<RiuViewer> m_overrideViewer;
    /// Recursion guard for mutually referencing comparison views.
    bool m_isCallingUpdateDisplayModelForCurrentTimestepAndRedraw;

    /// Stable project-local identifier.
    caf::PdmField<int>                     m_id;
    /// Owned automatic/custom name configuration.
    caf::PdmChildField<RimViewNameConfig*> m_nameConfig;
    /// Persistent lighting-disable switch.
    caf::PdmField<bool>                    m_disableLighting;
    /// Persistent viewer clear color.
    caf::PdmField<cvf::Color3f>            m_backgroundColor;
    /// Persistent grid-box visibility.
    caf::PdmField<bool>                    m_showGridBox;
    /// Persistent vertical-scale label visibility.
    caf::PdmField<bool>                    m_showZScaleLabel;
    /// Non-owning comparison view rendered beside this view.
    caf::PdmPtrField<Rim3dView*>           m_comparisonView;

    // Camera position and point of interest. The member variables are mutable to allow for setting them from const methods.
    // The camera position and point of interest can change rapidly as the user interacts with the 3D view. Only update the Pdm field values
    // when the application requests the camera position or point of interest.
    mutable caf::PdmField<cvf::Mat4d>   m_cameraPosition;
    mutable caf::PdmField<cvf::Vec3d>   m_cameraPointOfInterest;
    /// Editor proxy that reads and writes the live viewer point of interest.
    caf::PdmProxyValueField<cvf::Vec3d> m_cameraPointOfInterestProxy;
    /// Editor proxy that reads and writes the live viewer camera matrix.
    caf::PdmProxyValueField<cvf::Mat4d> m_cameraPositionProxy;

    /// Enables explicit annotation-placement configuration for this view.
    caf::PdmField<bool>                                                    m_useCustomAnnotationStrategy;
    /// Selected annotation-label placement strategy.
    caf::PdmField<caf::AppEnum<RivAnnotationTools::LabelPositionStrategy>> m_annotationStrategy;
    /// Expected annotation count used to tune automatic placement.
    caf::PdmField<int>                                                     m_annotationCountHint;

    /// Scene font size relative to the application default.
    caf::PdmField<caf::FontTools::RelativeSizeEnum> m_fontSize;

    /// Runtime builder for annotation render parts.
    cvf::ref<RivAnnotationsPartMgr> m_annotationsPartManager;
    /// Runtime builder for measurement render parts.
    cvf::ref<RivMeasurementPartMgr> m_measurementPartManager;

    /// Shared timer driving animation while at least one consumer is registered.
    std::unique_ptr<QTimer> m_animationTimer;
    /// Fixed animation-timer interval in milliseconds.
    const int               m_animationIntervalMillisec;
    /// Reference count preventing one animation consumer from stopping another.
    int                     m_animationTimerUsers;
};
