/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2025- Equinor ASA
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
/// @brief OpenTelemetry exporter preferences.

#pragma once

#include "cafPdmField.h"
#include "cafPdmObject.h"

#include <QString>

#include <map>
#include <vector>

//==================================================================================================
/// @brief Read-only runtime configuration for OpenTelemetry export and event filtering.
///
/// Values are populated from an external key/value configuration by setData(). They are displayed
/// in the preferences dialog for diagnostics, but setFieldStates() disables editing and PDM XML I/O
/// so secrets and deployment-specific values are not copied into ResInsight settings or projects.
/// Unknown keys are logged and ignored.
///
/// The service identity is fixed to ResInsight and its build version. Allowlist and denylist values
/// are stored as comma-separated text and converted to QStringList on access.
//==================================================================================================
class RiaPreferencesOpenTelemetry : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes documented defaults before any external configuration is loaded.
    RiaPreferencesOpenTelemetry();

    /// @return The telemetry preference object owned by the current application preferences.
    static RiaPreferencesOpenTelemetry* current();

    /// Loads recognized telemetry keys and records their source configuration file.
    /// @param keyValuePairs External keys using snake_case configuration names.
    /// @param configFile Path or description shown to identify the active source.
    void setData( const std::map<QString, QString>& keyValuePairs, const QString& configFile );

    /// @return Fixed telemetry service name, "ResInsight".
    QString serviceName() const;

    /// @return Product version compiled from ResInsightVersion.cmake.
    QString serviceVersion() const;

    /// @return Azure/Application Insights connection string.
    QString     connectionString() const;
    /// @return Maximum exporter batching delay in milliseconds.
    int         batchTimeoutMs() const;
    /// @return Maximum number of telemetry items in one export batch.
    int         maxBatchSize() const;
    /// @return Maximum number of queued telemetry items.
    int         maxQueueSize() const;
    /// @return Memory threshold in MiB used by telemetry safeguards.
    int         memoryThresholdMb() const;
    /// @return Fraction of eligible events sampled for export.
    double      samplingRate() const;
    /// @return Export connection timeout in milliseconds.
    int         connectionTimeoutMs() const;
    /// @return Comma-separated allowlist parsed into non-empty event names.
    QStringList eventAllowlist() const;
    /// @return Comma-separated denylist parsed into non-empty event names.
    QStringList eventDenylist() const;

protected:
    /// Places connection information first and advanced exporter settings in a collapsed group.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;

private:
    /// Makes all fields read-only and disables persistence through PDM XML capabilities.
    void setFieldStates();

private:
    caf::PdmField<QString> m_configFile;         ///< External configuration source shown for diagnostics.
    caf::PdmField<QString> m_connectionString;   ///< Azure/Application Insights exporter connection string.
    caf::PdmField<int>     m_batchTimeoutMs;     ///< Maximum exporter batching delay.
    caf::PdmField<int>     m_maxBatchSize;       ///< Maximum items per export batch.
    caf::PdmField<int>     m_maxQueueSize;       ///< Maximum buffered telemetry items.
    caf::PdmField<int>     m_memoryThresholdMb;  ///< Memory safeguard threshold in MiB.
    caf::PdmField<double>  m_samplingRate;       ///< Export sampling fraction.
    caf::PdmField<int>     m_connectionTimeoutMs; ///< Network connection timeout.
    caf::PdmField<QString> m_eventAllowlist;     ///< Comma-separated events explicitly allowed.
    caf::PdmField<QString> m_eventDenylist;      ///< Comma-separated events explicitly denied.
};
