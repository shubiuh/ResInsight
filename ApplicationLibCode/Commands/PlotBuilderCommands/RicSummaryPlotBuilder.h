////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2021-     Equinor ASA
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

class RimPlot;
class RimMultiPlot;
class RifEclipseSummaryAddress;
class RimSummaryCase;
class RimSummaryEnsemble;
class RimSummaryPlot;
class RimEnsembleCurveSet;
class RimSummaryCurve;
class RimSummaryMultiPlot;
class RiaSummaryCurveAddress;

/// @brief Utilities for caf command workflows.
namespace caf
{
class PdmObject;
class PdmObjectHandle;
} // namespace caf

#include <set>
#include <vector>

//==================================================================================================
/// @brief Builds ria summary plot data for command workflows.
//==================================================================================================
class RiaSummaryPlotBuilder
{
public:
    enum class RicGraphCurveGrouping
    {
        SINGLE_CURVES,
        CURVES_FOR_OBJECT,
        NONE
    };

public:
    /// Constructs the command object.
    RiaSummaryPlotBuilder();

    /// Sets data sources.
    void setDataSources( const std::vector<RimSummaryCase*>& summaryCases, const std::vector<RimSummaryEnsemble*>& ensembles );

    /// Sets addresses.
    void setAddresses( const std::set<RifEclipseSummaryAddress>& addresses );

    /// Sets individual plot per data source.
    void setIndividualPlotPerDataSource( bool enable );
    /// Sets grouping.
    void setGrouping( RicGraphCurveGrouping groping );

    /// Creates plots.
    std::vector<RimSummaryPlot*> createPlots() const;

    // Static helper functions
    static std::set<RifEclipseSummaryAddress> addressesForSource( caf::PdmObject* summarySource );

    /// Creates curve set.
    static RimEnsembleCurveSet* createCurveSet( RimSummaryEnsemble* ensemble, const RifEclipseSummaryAddress& addr );
    /// Creates curve.
    static RimSummaryCurve*     createCurve( RimSummaryCase* summaryCase, const RifEclipseSummaryAddress& addr );

    /// @return The duplicate plots.
    static std::vector<RimPlot*>        duplicatePlots( const std::vector<RimPlot*>& plots );
    /// @return The duplicate summary plots.
    static std::vector<RimSummaryPlot*> duplicateSummaryPlots( const std::vector<RimSummaryPlot*>& plots );

    /// Creates and append multi plot.
    static RimMultiPlot* createAndAppendMultiPlot( const std::vector<RimPlot*>& plots );
    /// Appends plots to multi plot.
    static void          appendPlotsToMultiPlot( RimMultiPlot* multiPlot, const std::vector<RimPlot*>& plots );

    /// Creates and append default summary multi plot.
    static RimSummaryMultiPlot* createAndAppendDefaultSummaryMultiPlot( const std::vector<RimSummaryCase*>&     cases,
                                                                        const std::vector<RimSummaryEnsemble*>& ensembles,
                                                                        bool skipCreationOfPlotBasedOnPreferences = true );

    /// Creates and append single summary multi plot no auto settings.
    static RimSummaryMultiPlot* createAndAppendSingleSummaryMultiPlotNoAutoSettings( RimSummaryPlot* plot );
    /// Creates and append single summary multi plot.
    static RimSummaryMultiPlot* createAndAppendSingleSummaryMultiPlot( RimSummaryPlot* plot );
    /// Creates and append summary multi plot.
    static RimSummaryMultiPlot* createAndAppendSummaryMultiPlot( const std::vector<RimSummaryPlot*>& plots );
    /// Creates and append summary multi plot.
    static RimSummaryMultiPlot* createAndAppendSummaryMultiPlot( const std::vector<caf::PdmObjectHandle*>& objects );
    /// Appends plots to summary multi plot.
    static void                 appendPlotsToSummaryMultiPlot( RimSummaryMultiPlot* multiPlot, const std::vector<RimSummaryPlot*>& plots );

    /// Creates plot.
    static RimSummaryPlot* createPlot( const std::vector<RimSummaryCurve*>& summaryCurves );

    /// Creates plot.
    static RimSummaryPlot* createPlot( const std::set<RifEclipseSummaryAddress>& addresses,
                                       const std::vector<RimSummaryCase*>&       summaryCases,
                                       const std::vector<RimSummaryEnsemble*>&   ensembles );

    /// Creates cross plot.
    static RimSummaryPlot* createCrossPlot( const std::vector<RiaSummaryCurveAddress>& addresses,
                                            const std::vector<RimSummaryCase*>&        summaryCases,
                                            const std::vector<RimSummaryEnsemble*>&    ensembles );

    /// Appends curves to plot.
    static void appendCurvesToPlot( RimSummaryPlot*                           summaryPlot,
                                    const std::set<RifEclipseSummaryAddress>& addresses,
                                    const std::vector<RimSummaryCase*>&       summaryCases,
                                    const std::vector<RimSummaryEnsemble*>&   ensembles );

    /// Adds new ensemble curve.
    static RimEnsembleCurveSet*
        addNewEnsembleCurve( RimSummaryPlot* summaryPlot, const RiaSummaryCurveAddress& curveAddress, RimSummaryEnsemble* ensemble );

    /// Adds new summary curve.
    static RimSummaryCurve*
        addNewSummaryCurve( RimSummaryPlot* summaryPlot, const RiaSummaryCurveAddress& curveAddress, RimSummaryCase* summaryCase );

private:
    std::set<RifEclipseSummaryAddress> m_addresses;
    std::vector<RimSummaryCase*>       m_summaryCases;
    std::vector<RimSummaryEnsemble*>   m_ensembles;

    bool m_individualPlotPerDataSource;

    RicGraphCurveGrouping m_graphCurveGrouping;
};
