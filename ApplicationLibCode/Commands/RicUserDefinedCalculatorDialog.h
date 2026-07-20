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
/// @brief Declares user defined calculator command support.

#pragma once

#include <QDialog>

class RimUserDefinedCalculation;
class RimUserDefinedCalculationCollection;

//==================================================================================================
/// @brief Dialog for configuring user defined calculator.
//==================================================================================================
class RicUserDefinedCalculatorDialog : public QDialog
{
    Q_OBJECT

public:
    /// Constructs the command object.
    RicUserDefinedCalculatorDialog( QWidget* parent, const QString& title );
    /// Destroys the command object.
    ~RicUserDefinedCalculatorDialog() override;

    /// Sets calculation and update.
    virtual void                                 setCalculationAndUpdateUi( RimUserDefinedCalculation* calculation ) = 0;
    /// @return The calculation collection.
    virtual RimUserDefinedCalculationCollection* calculationCollection() const                                       = 0;
    /// @return The calculator widget.
    virtual QWidget*                             getCalculatorWidget()                                               = 0;
    /// Updates command support.
    virtual void                                 updateUi()                                                          = 0;

private slots:
    /// Performs the slot try close command operation.
    void slotTryCloseDialog();

protected:
    /// Sets up.
    void   setUp();
    /// @return The dirty count.
    size_t dirtyCount() const;
};
