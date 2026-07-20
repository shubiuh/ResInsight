/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Shared project model for Eclipse-format reservoir cases.
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

#include "RiaPorosityModel.h"

#include "RiaDefines.h"
#include "RimCase.h"

#include "RifReaderSettings.h"

#include "cafPdmChildArrayField.h"
#include "cafPdmChildField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"

#include "cvfColor3.h"
#include "cvfObject.h"

#include <memory>
#include <set>

class QString;

class RimWellTargetMapping;
class RimFaultDistanceCollection;
class RimDataAnalyticsCollection;
class RigCaseCellResultsData;
class RigEclipseCaseData;
class RigFormationNames;
class RigGridBase;
class RigMainGrid;
class RigVirtualPerforationTransmissibilities;
class RimCaseCollection;
class RimDataFilterCollection;
class RimEclipseContourMapView;
class RimEclipseContourMapViewCollection;
class RimEclipseInputPropertyCollection;
class RimEclipseView;
class RimIdenticalGridCaseGroup;
class RimReservoirCellResultsStorage;
class RimEclipseResultAddressCollection;
class RimEclipseViewCollection;
class RimResultNameAlias;

namespace caf
{
class CmdFeatureMenuBuilder;
}

/// @brief Connects persistent Eclipse case configuration to loaded RigEclipseCaseData.
///
/// RimEclipseCase owns views, result-storage descriptors, input-property metadata, filters, aliases,
/// and analytics configuration. The heavy grid and result data is retained through a cvf reference
/// and can be opened lazily or released independently of the persistent project object. Concrete
/// subclasses implement the actual grid source, such as simulator result files, input decks, or
/// in-memory corner-point arrays.
class RimEclipseCase : public RimCase
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes persistent collections and empty runtime reservoir data.
    RimEclipseCase();
    /// Closes loaded reservoir resources and releases transient cached data.
    ~RimEclipseCase() override;

    /// @return All Eclipse views associated with this case, including global-view storage.
    std::vector<RimEclipseView*> reservoirViews() const;
    /// @return Case-owned regular view collection.
    RimEclipseViewCollection*    viewCollection() const;

    /// @return External fault files associated with this case.
    std::vector<QString> filesContainingFaults() const;
    /// Replaces the persisted external fault-file list with @p val.
    void                 setFilesContainingFaults( const std::vector<QString>& val );

    /// Ensures runtime reservoir data is available, safely reusing already-open state.
    bool         ensureReservoirCaseIsOpen();
    /// Opens grid data and initializes common results, faults, and derived caches.
    bool         openReservoirCase();
    /// Releases loaded runtime case data while preserving persistent configuration.
    virtual void closeReservoirCase();
    /// Opens the concrete subclass's grid source and installs its RigEclipseCaseData.
    virtual bool openEclipseGridFile() = 0;
    /// Closes and reopens the grid source, then refreshes dependent views.
    virtual void reloadEclipseGridFile();
    /// Imports additional scalar properties from Eclipse ASCII files.
    virtual bool importAsciiInputProperties( const QStringList& fileNames );

    /// @name Runtime reservoir data
    /// @{
    RigEclipseCaseData*       eclipseCaseData();
    const RigEclipseCaseData* eclipseCaseData() const;
    /// @}
    /// Ensures the input deck has been parsed far enough to expose EQUIL data.
    void                      ensureDeckIsParsedForEquilData();
    /// Returns a stable automatically assigned color for @p wellName within this case.
    cvf::Color3f              defaultWellColor( const QString& wellName );

    /// @return Main grid of the loaded case, or `nullptr` while closed.
    const RigMainGrid* mainGrid() const;
    /// @return Whether this and @p otherCase have compatible main-grid dimensions.
    bool               isGridSizeEqualTo( const RimEclipseCase* otherCase ) const;

    /// @name Cell result access by pore system
    /// @{
    RigCaseCellResultsData*       results( RiaDefines::PorosityModelType porosityModel );
    const RigCaseCellResultsData* results( RiaDefines::PorosityModelType porosityModel ) const;

    RimReservoirCellResultsStorage*       resultsStorage( RiaDefines::PorosityModelType porosityModel );
    const RimReservoirCellResultsStorage* resultsStorage( RiaDefines::PorosityModelType porosityModel ) const;
    /// @}

    /// Creates a default view in the case or optional global view collection.
    RimEclipseView* createAndAddReservoirView( bool useGlobalViewCollection = false );
    /// Creates a default view owned by @p viewColl.
    RimEclipseView* createAndAddReservoirView( RimEclipseViewCollection* viewColl );
    /// Deep-copies @p sourceView, assigns a new ID, and adds it to the matching collection.
    RimEclipseView* createCopyAndAddView( const RimEclipseView* sourceView );

    /// Lazily computes completion transmissibilities and returns the case-owned runtime cache.
    const RigVirtualPerforationTransmissibilities* computeAndGetVirtualPerforationTransmissibilities();

    /// @return Directory containing the concrete case source, if it has one.
    virtual QString locationOnDisc() const { return QString(); }

    /// @name Owning and associated collections
    /// @{
    RimCaseCollection*                  parentCaseCollection();
    RimEclipseContourMapViewCollection* contourMapCollection() const;
    RimEclipseInputPropertyCollection*  inputPropertyCollection() const;
    RimDataFilterCollection*            dataFilterCollection() const;
    /// @}

    /// @name RimCase time-step interface
    /// @{
    QStringList            timeStepStrings() const override;
    QString                timeStepName( int frameIdx ) const override;
    std::vector<QDateTime> timeStepDates() const override;
    /// @}

    /// @name RimCase spatial interface
    /// @{
    cvf::BoundingBox reservoirBoundingBox() override;
    cvf::BoundingBox activeCellsBoundingBox() const override;
    cvf::BoundingBox allCellsBoundingBox() const override;
    cvf::Vec3d       displayModelOffset() const override;
    /// @}

    /// @return Representative main-grid cell size.
    double characteristicCellSize() const override;

    /// @return Sorted unique simulation-well names from the loaded case.
    std::set<QString> sortedSimWellNames() const;

    /// Imports configured input properties and aligns them with loaded grid/result metadata.
    void loadAndSynchronizeInputProperties( bool importGridOrFaultData );

    /// Ensures fault topology and geometry have been computed.
    void ensureFaultDataIsComputed();
    /// Ensures non-neighbor connection data is available and reports success.
    bool ensureNncDataIsComputed();
    /// Rebuilds and redraws every associated view.
    void createDisplayModelAndUpdateAllViews();
    /// Computes and caches the active-cell spatial extent.
    void computeActiveCellsBoundingBox();

    /// Replaces reader configuration used on the next open operation.
    void              setReaderSettings( RifReaderSettings& readerSettings );
    /// @return Current reader configuration.
    RifReaderSettings readerSettings() const;

    /// Adds @p wellTargetMapping to the enclosing ensemble that owns mappings.
    void addWellTargetMapping( RimWellTargetMapping* wellTargetMapping );

    /// @return Case-owned fault-distance calculation collection.
    RimFaultDistanceCollection* faultDistanceCollection() const;

    /// Rebuilds project-tree result-address nodes from current result metadata.
    void updateResultAddressCollection();

    /// Installs @p eclipseCase as the loaded runtime data, retaining it through cvf reference counting.
    void setReservoirData( RigEclipseCaseData* eclipseCase );

    /// Adds a user-facing alias for @p resultName.
    void addResultAlias( const QString& resultName, const QString& aliasName );
    /// Removes all result-name aliases.
    void clearResultAliases();

    /// Updates formation results and associated view state from the active definition.
    void updateFormationNamesData() override;

protected:
    /// Migrates legacy views/mappings and restores result-storage relationships after reading.
    void initAfterRead() override;
    /// Handles axis flipping, result release, aliases, and other case-level UI actions.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;
    /// Responds when fields in owned result or filter objects change through the UI.
    void childFieldChangedByUi( const caf::PdmFieldHandle* changedChildField ) override;
    /// Defines the case tree containing views, results, properties, filters, and analytics.
    void defineUiTreeOrdering( caf::PdmUiTreeOrdering& uiTreeOrdering, QString uiConfigName = "" ) override;
    /// Adds Eclipse-case-specific context commands.
    void appendMenuItems( caf::CmdFeatureMenuBuilder& menuBuilder ) const override;

    /// Adds result-alias fields to the property editor.
    void resultAliasUiOrdering( caf::PdmUiOrdering& uiOrdering );

    /// Applies persistent aliases to current runtime result metadata.
    void syncResultAliases();

    // Internal methods
protected:
    /// Computes derived grid, active-cell, and result data after loading.
    void                      computeCachedData();
    /// @return Additional input-property and fault files associated with the case.
    std::vector<QString>      additionalFiles() const;
    /// @return Oil-field-level Eclipse view collection used for global views.
    RimEclipseViewCollection* globalViewCollection() const;
    /// Appends valid views from @p viewColl to @p views.
    void addViewsFromViewCollection( std::vector<RimEclipseView*>& views, const RimEclipseViewCollection* viewColl ) const;

private:
    /// Chooses a date/time format capable of representing all case time steps.
    void                                   createTimeStepFormatString();
    /// @return Eclipse views stored outside the regular case view collection.
    std::vector<Rim3dView*>                allSpecialViews() const override;
    /// @return Contour-map views associated with this case.
    std::vector<RimEclipseContourMapView*> contourMapViews() const;

    /// Recreates owned result-address tree nodes from available matrix and fracture results.
    void buildResultChildNodes();

    /// @return Formation metadata selected by the case or embedded in its runtime data.
    const RigFormationNames* effectiveFormationNames() const;

protected:
    /// Mirrors imported grid geometry along the X axis.
    caf::PdmField<bool>                                    m_flipXAxis;
    /// Mirrors imported grid geometry along the Y axis.
    caf::PdmField<bool>                                    m_flipYAxis;
    /// Owned metadata for imported Eclipse input properties.
    caf::PdmChildField<RimEclipseInputPropertyCollection*> m_inputPropertyCollection;

    /// Owned persistent result-name aliases.
    caf::PdmChildArrayField<RimResultNameAlias*> m_resultAliasList;

    /// Non-PDM reader settings applied when the runtime case is opened.
    RifReaderSettings m_readerSettings;

private:
    /// UI action field used to release loaded result arrays.
    caf::PdmField<bool> m_releaseResultMemory;

    /// Reference-counted loaded grid, wells, faults, and result data.
    cvf::ref<RigEclipseCaseData>    m_rigEclipseCase;
    /// Cached Qt format chosen for displaying case time steps.
    QString                         m_timeStepFormatString;
    /// Per-case stable well-name color assignments.
    std::map<QString, cvf::Color3f> m_wellToColorMap;

    /// Owned project-tree groupings of available result addresses.
    caf::PdmChildArrayField<RimEclipseResultAddressCollection*> m_resultAddressCollections;

    /// Owned persistent matrix-result storage configuration.
    caf::PdmChildField<RimReservoirCellResultsStorage*> m_matrixModelResults;
    /// Owned persistent fracture-result storage configuration.
    caf::PdmChildField<RimReservoirCellResultsStorage*> m_fractureModelResults;
    /// Owned regular Eclipse views for this case.
    caf::PdmChildField<RimEclipseViewCollection*>       m_viewCollection;
    /// Owned case-level filter configuration.
    caf::PdmChildField<RimDataFilterCollection*>        m_dataFilterCollection;

    /// Owned data-analytics definitions associated with this case.
    caf::PdmChildField<RimDataAnalyticsCollection*> m_dataAnalyticsCollection;

    /// Persisted external files from which fault definitions are imported.
    caf::PdmField<std::vector<caf::FilePath>> m_filesContainingFaults;

    /// Legacy direct view storage retained for migration.
    caf::PdmChildArrayField<RimEclipseView*>                m_reservoirViews_OBSOLETE;
    /// Legacy case-local contour maps retained for migration.
    caf::PdmChildField<RimEclipseContourMapViewCollection*> m_contourMapCollection_OBSOLETE;
    /// Legacy case-local target mappings retained for migration.
    caf::PdmChildArrayField<RimWellTargetMapping*>          m_wellTargetMappings_OBSOLETE;
};
