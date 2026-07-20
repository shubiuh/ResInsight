/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2025 Equinor ASA
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
/// @brief Declares msw table rows reservoir-data functionality.

#pragma once

#include <cstddef>
#include <optional>
#include <string>

// clang-format off


// The structures below represent data from various MSW tables
// The variable names correspond to the table names in Opm::ParserKeywords
// If a parameter has a default defined, std::optional is used
// 
// Example file path:
// custom-opm-common/generated-opm-common/include/opm/input/eclipse/Parser/ParserKeywords/W.hpp

//==================================================================================================
/// Header structure for WELSEGS table (well-level information)
//==================================================================================================
/// @brief Models welsegs header for reservoir-data processing.
struct WelsegsHeader
{
    // type                     variableName;       // Opm::ParserKeywords::WELSEGS::...

    /// Stores well.
    std::string                 well;               // WELNAME
    /// Stores top depth.
    double                      topDepth;           // TOPDEP
    /// Stores top length.
    double                      topLength;          // TOPLEN
    /// Stores wellbore volume.
    std::optional<double>       wellboreVolume;     // WBORVOL
    /// Stores info type.
    std::string                 infoType;           // TUBOPT
    /// Stores pressure components.
    std::optional<std::string>  pressureComponents; // PRESOPT
    /// Stores flow model.
    std::optional<std::string>  flowModel;          // FLOWOPT
};

//==================================================================================================
/// Row structure for WELSEGS table segment data
//==================================================================================================
/// @brief Models welsegs row for reservoir-data processing.
struct WelsegsRow
{
    // type               variableName; // Opm::ParserKeywords::WELSEGS::...

    /// Stores segment1.
    int                   segment1;     // ISEG1
    /// Stores segment2.
    int                   segment2;     // ISEG2
    /// Stores branch.
    int                   branch;       // IBRANCH
    /// Stores join segment.
    int                   joinSegment;  // ISEG3
    /// Stores length.
    double                length;       // LENGTH
    /// Stores depth.
    double                depth;        // DEPTH
    /// Stores diameter.
    std::optional<double> diameter;     // ID
    /// Stores roughness.
    std::optional<double> roughness;    // EPSILON
    
    /// Stores description.
    std::string           description;
    /// Stores source well name.
    std::string           sourceWellName;   // Name of the source well path object. This can be different than WELNAME used in other tables.
};

//==================================================================================================
/// Row structure for COMPSEGS table data
//==================================================================================================
/// @brief Models compsegs row for reservoir-data processing.
struct CompsegsRow
{
    // type             variableName;   // Opm::ParserKeywords::COMPSEGS::...

    /// Stores i.
    size_t              i;              // I (1-based)
    /// Stores j.
    size_t              j;              // J (1-based)
    /// Stores k.
    size_t              k;              // K (1-based)
    /// Stores branch.
    int                 branch;         // IBRANCH
    /// Stores distance start.
    double              distanceStart;  // LENGTH1
    /// Stores distance end.
    double              distanceEnd;    // LENGTH2

    /// Stores grid name.
    std::string gridName; // Empty for main grid, populated for LGR data

    /// Returns whether main grid.
    bool isMainGrid() const { return gridName.empty(); }
    /// Returns whether lgr grid.
    bool isLgrGrid() const { return !gridName.empty(); }
};

//==================================================================================================
/// Row structure for WSEGVALV table data
//==================================================================================================
/// @brief Models wsegvalv row for reservoir-data processing.
struct WsegvalvRow
{
    // type                     variableName;   // Opm::ParserKeywords::WSEGVALV::...

    /// Stores well.
    std::string                 well;           // WELNAME
    /// Stores segment number.
    int                         segmentNumber;  // ISEG1
    /// Stores cv.
    double                      cv;             // ICDCV
    /// Stores area.
    double                      area;           // AREARESET
    /// Stores extra length.
    std::optional<double>       extraLength;    // SEGLEN
    /// Stores pipe d.
    std::optional<double>       pipeD;          // ID
    /// Stores roughness.
    std::optional<double>       roughness;      // EPSILON
    /// Stores pipe a.
    std::optional<double>       pipeA;          // AREAPIPE
    /// Stores status.
    std::optional<std::string>  status;         // STATUS
    /// Stores max a.
    std::optional<double>       maxA;           // AREAMAX

    /// Stores description.
    std::string                 description;    // comment shown in export
};

//==================================================================================================
/// Row structure for WSEGAICD table data
//==================================================================================================
/// @brief Models wsegaicd row for reservoir-data processing.
struct WsegaicdRow
{
    // type                     variableName;       // Opm::ParserKeywords::WSEGAICD::...

    /// Stores well.
    std::string                 well;               //  1 WELNAME
    /// Stores segment1.
    int                         segment1;           //  2 ISEG1
    /// Stores segment2.
    int                         segment2;           //  3 ISEG2
    /// Stores strength.
    double                      strength;           //  4 ICDSTREN
    /// Stores length.
    std::optional<double>       length;             //  5 ICDLEN
    /// Stores density cali.
    std::optional<double>       densityCali;        //  6 CALDEN
    /// Stores viscosity cali.
    std::optional<double>       viscosityCali;      //  7 CALVISC
    /// Stores critical value.
    std::optional<double>       criticalValue;      //  8 EMLCRT
    /// Stores width trans.
    std::optional<double>       widthTrans;         //  9 EMLTRANS
    /// Stores max visc ratio.
    std::optional<double>       maxViscRatio;       // 10 EMLMAX
    /// Stores method scaling factor.
    std::optional<int>          methodScalingFactor;// 11 NSCALFAC
    /// Stores max abs rate.
    double                      maxAbsRate;         // 12 CALRATE
    /// Stores flow rate exponent.
    double                      flowRateExponent;   // 13 RATEXP
    /// Stores visc exponent.
    double                      viscExponent;       // 14 VISCEXP
    /// Stores status.
    std::optional<std::string>  status;             // 15 STATUS
    /// Stores oil flow fraction.
    std::optional<double>       oilFlowFraction;    // 16 A1
    /// Stores water flow fraction.
    std::optional<double>       waterFlowFraction;  // 17 A2
    /// Stores gas flow fraction.
    std::optional<double>       gasFlowFraction;    // 18 A3
    /// Stores oil visc fraction.
    std::optional<double>       oilViscFraction;    // 19 B1
    /// Stores water visc fraction.
    std::optional<double>       waterViscFraction;  // 20 B2
    /// Stores gas visc fraction.
    std::optional<double>       gasViscFraction;    // 21 B3

    /// Stores description.
    std::string                 description;
};

//==================================================================================================
/// Row structure for WSEGSICD table data
//==================================================================================================
/// @brief Models wsegsicd row for reservoir-data processing.
struct WsegsicdRow
{
    // type                     variableName;       // Opm::ParserKeywords::WSEGSICD::...

    /// Stores well.
    std::string                 well;               //  1 WELNAME
    /// Stores segment1.
    int                         segment1;           //  2 ISEG1
    /// Stores segment2.
    int                         segment2;           //  3 ISEG2
    /// Stores strength.
    double                      strength;           //  4 ICDSTREN
    /// Stores length.
    std::optional<double>       length;             //  5 ICDLEN
    /// Stores density cali.
    std::optional<double>       densityCali;        //  6 CALDEN
    /// Stores viscosity cali.
    std::optional<double>       viscosityCali;      //  7 CALVISC
    /// Stores critical value.
    std::optional<double>       criticalValue;      //  8 EMLCRT
    /// Stores width trans.
    std::optional<double>       widthTrans;         //  9 EMLTRANS
    /// Stores max visc ratio.
    std::optional<double>       maxViscRatio;       // 10 EMLMAX
    /// Stores method scaling factor.
    std::optional<int>          methodScalingFactor;// 11 NSCALFAC
    /// Stores max abs rate.
    double                      maxAbsRate;         // 12 CALRATE
    /// Stores status.
    std::optional<std::string>  status;             // 13 STATUS

    /// Stores description.
    std::string                 description;
};
