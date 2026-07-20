/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2018-     Equinor ASA
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
/// @brief Declares holo lens create session command support.

#pragma once

#include "cafPdmChildField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPtrField.h"

class RicHoloLensServerSettings;

//==================================================================================================
/// @brief UI model for configuring holo lens create session.
//==================================================================================================
class RicHoloLensCreateSessionUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Constructs the command object.
    RicHoloLensCreateSessionUi();
    /// Destroys the command object.
    ~RicHoloLensCreateSessionUi() override;

    /// @return The server url.
    QString serverUrl() const;
    /// @return The session name.
    QString sessionName() const;
    /// @return The session pin code.
    QString sessionPinCode() const;

protected:
    /// Defines the field ordering used by the command UI.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;

private:
    caf::PdmChildField<RicHoloLensServerSettings*> m_serverSettings;

    caf::PdmField<QString> m_sessionName;
    caf::PdmField<QString> m_sessionPinCode;
};
