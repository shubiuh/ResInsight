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
/// @brief Canonical result and property name strings used across ResInsight.

#pragma once

class RigEclipseResultAddress;

#include <QString>

#include <set>
#include <vector>

namespace RiaResultNames
{
/// @name Result classification
/// These predicates encode display and range-handling rules shared by result consumers.
/// @{
bool isPerCellFaceResult( const QString& resultName );
bool isLogarithmicResult( const QString& resultName );
bool isFlowResultWithBothPosAndNegValues( const QString& resultName );
bool isCategoryResult( const QString& resultName );
/// @}

/// @name Canonical generated-result identifiers
/// Returned strings are stable keys used to register, find, and serialize derived results.
/// @{
QString undefinedResultName();
QString undefinedGridFaultName();
QString undefinedGridFaultWithInactiveName();
QString combinedTransmissibilityResultName();
QString combinedWaterFluxResultName();
QString combinedOilFluxResultName();
QString combinedGasFluxResultName();

QString ternarySaturationResultName();
QString combinedMultResultName();

QString eqlnumResultName();

QString riTranXResultName();
QString riTranYResultName();
QString riTranZResultName();
QString combinedRiTranResultName();

QString riMultXResultName();
QString riMultYResultName();
QString riMultZResultName();
QString combinedRiMultResultName();

QString riNncCells();

QString riAreaNormTranXResultName();
QString riAreaNormTranYResultName();
QString riAreaNormTranZResultName();
QString combinedRiAreaNormTranResultName();

QString riCellVolumeResultName();
QString riOilVolumeResultName();
QString mobilePoreVolumeName();
QString riPorvSoil();
QString riPorvSgas();
QString riPorvSoilSgas();

QString faultReactAssessmentPrefix();

QString completionTypeResultName();
/// @}

/// Returns the canonical grid-index result name, accounting for radial-grid conventions where needed.
QString                 indexIResultName( bool isRadial );
QString                 indexJResultName( bool isRadial );
QString                 indexKResultName();
/// Builds the static integer result address used for index and categorical results.
RigEclipseResultAddress staticIntegerAddress( const QString& resultName );

/// @name Common simulator property identifiers
/// @{
QString faultDistanceName();
QString facies();

QString porv();
QString rockv();
QString soil();
QString sgas();
QString swat();

QString sowcr();
QString sogcr();
QString sgcr();
QString convergence();

QString opernum();
QString bordnum();
/// @}

/// @name Wellbore-stability derived-result identifiers
/// @{
QString wbsAzimuthResult();
QString wbsInclinationResult();
QString wbsPPResult();
QString wbsSHResult();
QString wbsSHMkResult();
QString wbsOBGResult();
QString wbsFGResult();
QString wbsSFGResult();
QString wbsFGMkExpResult();
QString wbsFGMkMinResult();
QString wbsSHMkExpResult();
QString wbsSHMkMinResult();
QString wbsSHMkMaxResult();
QString wbsPPMinResult();
QString wbsPPMaxResult();
QString wbsPPExpResult();
QString wbsPPInitialResult();
/// @}

/// @name Fault and region result identifiers
/// @{
QString           formationBinaryAllanResultName();
QString           formationAllanResultName();
std::set<QString> nncResultNames();

QString satnumResult();
QString imbnumResult();
/// @}

/// @return Ordered wellbore-stability angle results available to selectors.
std::vector<QString> wbsAngleResultNames();
/// @return Ordered collection of all supported wellbore-stability derived results.
std::vector<QString> wbsDerivedResultNames();

/// @return Canonical generated result containing active formation names.
QString activeFormationNamesResultName();

}; // namespace RiaResultNames
