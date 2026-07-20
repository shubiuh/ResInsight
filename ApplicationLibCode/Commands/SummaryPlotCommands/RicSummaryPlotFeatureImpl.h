/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2019-     Equinor ASA
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
/// @brief Declares summary plot command support.
#pragma once

#include <QString>
#include <QStringList>

#include <set>
#include <vector>

class RifEclipseSummaryAddress;

/// @brief Utilities for caf command workflows.
namespace caf
{
class PdmObject;
}

class RimSummaryCurve;
class RimSummaryPlot;
class RimSummaryCase;
class RimSummaryEnsemble;
class RimEnsembleCurveSet;

/// @brief Shared implementation helpers for summary plot commands.
class RicSummaryPlotFeatureImpl
{
public:
    enum class EnsembleColoringType
    {
        SINGLE_COLOR,
        PARAMETER,
        LOG_PARAMETER,
        NONE
    };

    /// Adds default curves to plot.
    static std::vector<RimSummaryCurve*> addDefaultCurvesToPlot( RimSummaryPlot* plot, RimSummaryCase* summaryCase );

    /// Creates summary plots from argument line.
    static void createSummaryPlotsFromArgumentLine( const QStringList& arguments );

    /// Creates summary plot for ensemble.
    static RimSummaryPlot* createSummaryPlotForEnsemble( const std::vector<RimSummaryCase*>& summaryCasesToUse,
                                                         RimSummaryEnsemble*                 ensemble,
                                                         QStringList                         summaryAddressFilters,
                                                         bool                                addHistoryCurves = false,
                                                         EnsembleColoringType ensembleColoringStyle           = EnsembleColoringType::NONE,
                                                         QString              ensembleColoringParameter       = "" );

    /// Creates summary plot for cases.
    static RimSummaryPlot* createSummaryPlotForCases( const std::vector<RimSummaryCase*>& summaryCasesToUse,
                                                      QStringList                         summaryAddressFilters,
                                                      bool                                addHistoryCurves = false );

    /// Creates multiple summary plots from addresses.
    static std::vector<RimSummaryPlot*>
        createMultipleSummaryPlotsFromAddresses( const std::vector<RimSummaryCase*>& summaryCasesToUse,
                                                 RimSummaryEnsemble*                 ensemble,
                                                 QStringList                         summaryAddressFilters,
                                                 bool                                addHistoryCurves          = false,
                                                 EnsembleColoringType                ensembleColoringStyle     = EnsembleColoringType::NONE,
                                                 QString                             ensembleColoringParameter = "" );

    /// Inserts filtered addresses in set.
    static void insertFilteredAddressesInSet( const QStringList&                        curveFilters,
                                              const std::set<RifEclipseSummaryAddress>& allAddressesInCase,
                                              std::set<RifEclipseSummaryAddress>*       setToInsertFilteredAddressesIn,
                                              std::vector<bool>*                        usedFilters );

    /// @return The summary plot command line help text.
    static QString summaryPlotCommandLineHelpText();

private:
    /// Creates curve set.
    static RimEnsembleCurveSet* createCurveSet( RimSummaryEnsemble*             ensemble,
                                                const RifEclipseSummaryAddress& addr,
                                                EnsembleColoringType            ensembleColoringStyle,
                                                QString                         ensembleColoringParameter );

    /// Creates history curve.
    static RimSummaryCurve* createHistoryCurve( const RifEclipseSummaryAddress& addr, RimSummaryCase* summaryCasesToUse );

    /// Adds curves from address filters to plot.
    static std::vector<RimSummaryCurve*> addCurvesFromAddressFiltersToPlot( const QStringList& curveFilters,
                                                                            RimSummaryPlot*    plot,
                                                                            RimSummaryCase*    summaryCase,
                                                                            bool               addHistoryCurves );

    /// Applies summary address filters to cases.
    static std::set<RifEclipseSummaryAddress> applySummaryAddressFiltersToCases( const std::vector<RimSummaryCase*>& summaryCasesToUse,
                                                                                 const QStringList& summaryAddressFilters );
};

#include "RigEclipseResultAddress.h"

//--------------------------------------------------------------------------------------------------
/// @brief Supports rig grid cell result address command workflows.
//--------------------------------------------------------------------------------------------------
class RigGridCellResultAddress
{
public:
    RigGridCellResultAddress()
        : gridIndex( -1 )
        , i( -1 )
        , j( -1 )
        , k( -1 )
    {
    }

    RigGridCellResultAddress( size_t gridIndex, size_t i, size_t j, size_t k, const RigEclipseResultAddress& eclipseResultAddress )
        : gridIndex( gridIndex )
        , i( i )
        , j( j )
        , k( k )
        , eclipseResultAddress( eclipseResultAddress )
    {
    }

    /// Creates grid cell addresses from filter.
    static std::vector<RigGridCellResultAddress> createGridCellAddressesFromFilter( const QString& text );
    // Using zero based ijk

    size_t                  gridIndex;
    size_t                  i;
    size_t                  j;
    size_t                  k;
    RigEclipseResultAddress eclipseResultAddress;
};
