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
/// @brief Shared enum and constant definitions used across ResInsight modules.

#pragma once

#include "enum_bitmask.hpp"

#include <QString>
#include <vector>

namespace RiaDefines
{
/// Unit conventions encoded by an Eclipse reservoir model.
enum class EclipseUnitSystem
{
    UNITS_METRIC,
    UNITS_FIELD,
    UNITS_LAB,
    UNITS_UNKNOWN,
};

/// Origin or semantic family of a cell result in the result hierarchy.
enum class ResultCatType
{
    DYNAMIC_NATIVE,
    STATIC_NATIVE,
    SOURSIMRL,
    GENERATED,
    INPUT_PROPERTY,
    FORMATION_NAMES,
    ALLAN_DIAGRAMS,
    FLOW_DIAGNOSTICS,
    INJECTION_FLOODING,
    REMOVED,

    UNDEFINED = 999
};

/// Scalar storage type used when interpreting imported result arrays.
enum class ResultDataType
{
    UNKNOWN,
    FLOAT,
    DOUBLE,
    INTEGER
};

/// Components that may be attached to a modeled well path.
/// @warning Enumerator order is consumed by property-filter logic and is serialization-sensitive.
enum class WellPathComponentType
{
    // Production Tube
    WELL_PATH,
    // Well path flow completions
    PERFORATION_INTERVAL,
    FISHBONES,
    FRACTURE,
    ICD,
    AICD,
    ICV,
    // Well path construction features
    CASING,
    LINER,
    PACKER,
    // MSW segments from WELSEGS
    MSW_SEGMENT,
    // More valves
    SICD,
    UNDEFINED_COMPONENT
};

/// Amount of grid-edge geometry drawn in a 3D view.
enum class MeshModeType
{
    FULL_MESH,
    FAULTS_MESH,
    NO_MESH
};

/// @name Built-in mock-model identifiers
/// Stable names understood by the synthetic case generators.
/// @{
QString mockModelBasic();
QString mockModelBasicWithResults();
QString mockModelLargeWithResults();
QString mockModelCustomized();
QString mockModelBasicInputCase();
/// @}

// Units and conversions
enum class DepthUnitType
{
    UNIT_METER,
    UNIT_FEET,
    UNIT_NONE
};

/// Converts reservoir-model units to their corresponding depth unit.
DepthUnitType fromEclipseUnit( EclipseUnitSystem eclipseUnit );
/// Infers an Eclipse unit convention from a depth unit when possible.
EclipseUnitSystem fromDepthUnit( DepthUnitType depthUnit );

// Depth types used for well log plots
/// Supported vertical coordinates for well-log tracks.
enum class DepthType
{
    MEASURED_DEPTH,
    TRUE_VERTICAL_DEPTH,
    PSEUDO_LENGTH,
    CONNECTION_NUMBER,
    TRUE_VERTICAL_DEPTH_RKB
};

/// Reservoir-fluid phases used for results, wells, and curve styling.
enum class PhaseType
{
    OIL_PHASE,
    GAS_PHASE,
    WATER_PHASE,
    PHASE_NOT_APPLICABLE
};

/// Bit mask describing one or more file families accepted by an import operation.
enum class ImportFileType : uint32_t
{
    NOT_A_VALID_IMPORT_FILE = 1 << 0,
    ECLIPSE_GRID_FILE       = 1 << 1,
    ECLIPSE_EGRID_FILE      = 1 << 2,
    ECLIPSE_INPUT_FILE      = 1 << 3,
    ECLIPSE_SUMMARY_FILE    = 1 << 4,
    GEOMECH_ODB_FILE        = 1 << 5,
    RESINSIGHT_PROJECT_FILE = 1 << 6,
    GEOMECH_INP_FILE        = 1 << 7,
    EM_H5GRID               = 1 << 8,
    ROFF_FILE               = 1 << 9,
    GEOMECH_VTK_FILE        = 1 << 10,
    VTK_GRID_FILE           = 1 << 11,
    ECLIPSE_RESULT_GRID     = ECLIPSE_GRID_FILE | ECLIPSE_EGRID_FILE,
    ANY_ECLIPSE_FILE        = ECLIPSE_RESULT_GRID | ECLIPSE_INPUT_FILE | ECLIPSE_SUMMARY_FILE | ROFF_FILE | EM_H5GRID,
    ANY_GEOMECH_FILE        = GEOMECH_ODB_FILE | GEOMECH_INP_FILE | GEOMECH_VTK_FILE,
    ANY_IMPORT_FILE         = ANY_ECLIPSE_FILE | ANY_GEOMECH_FILE | RESINSIGHT_PROJECT_FILE
};

/// @return Whether @p fileType contains a supported geo-mechanical format.
bool isGeoMechFileType( ImportFileType fileType );
/// @return Whether @p fileType contains an Eclipse result-grid format.
bool isEclipseResultFileType( ImportFileType fileType );
/// @return Whether @p fileType contains any Eclipse-related format.
bool isEclipseFileType( ImportFileType fileType );

/// Detects an import-file category from the extension and recognized filename conventions.
ImportFileType obtainFileTypeFromFileName( const QString& fileName );
/// Produces a user-facing directory label for the requested file-type mask.
QString        defaultDirectoryLabel( ImportFileType fileTypes );

/// Project case kinds used when dispatching imports and creating case objects.
enum class CaseType
{
    UNDEFINED_CASE      = -1,
    ECLIPSE_RESULT_CASE = 1,
    ECLIPSE_INPUT_CASE  = 2,
    ECLIPSE_STAT_CASE   = 3,
    ECLIPSE_SOURCE_CASE = 4,
    GEOMECH_ODB_CASE    = 5
};

/// Roles whose font sizes can be configured independently.
enum class FontSettingType
{
    SCENE_FONT,
    ANNOTATION_FONT,
    WELL_LABEL_FONT,
    PLOT_FONT
};

/// Logical index axes in a structured reservoir grid.
enum class GridCaseAxis
{
    AXIS_I,
    AXIS_J,
    AXIS_K,
    UNDEFINED_AXIS
};

/// Available backends for reading reservoir-grid data.
enum class GridModelReader
{
    RESDATA,
    OPM_COMMON,
    NOT_SET
};

/// Color themes supported by the desktop UI.
enum class ThemeEnum
{
    DEFAULT,
    DARK,
    UNDEFINED
};

/// Mouse navigation conventions offered by the 3D viewer.
enum class RINavigationPolicy : short
{
    NAVIGATION_POLICY_CEETRON,
    NAVIGATION_POLICY_CAD,
    NAVIGATION_POLICY_GEOQUEST,
    NAVIGATION_POLICY_RMS
};

/// Operational role assigned to a simulation well.
enum class WellProductionType : short
{
    PRODUCER,
    OIL_INJECTOR,
    GAS_INJECTOR,
    WATER_INJECTOR,
    UNDEFINED_PRODUCTION_TYPE
};

/// @return Whether the production type represents any injector role.
bool isInjector( WellProductionType wellProductionType );

/// @return Delimiter used to serialize multiple strings into legacy scalar settings fields.
QString stringListSeparator();

/// Number of columns used by a multi-plot layout; the large sentinel means no limit.
enum class ColumnCount
{
    COLUMNS_1         = 1,
    COLUMNS_2         = 2,
    COLUMNS_3         = 3,
    COLUMNS_4         = 4,
    COLUMNS_UNLIMITED = 1000,
};

/// Number of plot rows placed on each multi-plot page.
enum class RowCount
{
    ROWS_1 = 1,
    ROWS_2 = 2,
    ROWS_3 = 3,
    ROWS_4 = 4,
};

/// Bit mask identifying which portions of a multi-plot page require regeneration.
enum class MultiPlotPageUpdateType : uint32_t
{
    NONE   = 0b00000000,
    LEGEND = 0b00000001,
    PLOT   = 0b00000010,
    TITLE  = 0b00000100,
    ALL    = 0b00000111
};

/// @name Multi-plot update-mask queries
/// @{
bool isFullUpdate( MultiPlotPageUpdateType updateType );
bool isLegendUpdate( MultiPlotPageUpdateType updateType );
bool isTitleUpdate( MultiPlotPageUpdateType updateType );
bool isPlotUpdate( MultiPlotPageUpdateType updateType );
/// @}

/// @return Standard vertical-scale choices offered by 3D view editors.
std::vector<double> viewScaleOptions();

/// Bit mask describing the data families displayed in a 3D view.
enum class View3dContent
{
    NONE              = 0b00000000,
    ECLIPSE_DATA      = 0b00000001,
    GEOMECH_DATA      = 0b00000010,
    FLAT_INTERSECTION = 0b00000100,
    CONTOUR           = 0b00001000,
    SEISMIC           = 0b00010000,
    ALL               = 0b00011111
};

/// Bit mask for auxiliary object categories displayed in a 3D view.
enum class ItemIn3dView
{
    NONE    = 0b00000000,
    SURFACE = 0b00000001,
    POLYGON = 0b00000010,
    ALL     = 0b00000011
};

/// @return UI postfix used to identify features that are still in beta.
QString betaFeaturePostfix();
}; // namespace RiaDefines

// Activate bit mask operators at global scope
ENABLE_BITMASK_OPERATORS( RiaDefines::MultiPlotPageUpdateType )
ENABLE_BITMASK_OPERATORS( RiaDefines::View3dContent )
ENABLE_BITMASK_OPERATORS( RiaDefines::ItemIn3dView )
ENABLE_BITMASK_OPERATORS( RiaDefines::ImportFileType )
