/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) Statoil ASA
//  Copyright (C) Ceetron Solutions AS
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
/// @brief Grid import and reading preferences.

#pragma once

#include "cafPdmField.h"
#include "cafPdmObject.h"

#include "RiaDefines.h"
#include "RiaGridDefines.h"

#include <QString>

struct RifReaderSettings;

//==================================================================================================
/// @brief Controls how Eclipse grid models are read and which data they include.
///
/// RiaPreferencesGrid governs the grid model reader backend (OPM_COMMON or RESDATA), fault and
/// NNC import toggles, MSW data import, result index file usage, depth property computation,
/// active-only cell loading, radial-grid interpretation, and long-thin cell invalidation. The
/// persisted reader selection can be temporarily superseded by a command-line override without
/// rewriting user preferences. readerSettings() converts the PDM fields into the lower-level
/// RifReaderSettings consumed by grid importers.
//==================================================================================================
class RiaPreferencesGrid : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

    using GridModelEnum = caf::AppEnum<RiaDefines::GridModelReader>;

public:
    /// Initializes grid import defaults and PDM editor metadata.
    RiaPreferencesGrid();

    /// @return The grid preference object owned by the current application preferences.
    static RiaPreferencesGrid* current();

    /// @return Minimal reader settings intended for geometry-only imports.
    static RifReaderSettings   gridOnlyReaderSettings();

    /// @return Complete reader settings derived from the current preferences and override.
    RifReaderSettings          readerSettings();

    /// @return Whether fault geometry is imported.
    bool    importFaults() const;
    /// @return Whether non-neighbor connections are imported.
    bool    importNNCs() const;
    /// @return Whether inactive cells contribute to generated fault surfaces.
    bool    includeInactiveCellsInFaultGeometry() const;
    /// @return Whether advanced multi-segment-well data is imported.
    bool    importAdvancedMswData() const;
    /// @return Prefix used when resolving absolute paths referenced by INCLUDE statements.
    QString includeFileAbsolutePathPrefix() const;
    /// @return Whether auxiliary result-index files should accelerate result lookup.
    bool    useResultIndexFile() const;
    /// @return Whether well data should be omitted during grid import.
    bool    skipWellData() const;
    /// @return Whether SOIL is loaded and selected for initial display.
    bool    loadAndShowSoil() const;
    /// @return Whether depth-related cell properties are calculated automatically.
    bool    autoComputeDepthRelatedProperties() const;
    /// @return Whether import should retain only active cells.
    bool    onlyLoadActiveCells() const;
    /// @return Whether long, thin cells should be invalidated during import.
    bool    invalidateLongThinCells() const;

    /// @return The effective reader backend after applying any transient override.
    RiaDefines::GridModelReader gridModelReader() const;

    /// Sets a transient reader override parsed from a command-line name.
    void                        setGridModelReaderOverride( const std::string& readerName );

    /// Sets a transient reader override without changing the persisted selection.
    void                        setGridModelReaderOverride( const RiaDefines::GridModelReader readerType );

    /// @return The effective transient override, or the class's no-override sentinel.
    RiaDefines::GridModelReader gridModelReaderOverride() const;

    /// @return The configured radial-grid interpretation mode.
    RiaGridDefines::RadialGridMode radialGridMode() const;

    /// Adds grid fields to the parent preferences editor.
    void appendItems( caf::PdmUiOrdering& uiOrdering );

private:
    caf::PdmField<GridModelEnum> m_gridModelReader;         ///< Persisted grid-reader backend.
    RiaDefines::GridModelReader  m_gridModelReaderOverride; ///< Transient command-line override.

    caf::PdmField<bool>    m_importFaults;                       ///< Import fault topology and geometry.
    caf::PdmField<bool>    m_importNNCs;                         ///< Import non-neighbor connections.
    caf::PdmField<bool>    m_includeInactiveCellsInFaultGeometry;///< Include inactive faces in fault geometry.
    caf::PdmField<bool>    m_importAdvancedMswData;              ///< Load advanced multi-segment-well data.
    caf::PdmField<QString> m_includeFileAbsolutePathPrefix;      ///< Prefix used to resolve absolute INCLUDE paths.
    caf::PdmField<bool>    m_useResultIndexFile;                 ///< Use auxiliary result-index files when available.
    caf::PdmField<bool>    m_skipWellData;                       ///< Skip well-result loading during import.
    caf::PdmField<bool>    m_autoComputeDepthRelatedProperties; ///< Compute depth-derived properties after loading.
    caf::PdmField<bool>    m_loadAndShowSoil;                    ///< Load and initially expose SOIL result data.
    caf::PdmField<bool>    m_onlyLoadActiveCells;                ///< Restrict cell data to active cells.
    caf::PdmField<bool>    m_invalidateLongThinCells;            ///< Reject cells that violate shape criteria.

    /// Interpretation used when importing and displaying radial grids.
    caf::PdmField<caf::AppEnum<RiaGridDefines::RadialGridMode>> m_radialGridMode;
};
