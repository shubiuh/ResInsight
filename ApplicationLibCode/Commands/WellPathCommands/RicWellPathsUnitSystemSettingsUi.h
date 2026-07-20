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
/// @brief Declares well paths unit system settings command support.

#pragma once

#include "RiaDefines.h"

#include "cafPdmField.h"
#include "cafPdmObject.h"

//==================================================================================================
/// @brief UI model for configuring well paths unit system settings.
//==================================================================================================
class RicWellPathsUnitSystemSettingsUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Constructs the command object.
    RicWellPathsUnitSystemSettingsUi();

    caf::PdmField<caf::AppEnum<RiaDefines::EclipseUnitSystem>> unitSystem;
};
