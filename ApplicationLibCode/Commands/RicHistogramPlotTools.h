/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2025-     Equinor ASA
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
/// @brief Declares histogram plot command support.

#pragma once

#include <set>
#include <vector>

#include <QString>

class RimHistogramDataSource;
class RimHistogramPlot;
class RimHistogramMultiPlot;
class RimHistogramMultiPlotCollection;
class RimEnsembleParameterHistogramDataSource;
class RimHistogramCurve;
class RimSummaryEnsemble;

//--------------------------------------------------------------------------------------------------
/// @brief Utility functions for histogram plot.
//--------------------------------------------------------------------------------------------------
class RicHistogramPlotTools
{
public:
    enum class DataSourceType
    {
        ENSEMBLE_PARAMETER,
        GRID_STATISTICS,
        SUMMARY_VECTOR,
        ENSEMBLE_FRACTURE_STATISTICS
    };

    /// @return All data source types.
    static std::vector<DataSourceType> allDataSourceTypes();

    /// Creates default histogram curve.
    static void createDefaultHistogramCurve( RimHistogramPlot* plot, DataSourceType dataSourceType );
    /// Creates histogram curve.
    static void createHistogramCurve( RimHistogramPlot* plot, RimHistogramDataSource* dataSource );
    /// Appends ensemble parameter histogram curve.
    static void appendEnsembleParameterHistogramCurve( RimHistogramPlot* plot, RimEnsembleParameterHistogramDataSource* dataSource );
    /// Adds histogram curve to plot.
    static void addHistogramCurveToPlot( RimHistogramPlot* plot, RimHistogramCurve* curve, bool resolveRefs = false );

    /// Adds new histogram multiplot.
    static RimHistogramMultiPlot* addNewHistogramMultiplot();
    /// Adds new histogram multiplot.
    static RimHistogramMultiPlot* addNewHistogramMultiplot( RimHistogramMultiPlotCollection* collection );
    /// Adds new histogram plot.
    static RimHistogramPlot*      addNewHistogramPlot( RimHistogramMultiPlot* histogramMultiPlot );

    /// Appends ensemble to histogram.
    static void appendEnsembleToHistogram( RimHistogramPlot* plot, RimSummaryEnsemble* ensemble );

    /// @return The existing ensemble parameters.
    static std::set<QString> existingEnsembleParameters( RimHistogramPlot* plot );

private:
    /// @return The existing data sources.
    static std::vector<RimHistogramDataSource*> existingDataSources( RimHistogramPlot* plot );
};
