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
/// @brief Declares summary curve calculator command support.

#pragma once

#include "RicUserDefinedCalculatorDialog.h"

#include <memory>

class RicCalculatorWidgetCreator;

//==================================================================================================
/// @brief Dialog for configuring summary curve calculator.
//==================================================================================================
class RicSummaryCurveCalculatorDialog : public RicUserDefinedCalculatorDialog
{
    Q_OBJECT

public:
    /// Constructs the command object.
    RicSummaryCurveCalculatorDialog( QWidget* parent );
    /// Destroys the command object.
    ~RicSummaryCurveCalculatorDialog() override;

    /// Sets calculation and update.
    void                                 setCalculationAndUpdateUi( RimUserDefinedCalculation* calculation ) override;
    /// @return The calculator widget.
    QWidget*                             getCalculatorWidget() override;
    /// Updates command support.
    void                                 updateUi() override;
    /// @return The calculation collection.
    RimUserDefinedCalculationCollection* calculationCollection() const override;

private:
    std::unique_ptr<RicCalculatorWidgetCreator> m_summaryCalcEditor;
};
