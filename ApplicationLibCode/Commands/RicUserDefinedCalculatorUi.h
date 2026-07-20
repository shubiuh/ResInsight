////////////////////////////    /////////////////////////////////////////////////////
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

#include "RiuCalculationsContextMenuManager.h"

#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPtrField.h"

#include <memory>

class RimUserDefinedCalculationCollection;
class RimUserDefinedCalculation;

//==================================================================================================
/// @brief UI model for configuring user defined calculator.
//==================================================================================================
class RicUserDefinedCalculatorUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Constructs the command object.
    RicUserDefinedCalculatorUi();

    /// @return The current calculation.
    RimUserDefinedCalculation* currentCalculation() const;
    /// Sets current calculation.
    void                       setCurrentCalculation( RimUserDefinedCalculation* calculation );

    /// @return The parse expression.
    bool parseExpression() const;
    /// Calculates command support.
    bool calculate() const;

    /// @return The calculations group name.
    virtual QString                              calculationsGroupName() const                                       = 0;
    /// @return The calulation group name.
    virtual QString                              calulationGroupName() const                                         = 0;
    /// @return The calculation collection.
    virtual RimUserDefinedCalculationCollection* calculationCollection() const                                       = 0;
    /// Performs the notify calculated name changed command operation.
    virtual void                                 notifyCalculatedNameChanged( int id, const QString& newName ) const = 0;

protected:
    /// Defines the field ordering used by the command UI.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;
    /// Responds to a value changed through the command UI.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;
    /// @return The selectable values for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;
    /// Performs the on editor widgets created command operation.
    void                          onEditorWidgetsCreated() override;

    /// Exports calculations.
    virtual void exportCalculations() = 0;
    /// Imports calculations.
    virtual void importCalculations() = 0;

private:
    /// Performs the on variable updated command operation.
    void onVariableUpdated( const SignalEmitter* emitter );
    /// Performs the connect signals command operation.
    void connectSignals( RimUserDefinedCalculation* calculation );
    /// Performs the on delete calculation clicked command operation.
    void onDeleteCalculationClicked();

private:
    caf::PdmPtrField<RimUserDefinedCalculation*> m_currentCalculation;

    std::unique_ptr<RiuCalculationsContextMenuManager> m_calcContextMenuMgr;
};
