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
/// @brief Declares saturation pressure command support.

#pragma once

#include "RimEclipseCase.h"

#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPtrField.h"

//==================================================================================================
/// @brief UI model for configuring saturation pressure.
//==================================================================================================
class RicSaturationPressureUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Constructs the command object.
    RicSaturationPressureUi();

    /// Sets selected case.
    void            setSelectedCase( RimEclipseCase* eclipseCase );
    /// @return The selected case.
    RimEclipseCase* selectedCase() const;
    /// @return The selected time step.
    int             selectedTimeStep() const;

protected:
    /// @return The selectable values for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;

private:
    caf::PdmPtrField<RimEclipseCase*> m_caseToApply;
    caf::PdmField<int>                m_timeStep;
};
