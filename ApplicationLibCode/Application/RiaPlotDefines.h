/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2022-     Equinor ASA
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
/// @brief Plot-related enumerations, naming helpers, and axis utilities.

#pragma once

class QPaintDevice;
class QString;

// Defines relate to plotting
namespace RiaDefines
{
/// Physical axis location around a Cartesian plot canvas.
enum class PlotAxis
{
    PLOT_AXIS_LEFT,
    PLOT_AXIS_RIGHT,
    PLOT_AXIS_BOTTOM,
    PLOT_AXIS_TOP
};

/// Annotation content rendered behind or alongside a well-log track.
enum class RegionAnnotationType
{
    NO_ANNOTATIONS        = 0,
    FORMATION_ANNOTATIONS = 1,
    // Used to have Wbs-parameter coding as 2
    RESULT_PROPERTY_ANNOTATIONS = 3
};
/// Bit flags controlling line and fill treatment for annotated regions.
enum RegionDisplay
{
    DARK_LINES              = 0x01,
    COLORED_LINES           = 0x02,
    COLOR_SHADING           = 0x04,
    COLOR_SHADING_AND_LINES = 0x05,
    LIGHT_LINES             = 0x08,
};
/// Column span occupied by a track in a multi-column layout.
enum class TrackSpan
{
    FULL_WIDTH,
    LEFT_COLUMN,
    CENTRE_COLUMN,
    RIGHT_COLUMN
};
/// Primary direction in which plot content or layouts are arranged.
enum class Orientation
{
    HORIZONTAL = 0,
    VERTICAL
};

/// Controls whether multi-plots expose one shared axis or all child axes.
enum class MultiPlotAxisVisibility
{
    ONE_VISIBLE,
    ALL_VISIBLE
};

/// Source used to construct an object's displayed name.
enum class ObjectNamingMethod
{
    CUSTOM,
    AUTO,
    TEMPLATE
};

/// Preferred arrangement when child plot windows are tiled.
enum class WindowTileMode
{
    DEFAULT,
    VERTICAL,
    HORIZONTAL,
    UNDEFINED,
};

/// Horizontal alignment used for plot labels and annotations.
enum class TextAlignment
{
    LEFT,
    RIGHT,
    CENTER
};

/// Cursor readout behavior used while interacting with plots.
enum class ReadOutType
{
    NONE,
    SNAP_TO_POINT,
    TIME_TRACKING,
    TIME_VALUE_TRACKING
};

/// @name Plot-name template variables
/// Tokens returned by these helpers are substituted when automatic plot and curve names are built.
/// @{
QString namingVariableCase();
QString namingVariableWell();
QString namingVariableRefWell();
QString namingVariableWellBranch();
QString namingVariableResultName();
QString namingVariableResultType();
QString namingVariableTime();
QString namingVariableTimestep();
QString namingVariableAirGap();
QString namingVariableWaterDepth();
/// @}

/// @return Standard text representing an empty selection.
QString selectionTextNone();

/// @name Default plot ranges
/// @{
double minimumDefaultValuePlot();
double minimumDefaultLogValuePlot();
double maximumDefaultValuePlot();
/// @}

/// @name Axis geometry helpers
/// @{
bool     isHorizontal( PlotAxis axis );
bool     isVertical( PlotAxis axis );
PlotAxis opposite( PlotAxis axis );
/// @}

/// Converts logical pixel sizes to the resolution of @p paintDevice.
double scalingFactor( QPaintDevice* paintDevice );

/// @name Stable project-editor group names
/// @{
QString curveNameGroupName();
QString appearanceGroupName();
QString additionalDataSourcesGroupName();
/// @}

}; // namespace RiaDefines
