/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2016-     Statoil ASA
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
/// @brief Declares flow diag defines reservoir-data functionality.
#pragma once

#include <string>
#include <vector>

#include <QString>

namespace RigFlowDiagDefines
{
/// @brief Models rel perm curve for reservoir-data processing.
struct RelPermCurve
{
    /// Enumerates the supported ident values.
    enum Ident
    {
        KRW,
        KRG,
        KROW,
        KROG,
        PCOW,
        PCOG
    };
    /// Enumerates the supported eps mode values.
    enum EpsMode
    {
        EPS_ON,
        EPS_OFF
    };
    /// Enumerates the supported curve set values.
    enum CurveSet
    {
        DRAINAGE,
        IMBIBITION
    };

    /// Returns whether water curve.
    bool isWaterCurve() const { return ( ident == KRW || ident == KROW || ident == PCOW ); }
    /// Returns whether gas curve.
    bool isGasCurve() const { return ( ident == KRG || ident == KROG || ident == PCOG ); }

    /// Stores ident.
    Ident               ident;
    /// Stores name.
    std::string         name;
    /// Stores eps mode.
    EpsMode             epsMode;
    /// Stores curve set.
    CurveSet            curveSet;
    /// Stores saturation vals.
    std::vector<double> saturationVals;
    /// Stores y vals.
    std::vector<double> yVals;
};

/// Enumerates the supported pvt curve type values.
enum PvtCurveType
{
    PVT_CT_FVF,
    PVT_CT_VISCOSITY
};

/// @brief Models pvt curve for reservoir-data processing.
struct PvtCurve
{
    /// Enumerates the supported phase values.
    enum Phase
    {
        OIL,
        GAS
    };
    /// Enumerates the supported ident values.
    enum Ident
    {
        Unknown,
        Bo,
        Bg,
        Visc_o,
        Visc_g
    };

    /// Stores ident.
    Ident               ident;
    /// Stores phase.
    Phase               phase;
    /// Stores pressure vals.
    std::vector<double> pressureVals;
    /// Stores y vals.
    std::vector<double> yVals;
    /// Stores mix rat vals.
    std::vector<double> mixRatVals;
};

/// @brief Models flow characteristics result frame for reservoir-data processing.
struct FlowCharacteristicsResultFrame
{
    /// Type alias used for curve.
    using Curve = std::pair<std::vector<double>, std::vector<double>>;

    /// Stores storage cap flow cap curve.
    Curve  m_storageCapFlowCapCurve;
    /// Stores dimensionless time sweep efficiency curve.
    Curve  m_dimensionlessTimeSweepEfficiencyCurve;
    /// Stores lorenz coefficient.
    double m_lorenzCoefficient = HUGE_VAL;
};

QString tofResultName();
QString cellFractionResultName();
QString maxFractionTracerResultName();
QString communicationResultName();
QString numFloodedPv();

QString flowTotalName();
QString flowOilName();
QString flowGasName();
QString flowWaterName();

QString reservoirTracerName();
QString tinyTracerGroupName();

} // namespace RigFlowDiagDefines
