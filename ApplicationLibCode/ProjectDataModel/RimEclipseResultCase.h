/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Eclipse simulator result case with lazy grid, well, RFT, and flow-diagnostics data.
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

#include "RimEclipseCase.h"

#include "cafFilePath.h"
#include "cafPdmChildArrayField.h"
#include "cafPdmProxyValueField.h"

#include <memory>

class RifReaderRftInterface;
class RifReaderOpmRft;
class RifReaderInterface;
class RigFlowDiagSolverInterface;
class RigMainGrid;
class RimEclipseInputProperty;
class RimEclipseInputPropertyCollection;
class RimFlowDiagSolution;
class RimResultNameAlias;

/// @brief Loads an Eclipse result file set and owns derived flow-diagnostics configurations.
///
/// Opening is split into grid/result metadata, active-cell data, and RFT data phases so workflows
/// pay only for the information they use. Boolean guards prevent repeated reader work. Flow solver
/// and RFT reader instances are runtime-owned unique pointers, while flow-diagnostics solutions and
/// file settings are serialized through PDM fields.
class RimEclipseResultCase : public RimEclipseCase
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes persistent result-case fields and unloaded runtime guards.
    RimEclipseResultCase();
    /// Releases readers, solver, and loaded reservoir data.
    ~RimEclipseResultCase() override;

    /// Sets the user-facing description and primary grid/result filename together.
    void setCaseInfo( const QString& userDescription, const QString& fileName );
    /// Sets an optional SourSim coupling result file.
    void setSourSimFileName( const QString& fileName );
    /// @return Whether a non-empty, existing SourSim file is configured.
    bool hasSourSimFile();

    /// Opens grid and result metadata using the configured reader backend.
    bool openEclipseGridFile() override;
    /// Releases result-case runtime data and resets the grid and active-cell loading state.
    void closeReservoirCase() override;

    /// Imports grid/well metadata and optionally presents the time-step filter UI.
    bool importGridAndResultMetaData( bool showTimeStepFilter );
    /// Imports additional ASCII properties into the result grid.
    bool importAsciiInputProperties( const QStringList& fileNames ) override;

    /// Reads active-cell information, optionally sharing grid geometry with @p mainEclipseCase.
    bool openAndReadActiveCellData( RigEclipseCaseData* mainEclipseCase );
    /// Appends dimensions for the main grid and local grids without loading all results.
    void readGridDimensions( std::vector<std::vector<int>>& gridDimensions );

    /// @return Unit convention reported by the loaded case reader.
    caf::AppEnum<RiaDefines::EclipseUnitSystem> unitSystem();

    /// @return Directory containing the primary Eclipse result grid.
    QString locationOnDisc() const override;

    /// @return Default owned flow-diagnostics solution, creating it if necessary.
    RimFlowDiagSolution*              defaultFlowDiagSolution();
    /// @return All owned flow-diagnostics solutions.
    std::vector<RimFlowDiagSolution*> flowDiagSolutions();
    /// @return Lazily created solver interface bound to the loaded case.
    RigFlowDiagSolverInterface*       flowDiagSolverInterface();

    /// @return Lazily initialized RFT/PLT reader for this result file set.
    RifReaderRftInterface* rftReader();

    /// @return Maximum short-branch segment count merged into an upstream multi-segment well branch.
    int mswMergeThreshold() const;

protected:
    /// Reloads dependent SourSim or reader state after result-case fields change.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;
    /// Configures file pickers and optional threshold editors.
    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;
    /// Arranges result-file, unit, time-step, SourSim, and MSW settings.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;

    /// Supplies reader-dependent and file-dependent editor choices.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;
    /// Migrates result-case settings and restores solution relationships after reading.
    void                          initAfterRead() override;

private:
    /// Imports or clears SourSim results and refreshes affected views.
    void loadAndUpdateSourSimData();
    /// Performs the one-time RFT import required before exposing rftReader().
    void ensureRftDataIsImported();
    /// @return Whether time-step metadata warrants displaying the filter dialog.
    bool showTimeStepFilterGUI();

    /// @return Human-readable list of phases present in the loaded case.
    QString phasesAsString() const;

    /// Creates a reader populated with the named built-in synthetic result model.
    cvf::ref<RifReaderInterface> createMockModel( QString modelName );

private:
    /// Lazily created flow-diagnostics solver tied to current runtime case data.
    std::unique_ptr<RigFlowDiagSolverInterface> m_flowDagSolverInterface;

    /// Lazily created OPM RFT reader.
    std::unique_ptr<RifReaderOpmRft> m_readerOpmRft;

    /// Read-only proxy exposing the loaded simulator unit convention.
    caf::PdmProxyValueField<caf::AppEnum<RiaDefines::EclipseUnitSystem>> m_unitSystem;
    /// Owned flow-diagnostics solution configurations.
    caf::PdmChildArrayField<RimFlowDiagSolution*>                        m_flowDiagSolutions;
    /// Optional SourSim coupling-data path.
    caf::PdmField<caf::FilePath>                                         m_sourSimFileName;

    /// Optional threshold controlling short-branch merging for multi-segment wells.
    caf::PdmField<std::pair<bool, int>> m_mswMergeThreshold;

    /// Whether grid and well metadata has been read for the current open cycle.
    bool m_gridAndWellDataIsReadFromFile;
    /// Whether active-cell information has been read for the current open cycle.
    bool m_activeCellInfoIsReadFromFile;
    /// Whether RFT data has been imported for the current open cycle.
    bool m_rftDataIsReadFromFile;
};
