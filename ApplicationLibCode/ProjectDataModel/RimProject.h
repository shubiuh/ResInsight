/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Root PDM document for a ResInsight project and its persistent object graph.
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

#pragma once

#include "RiaDefines.h"
#include "RiaPlotDefines.h"

#include "cafPdmChildArrayField.h"
#include "cafPdmChildField.h"
#include "cafPdmDocument.h"
#include "cvfCollection.h"

#include <vector>

class RigEclipseCaseData;
class RigGridManager;
class RigMainGrid;
class RigWellPath;

class RimTextAnnotation;
class RimReachCircleAnnotation;
class RimPolylinesAnnotation;
class RimSummaryCalculationCollection;
class RimSummaryCalculation;
class RimCase;
class RimDialogData;
class RimEclipseCase;
class RimGeoMechCase;
class RimIdenticalGridCaseGroup;
class RimMainPlotCollection;
class RimReservoirGridEnsemble;
class RimMeasurement;
class RimAdvancedSnapshotExportDefinition;
class RimObservedSummaryData;
class RimOilField;
class RimColorLegendCollection;
class RimScriptCollection;
class RimSummaryCase;
class RimSummaryEnsemble;
class RimSummaryCaseMainCollection;
class Rim3dView;
class RimGridView;
class RimPlotWindow;
class RimViewLinker;
class RimViewLinkerCollection;
class RimViewWindow;
class RimWellPath;
class RimFractureTemplateCollection;
class RimFractureTemplate;
class RimValveTemplateCollection;
class RimValveTemplate;
class RimCompletionTemplateCollection;
class RimPlotTemplateFolderItem;
class RimGridCalculationCollection;
class RimQuickAccessCollection;
class RimAutomationSettings;
class RimJobCollection;
class RimEnsembleFileSetCollection;

namespace caf
{
class PdmUiTreeOrdering;
}

class QAction;
class QMenu;

//==================================================================================================
/// @brief Owns the complete persistent ResInsight project model and coordinates project-wide queries.
///
/// RimProject is the serialization root for oil fields, plots, scripts, calculations, view linking,
/// automation, jobs, and UI restoration state. It also assigns stable numeric identifiers to cases,
/// views, plots, and ensembles and provides cross-cutting lookup helpers used by commands and the
/// application layer. Child and child-array PDM fields own their objects; pointers returned by
/// accessors are borrowed and remain valid only while the project keeps the corresponding child.
///
/// Loading and saving use PDM lifecycle hooks to migrate legacy fields, rebuild derived state, and
/// restrict exported fields. Call updatesAfterProjectFileIsRead() after external data has been loaded
/// so dependent editors, names, and display models can be synchronized.
//==================================================================================================
class RimProject : public caf::PdmDocument
{
    CAF_PDM_HEADER_INIT;

public:
    /// Constructs an empty project and all mandatory owned collections.
    RimProject();
    /// Closes project resources and destroys the owned PDM object graph.
    ~RimProject() override;
    /// @return The project owned by the active application, or `nullptr` when none is installed.
    static RimProject* current();

    /// Owned oil-field roots; the first entry is treated as the active oil field.
    caf::PdmChildArrayField<RimOilField*>                oilFields;
    /// Project-wide color legends shared by result configurations.
    caf::PdmChildField<RimColorLegendCollection*>        colorLegendCollection;
    /// Root of user-visible calculation and automation scripts.
    caf::PdmChildField<RimScriptCollection*>             scriptCollection;
    /// View-link definitions coordinating cameras and result state.
    caf::PdmChildField<RimViewLinkerCollection*>         viewLinkerCollection;
    /// Project-wide summary calculation definitions.
    caf::PdmChildField<RimSummaryCalculationCollection*> calculationCollection;
    /// Project-wide grid calculation definitions.
    caf::PdmChildField<RimGridCalculationCollection*>    gridCalculationCollection;

    /// @return Owned root collection containing all plot windows and plot types.
    RimMainPlotCollection* mainPlotCollection() const;

    /// Owned reusable definitions for multi-view snapshot export.
    caf::PdmChildArrayField<RimAdvancedSnapshotExportDefinition*> multiSnapshotDefinitions;

    /// Serialized expansion and selection state for the main-window project tree.
    caf::PdmField<QString> mainWindowTreeViewStates;
    /// Serialized current-index paths for the main-window project tree.
    caf::PdmField<QString> mainWindowCurrentModelIndexPaths;

    /// Serialized expansion and selection state for the plot-window project tree.
    caf::PdmField<QString> plotWindowTreeViewStates;
    /// Serialized current-index paths for the plot-window project tree.
    caf::PdmField<QString> plotWindowCurrentModelIndexPaths;

    /// Serialized Qt dock layout for the main 3D window.
    caf::PdmField<QString> mainWindowDockState;
    /// Serialized Qt dock layout for the plot window.
    caf::PdmField<QString> plotWindowDockState;

    /// Serializes the project to its configured document filename.
    /// @return `true` when the project file was written successfully.
    bool writeProjectFile();

    /// Rebuilds the script tree from serialized @p scriptDirectories up to @p maxFolderDepth.
    void setScriptDirectories( const QString& scriptDirectories, int maxFolderDepth );
    /// Rebuilds the plot-template tree from @p plotTemplateFolders.
    void setPlotTemplateFolders( const QStringList& plotTemplateFolders );

    /// @return Version recorded when the current project file was written.
    QString projectFileVersionString() const;
    /// Compares the loaded project version using ResInsight's version ordering rules.
    bool    isProjectFileVersionEqualOrOlderThan( const QString& otherProjectFileVersion ) const;
    /// Releases loaded case data and resets the document to a closed state.
    void    close();

    /// Changes the project filename and updates project-relative file dependencies.
    void setProjectFileNameAndUpdateDependencies( const QString& projectFileName );

    /// @name Stable identifier assignment
    /// Assigns a new identifier only when required, avoiding collisions within the relevant project scope.
    /// @{
    void assignCaseIdToCase( RimCase* reservoirCase );
    void assignIdToCaseGroup( RimIdenticalGridCaseGroup* caseGroup );
    void assignIdToCaseGroup( RimReservoirGridEnsemble* gridEnsemble );
    void assignViewIdToView( Rim3dView* view );
    void assignPlotIdToPlotWindow( RimPlotWindow* plotWindow );
    void assignCaseIdToSummaryCase( RimSummaryCase* summaryCase );
    void assignIdToEnsemble( RimSummaryEnsemble* summaryCaseCollection );
    /// @}

    /// @return Every Eclipse and geo-mechanical grid case in the project.
    [[nodiscard]] std::vector<RimCase*> allGridCases() const;

    /// @return Every individual summary case, including cases organized in ensembles.
    std::vector<RimSummaryCase*>     allSummaryCases() const;
    /// @return Every summary ensemble in the project.
    std::vector<RimSummaryEnsemble*> summaryEnsembles() const;

    /// @return Every project 3D view, including specialized views.
    [[nodiscard]] std::vector<Rim3dView*>   allViews() const;
    /// @return Views currently configured to be visible.
    [[nodiscard]] std::vector<Rim3dView*>   allVisibleViews() const;
    /// @return Visible views that derive from RimGridView.
    [[nodiscard]] std::vector<RimGridView*> allVisibleGridViews() const;
    /// @return Views that are not controlled by a RimViewLinker.
    [[nodiscard]] std::vector<Rim3dView*>   allNotLinkedViews() const;

    /// Schedules display-model recreation and redraw for every project view.
    void scheduleCreateDisplayModelAndRedrawAllViews();

    /// @return All owned oil fields in stored order.
    [[nodiscard]] std::vector<RimOilField*> allOilFields() const;
    /// @return Mutable first/active oil field, or `nullptr` for an empty project.
    RimOilField*                            activeOilField();
    /// @return Const first/active oil field, or `nullptr` for an empty project.
    const RimOilField*                      activeOilField() const;

    /// Populates @p contextMenu with commands applicable to the current selection.
    void actionsBasedOnSelection( QMenu& contextMenu );

    /// @name Persisted top-level window visibility
    /// @{
    bool show3DWindow() const;
    bool showPlotWindow() const;
    bool showPlotWindowOnTop() const;
    /// @}

    /// Invalidates and reloads completion-type results for every applicable view.
    void reloadCompletionTypeResultsInAllViews();
    /// Invalidates and reloads completion-type results using @p eclipseCase.
    void reloadCompletionTypeResultsForEclipseCase( RimEclipseCase* eclipseCase );

    /// @return Owned transient dialog data shared by project commands.
    RimDialogData* dialogData() const;

    /// @return Every Eclipse case in the project.
    std::vector<RimEclipseCase*> eclipseCases() const;
    /// Finds an Eclipse case whose canonical grid filename matches @p gridFileName.
    RimEclipseCase*              eclipseCaseFromGridFileName( const QString& gridFileName ) const;
    /// Finds an Eclipse case by its stable project identifier.
    RimEclipseCase*              eclipseCaseFromCaseId( const int caseId ) const;

    /// @return Unique simulation-well names collected from all Eclipse cases.
    std::vector<QString> simulationWellNames() const;

    /// Finds a well path associated with a simulation well and optional branch.
    RimWellPath*                           wellPathFromSimWellName( const QString& simWellName, int branchIndex = -1 );
    /// Finds a well path by its displayed name.
    RimWellPath*                           wellPathByName( const QString& wellPathName ) const;
    /// @return Every project well path.
    std::vector<RimWellPath*>              allWellPaths() const;
    /// @return Every text annotation owned by project annotation collections.
    std::vector<RimTextAnnotation*>        textAnnotations() const;
    /// @return Every reach-circle annotation owned by project annotation collections.
    std::vector<RimReachCircleAnnotation*> reachCircleAnnotations() const;

    /// @return Every geo-mechanical case in the project.
    std::vector<RimGeoMechCase*> geoMechCases() const;

    /// @name Completion and fracture template queries
    /// @{
    std::vector<RimFractureTemplateCollection*> allFractureTemplateCollections() const;
    std::vector<RimFractureTemplate*>           allFractureTemplates() const;

    std::vector<RimValveTemplateCollection*> allValveTemplateCollections() const;
    std::vector<RimValveTemplate*>           allValveTemplates() const;
    /// @}

    /// @return The common Eclipse unit system, or the enum's undefined state for mixed units.
    caf::AppEnum<RiaDefines::EclipseUnitSystem> commonUnitSystemForAllCases() const;
    /// @return Active oil field's owned interactive measurement model.
    RimMeasurement*                             measurement() const;

    /// @return Synthetic root of the discovered plot-template folder tree.
    RimPlotTemplateFolderItem* rootPlotTemplateItem() const;

    /// @return Addresses of all persistent file-path values in the project graph.
    std::vector<caf::FilePath*> allFilePaths() const;

    /// Rebuilds derived state and relationships after project deserialization and data loading.
    void updatesAfterProjectFileIsRead();

    /// @name Owned project service collections
    /// @{
    RimQuickAccessCollection*     pinnedFieldCollection() const;
    RimAutomationSettings*        automationSettings() const;
    RimEnsembleFileSetCollection* ensembleFileSetCollection() const;
    RimJobCollection*             jobCollection() const;
    /// @}

protected:
    /// Migrates legacy serialized fields and restores mandatory children after XML reading.
    void                              initAfterRead() override;
    /// Captures version and derived persistent state immediately before serialization.
    void                              setupBeforeSave() override;
    /// @return Explicit field set included in project export operations.
    std::vector<caf::PdmFieldHandle*> fieldsForExport() const override;

    /// Defines the stable top-level hierarchy shown in project trees.
    void defineUiTreeOrdering( caf::PdmUiTreeOrdering& uiTreeOrdering, QString uiConfigName = "" ) override;

private:
    /// Owned root of all plot model objects.
    caf::PdmChildField<RimMainPlotCollection*>        m_mainPlotCollection;
    /// Owned project-level Quick Access definitions.
    caf::PdmChildField<RimQuickAccessCollection*>     m_pinnedFieldCollection;
    /// Owned automation configuration.
    caf::PdmChildField<RimAutomationSettings*>        m_automationSettings;
    /// Owned ensemble file-set definitions.
    caf::PdmChildField<RimEnsembleFileSetCollection*> m_ensembleFileSetCollection;
    /// Owned external and OPM job definitions.
    caf::PdmChildField<RimJobCollection*>             m_jobCollection;

    /// Serialized collection of paths used by legacy relocation logic.
    caf::PdmField<QString> m_globalPathList;
    /// ResInsight version written into the project file.
    caf::PdmField<QString> m_projectFileVersionString;

    /// Owned command-dialog working data.
    caf::PdmChildField<RimDialogData*>             m_dialogData;
    /// Owned synthetic root of discovered plot-template folders.
    caf::PdmChildField<RimPlotTemplateFolderItem*> m_plotTemplateTopFolder;

    /// Restore the main 3D window when the project is opened.
    caf::PdmField<bool> m_show3DWindow;
    /// Restore the plot window when the project is opened.
    caf::PdmField<bool> m_showPlotWindow;
    /// Keep the plot window above the 3D window after restoration.
    caf::PdmField<bool> m_showPlotWindowOnTopOf3DWindow;

    caf::PdmChildArrayField<RimEclipseCase*>            casesObsolete;      ///< Legacy case storage retained for migration.
    caf::PdmChildArrayField<RimIdenticalGridCaseGroup*> caseGroupsObsolete; ///< Legacy group storage retained for migration.
};
