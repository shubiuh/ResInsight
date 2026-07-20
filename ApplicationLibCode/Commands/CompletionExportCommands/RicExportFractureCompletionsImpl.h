/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017-     Statoil ASA
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
/// @brief Declares export fracture completions command support.

#pragma once

#include "RigCompletionData.h"

#include "cvfVector3.h"

#include <map>
#include <vector>

class RigFractureGrid;
class RicWellPathFractureReportItem;
class RigWellPath;
class RigTransmissibilityCondenser;
class RigEclipseToStimPlanCalculator;
class RigCaseCellResultsData;

class RimEclipseCase;
class RimFracture;
class RimFractureTemplate;
class RimWellPath;

class QTextStream;
class QString;

//--------------------------------------------------------------------------------------------------
/// @brief Supports export fracture completions command workflows.
//--------------------------------------------------------------------------------------------------
class RicExportFractureCompletionsImpl
{
public:
    enum PressureDepletionWBHPSource
    {
        WBHP_FROM_SUMMARY,
        WBHP_FROM_USER_DEF
    };

    //--------------------------------------------------------------------------------------------------
/// @brief Data used to configure or execute pressure depletion parameters command workflows.
    //--------------------------------------------------------------------------------------------------
    struct PressureDepletionParameters
    {
        PressureDepletionParameters( bool                        performScaling          = false,
                                     int                         pressureScalingTimeStep = 0,
                                     PressureDepletionWBHPSource wbhpSource              = WBHP_FROM_SUMMARY,
                                     double                      userWBHP                = 200.0 )
            : performScaling( performScaling )
            , pressureScalingTimeStep( pressureScalingTimeStep )
            , wbhpSource( wbhpSource )
            , userWBHP( userWBHP )
        {
        }

        bool                        performScaling;
        int                         pressureScalingTimeStep;
        PressureDepletionWBHPSource wbhpSource;
        double                      userWBHP;
    };

    /// @return The generate compdat values for well path.
    static std::vector<RigCompletionData>
        generateCompdatValuesForWellPath( RimWellPath*                                wellPath,
                                          RimEclipseCase*                             caseToApply,
                                          std::vector<RicWellPathFractureReportItem>* fractureDataForReport,
                                          QTextStream*                                outputStreamForIntermediateResultsText,
                                          PressureDepletionParameters                 pdParams = PressureDepletionParameters() );

    /// @return The generate compdat values.
    static std::vector<RigCompletionData> generateCompdatValues( RimEclipseCase*                             caseToApply,
                                                                 const QString&                              wellNameForExport,
                                                                 const RigWellPath*                          wellPathGeometry,
                                                                 const std::vector<const RimFracture*>&      fractures,
                                                                 std::vector<RicWellPathFractureReportItem>* fractureDataReportItems,
                                                                 QTextStream*                outputStreamForIntermediateResultsText,
                                                                 PressureDepletionParameters pdParams = PressureDepletionParameters() );

    /// Gets well pressures and initial production time step from summary data.
    static void getWellPressuresAndInitialProductionTimeStepFromSummaryData( const RimEclipseCase* caseToApply,
                                                                             const QString&        wellPathName,
                                                                             int                   currentTimeStep,
                                                                             int*                  initialTimeStep,
                                                                             double*               initialWellPressure,
                                                                             double*               currentWellPressure );

private:
    /// @return The generate compdat values const.
    static std::vector<RigCompletionData> generateCompdatValuesConst( const RimEclipseCase*                       caseToApply,
                                                                      const QString&                              wellPathName,
                                                                      const RigWellPath*                          wellPathGeometry,
                                                                      const std::vector<const RimFracture*>&      fractures,
                                                                      std::vector<RicWellPathFractureReportItem>* fractureDataReportItems,
                                                                      QTextStream*                outputStreamForIntermediateResultsText,
                                                                      PressureDepletionParameters pdParams );

    /// @return The check for stim plan conductivity.
    static bool checkForStimPlanConductivity( const RimFractureTemplate* fracTemplate, const RimFracture* fracture );

    /// Calculates internal fracture transmissibilities.
    static void calculateInternalFractureTransmissibilities( const RigFractureGrid*        fractureGrid,
                                                             double                        cDarcyInCorrectUnit,
                                                             RigTransmissibilityCondenser& transCondenser );

    /// Calculates fracture to well transmissibilities.
    static void calculateFractureToWellTransmissibilities( const RimFractureTemplate*    fracTemplate,
                                                           const RigFractureGrid*        fractureGrid,
                                                           const RimFracture*            fracture,
                                                           double                        cDarcyInCorrectUnit,
                                                           const RigWellPath*            wellPathGeometry,
                                                           RigTransmissibilityCondenser& transCondenser,
                                                           bool                          useInfiniteWellPI );

    /// Calculates matrix to well transmissibilities.
    static std::map<size_t, double> calculateMatrixToWellTransmissibilities( RigTransmissibilityCondenser& transCondenser );

    /// @return The generate compdat values for fracture.
    static std::vector<RigCompletionData> generateCompdatValuesForFracture( const std::map<size_t, double>& matrixToWellTransmissibilites,
                                                                            const QString&                  wellPathName,
                                                                            const RimEclipseCase*           caseToApply,
                                                                            const RimFracture*              fracture,
                                                                            const RimFractureTemplate*      fracTemplate );

    /// Computes non darcy flow parameters.
    static void computeNonDarcyFlowParameters( const RimFracture* fracture, std::vector<RigCompletionData>& allCompletionsForOneFracture );

    /// @return The sum up transmissibilities.
    static double sumUpTransmissibilities( const std::vector<RigCompletionData>& allCompletionsForOneFracture );

    /// Calculates and set report item data.
    static void calculateAndSetReportItemData( const std::vector<RigCompletionData>& allCompletionsForOneFracture,
                                               const RigEclipseToStimPlanCalculator& calculator,
                                               RicWellPathFractureReportItem&        reportItem );

    /// Performs the output intermediate results text command operation.
    static void outputIntermediateResultsText( QTextStream*                  outputStreamForIntermediateResultsText,
                                               const RimFracture*            fracture,
                                               RigTransmissibilityCondenser& transCondenser,
                                               const RigMainGrid*            mainGrid,
                                               const RigFractureGrid*        fractureGrid );

    /// Computes well points in fracture plane.
    static std::vector<cvf::Vec3d> computeWellPointsInFracturePlane( const RimFracture* fracture, const RigWellPath* wellPathGeometry );

    /// Loads results by name.
    static bool loadResultsByName( RigCaseCellResultsData* cellResultsData, const std::vector<QString>& resultNames );
};
