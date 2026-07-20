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
/// @brief Implements OpenTelemetry exporter preferences.

#include "RiaPreferencesOpenTelemetry.h"

#include "RiaApplication.h"
#include "RiaLogging.h"
#include "RiaPreferences.h"
#include "RiaQStringFormatter.h"
#include "RiaVersionInfo.h"

#include "cafPdmUiTextEditor.h"

CAF_PDM_SOURCE_INIT( RiaPreferencesOpenTelemetry, "RiaPreferencesOpenTelemetry" );

//--------------------------------------------------------------------------------------------------
/// Defaults provide bounded exporter behavior even when optional keys are absent.
//--------------------------------------------------------------------------------------------------
RiaPreferencesOpenTelemetry::RiaPreferencesOpenTelemetry()
{
    CAF_PDM_InitObject( "OpenTelemetry Configuration", "", "", "Configuration for OpenTelemetry crash reporting and telemetry" );

    CAF_PDM_InitField( &m_configFile, "configFile", QString( "No config file detected" ), "Config File" );
    CAF_PDM_InitField( &m_connectionString, "connectionString", QString(), "Azure Connection String" );
    m_connectionString.uiCapability()->setUiEditorTypeName( caf::PdmUiTextEditor::uiEditorTypeName() );

    CAF_PDM_InitField( &m_batchTimeoutMs, "batchTimeoutMs", 5000, "Batch Timeout (ms)" );
    CAF_PDM_InitField( &m_maxBatchSize, "maxBatchSize", 512, "Max Batch Size" );
    CAF_PDM_InitField( &m_maxQueueSize, "maxQueueSize", 10000, "Max Queue Size" );
    CAF_PDM_InitField( &m_memoryThresholdMb, "memoryThresholdMb", 50, "Memory Threshold (MB)" );
    CAF_PDM_InitField( &m_samplingRate, "samplingRate", 1.0, "Sampling Rate" );
    CAF_PDM_InitField( &m_connectionTimeoutMs, "connectionTimeoutMs", 10000, "Connection Timeout (ms)" );
    CAF_PDM_InitField( &m_eventAllowlist, "eventAllowlist", QString(), "Event Allowlist" );
    CAF_PDM_InitField( &m_eventDenylist, "eventDenylist", QString(), "Event Denylist" );

    setFieldStates();
}

//--------------------------------------------------------------------------------------------------
/// Resolves through RiaApplication so GUI and console modes use the same runtime configuration.
//--------------------------------------------------------------------------------------------------
RiaPreferencesOpenTelemetry* RiaPreferencesOpenTelemetry::current()
{
    return RiaApplication::instance()->preferences()->openTelemetryPreferences();
}

//--------------------------------------------------------------------------------------------------
/// Explicit key handling makes accepted deployment configuration discoverable and logs typos.
//--------------------------------------------------------------------------------------------------
void RiaPreferencesOpenTelemetry::setData( const std::map<QString, QString>& keyValuePairs, const QString& configFile )
{
    m_configFile = configFile;

    for ( const auto& [key, value] : keyValuePairs )
    {
        if ( key == "connection_string" )
        {
            m_connectionString = value;
        }
        else if ( key == "batch_timeout_ms" )
        {
            m_batchTimeoutMs = value.toInt();
        }
        else if ( key == "max_batch_size" )
        {
            m_maxBatchSize = value.toInt();
        }
        else if ( key == "max_queue_size" )
        {
            m_maxQueueSize = value.toInt();
        }
        else if ( key == "memory_threshold_mb" )
        {
            m_memoryThresholdMb = value.toInt();
        }
        else if ( key == "sampling_rate" )
        {
            m_samplingRate = value.toDouble();
        }
        else if ( key == "connection_timeout_ms" )
        {
            m_connectionTimeoutMs = value.toInt();
        }
        else if ( key == "event_allowlist" )
        {
            m_eventAllowlist = value;
        }
        else if ( key == "event_denylist" )
        {
            m_eventDenylist = value;
        }
        else
        {
            RiaLogging::warning( std::format( "Unknown OpenTelemetry config key: '{}'", key ) );
        }
    }
}

//--------------------------------------------------------------------------------------------------
/// External telemetry configuration is intentionally neither editable nor serialized by PDM.
//--------------------------------------------------------------------------------------------------
void RiaPreferencesOpenTelemetry::setFieldStates()
{
    for ( auto field : this->fields() )
    {
        field->uiCapability()->setUiReadOnly( true );
        field->xmlCapability()->disableIO();
    }
}

//--------------------------------------------------------------------------------------------------
/// Advanced operational limits are collapsed to keep connection diagnostics prominent.
//--------------------------------------------------------------------------------------------------
void RiaPreferencesOpenTelemetry::defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering )
{
    uiOrdering.add( &m_configFile );
    uiOrdering.add( &m_connectionString );

    auto group = uiOrdering.addNewGroup( "Configuration" );
    group->setCollapsedByDefault();

    group->add( &m_batchTimeoutMs );
    group->add( &m_maxBatchSize );
    group->add( &m_maxQueueSize );
    group->add( &m_memoryThresholdMb );
    group->add( &m_samplingRate );
    group->add( &m_connectionTimeoutMs );
    group->add( &m_eventAllowlist );
    group->add( &m_eventDenylist );

    uiOrdering.skipRemainingFields();
}

//--------------------------------------------------------------------------------------------------
/// The stable service name groups telemetry from different ResInsight versions together.
//--------------------------------------------------------------------------------------------------
QString RiaPreferencesOpenTelemetry::serviceName() const
{
    return QStringLiteral( "ResInsight" );
}

//--------------------------------------------------------------------------------------------------
/// Build version is used rather than a separately configurable telemetry version.
//--------------------------------------------------------------------------------------------------
QString RiaPreferencesOpenTelemetry::serviceVersion() const
{
    return QString( STRPRODUCTVER );
}

//--------------------------------------------------------------------------------------------------
/// Returns the connection material exactly as supplied by the external configuration.
//--------------------------------------------------------------------------------------------------
QString RiaPreferencesOpenTelemetry::connectionString() const
{
    return m_connectionString;
}

//--------------------------------------------------------------------------------------------------
/// Batch timeout controls latency versus exporter efficiency.
//--------------------------------------------------------------------------------------------------
int RiaPreferencesOpenTelemetry::batchTimeoutMs() const
{
    return m_batchTimeoutMs;
}

//--------------------------------------------------------------------------------------------------
/// Batch size bounds one exporter operation independently of total queue capacity.
//--------------------------------------------------------------------------------------------------
int RiaPreferencesOpenTelemetry::maxBatchSize() const
{
    return m_maxBatchSize;
}

//--------------------------------------------------------------------------------------------------
/// Queue size bounds buffered telemetry during slow or unavailable network export.
//--------------------------------------------------------------------------------------------------
int RiaPreferencesOpenTelemetry::maxQueueSize() const
{
    return m_maxQueueSize;
}

//--------------------------------------------------------------------------------------------------
/// Memory threshold provides a second safeguard beyond the item-count queue limit.
//--------------------------------------------------------------------------------------------------
int RiaPreferencesOpenTelemetry::memoryThresholdMb() const
{
    return m_memoryThresholdMb;
}

//--------------------------------------------------------------------------------------------------
/// Sampling rate is preserved as a fraction for downstream telemetry configuration.
//--------------------------------------------------------------------------------------------------
double RiaPreferencesOpenTelemetry::samplingRate() const
{
    return m_samplingRate;
}

//--------------------------------------------------------------------------------------------------
/// Connection timeout bounds individual network attempts by the exporter.
//--------------------------------------------------------------------------------------------------
int RiaPreferencesOpenTelemetry::connectionTimeoutMs() const
{
    return m_connectionTimeoutMs;
}

//--------------------------------------------------------------------------------------------------
/// Empty comma-separated elements are skipped so trailing commas do not create event names.
//--------------------------------------------------------------------------------------------------
QStringList RiaPreferencesOpenTelemetry::eventAllowlist() const
{
    return m_eventAllowlist().split( ',', Qt::SkipEmptyParts );
}

//--------------------------------------------------------------------------------------------------
/// Denylist parsing mirrors allowlist parsing for predictable filtering semantics.
//--------------------------------------------------------------------------------------------------
QStringList RiaPreferencesOpenTelemetry::eventDenylist() const
{
    return m_eventDenylist().split( ',', Qt::SkipEmptyParts );
}
