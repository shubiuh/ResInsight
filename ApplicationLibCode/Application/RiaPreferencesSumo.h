/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2024     Equinor ASA
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
/// @brief SUMO cloud storage connection preferences.

#pragma once

#include "cafPdmField.h"
#include "cafPdmObject.h"

//--------------------------------------------------------------------------------------------------
/// @brief Preferences for SUMO cloud API authentication and endpoint configuration.
///
/// RiaPreferencesSumo stores the server URL, OAuth2 authority, scopes, and client ID used to
/// connect to a SUMO cloud instance. Values come from an external cloud configuration and are
/// intentionally read-only and excluded from PDM XML persistence, preventing deployment-specific
/// connection details from leaking into projects or user settings.
//--------------------------------------------------------------------------------------------------
class RiaPreferencesSumo : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes empty connection fields and makes them read-only/non-persistent.
    RiaPreferencesSumo();

    /// @return The SUMO preference object owned by the current application preferences.
    static RiaPreferencesSumo* current();

    /// Loads recognized SUMO connection values from an external configuration.
    /// @param keyValuePairs Connection keys; unrecognized entries are ignored.
    /// @param configFile Path or description identifying the active source.
    void setData( const std::map<QString, QString>& keyValuePairs, const QString& configFile );

    /// @return SUMO API server URL.
    QString server() const;
    /// @return OAuth2 authority used for interactive authentication.
    QString authority() const;
    /// @return OAuth2 scopes requested for the SUMO client.
    QString scopes() const;
    /// @return OAuth2 public client identifier.
    QString clientId() const;

private:
    /// Makes external configuration visible for diagnostics but unavailable for editing or XML I/O.
    void setFieldStates();

private:
    caf::PdmField<QString> m_configFile; ///< External configuration source.
    caf::PdmField<QString> m_server;     ///< SUMO API server URL.
    caf::PdmField<QString> m_authority;  ///< OAuth2 authority URL.
    caf::PdmField<QString> m_scopes;     ///< Space-delimited OAuth2 scopes.
    caf::PdmField<QString> m_clientId;   ///< OAuth2 public client identifier.
};
