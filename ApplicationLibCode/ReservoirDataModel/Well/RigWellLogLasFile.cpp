/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2015-     Statoil ASA
//  Copyright (C) 2015-     Ceetron Solutions AS
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
/// @brief Implements well log las file reservoir-data functionality.

#include "RigWellLogLasFile.h"

#include "RigWellLogCurveData.h"

#include "RiaStringEncodingTools.h"

#include "RimWellLogCurve.h"

#include "laswell.hpp"
#include "well.hpp"

#include <QFileInfo>
#include <QString>

#include <cmath> // Needed for HUGE_VAL on Linux
#include <exception>

//--------------------------------------------------------------------------------------------------
/// Creates a RigWellLogLasFile instance.
//--------------------------------------------------------------------------------------------------
RigWellLogLasFile::RigWellLogLasFile()
    : RigWellLogData()
{
    m_wellLogFile = nullptr;
}

//--------------------------------------------------------------------------------------------------
/// Destroys the RigWellLogLasFile instance.
//--------------------------------------------------------------------------------------------------
RigWellLogLasFile::~RigWellLogLasFile()
{
    close();
}

//--------------------------------------------------------------------------------------------------
/// Returns the open.
//--------------------------------------------------------------------------------------------------
bool RigWellLogLasFile::open( const QString& fileName, QString* errorMessage )
{
    close();

    auto guessFormatFromFileExtension = []( const QString& filePath )
    {
        QFileInfo fi( filePath );
        if ( fi.suffix().toUpper() == "RMSWELL" )
            return NRLib::Well::RMS;
        else
            return NRLib::Well::LAS;
    };

    NRLib::Well* well = nullptr;

    try
    {
        int wellFormat = guessFormatFromFileExtension( fileName );

        well = NRLib::Well::ReadWell( RiaStringEncodingTools::toNativeEncoded( fileName ).data(), wellFormat );
        if ( !well )
        {
            return false;
        }
    }
    catch ( std::exception& e )
    {
        if ( well )
        {
            delete well;
        }

        if ( e.what() )
        {
            CVF_ASSERT( errorMessage );
            *errorMessage = e.what();
        }

        return false;
    }

    QStringList wellLogNames;

    const std::map<std::string, std::vector<double>>&          contLogs = well->GetContLog();
    std::map<std::string, std::vector<double>>::const_iterator itCL;
    for ( itCL = contLogs.begin(); itCL != contLogs.end(); ++itCL )
    {
        QString logName = QString::fromStdString( itCL->first );
        wellLogNames.append( logName );

        // 2018-11-09 Added MD https://github.com/OPM/ResInsight/issues/3641
        if ( logName.toUpper() == "DEPT" || logName.toUpper() == "DEPTH" || logName.toUpper() == "MD" || logName.toUpper() == "MDEPTH" )
        {
            m_depthLogName = logName;
        }
        else if ( logName.toUpper() == "TVDMSL" )
        {
            m_tvdMslLogName = logName;
        }
        else if ( logName.toUpper() == "TVDRKB" )
        {
            m_tvdRkbLogName = logName;
        }
    }

    m_wellLogChannelNames = wellLogNames;
    m_wellLogFile         = well;

    return true;
}

//--------------------------------------------------------------------------------------------------
/// Returns the close.
//--------------------------------------------------------------------------------------------------
void RigWellLogLasFile::close()
{
    if ( m_wellLogFile )
    {
        delete m_wellLogFile;
        m_wellLogFile = nullptr;
    }

    m_wellLogChannelNames.clear();
    m_depthLogName.clear();
}

//--------------------------------------------------------------------------------------------------
/// Returns the well name.
//--------------------------------------------------------------------------------------------------
QString RigWellLogLasFile::wellName() const
{
    CVF_ASSERT( m_wellLogFile );
    return RiaStringEncodingTools::fromNativeEncoded( m_wellLogFile->GetWellName().data() );
}

//--------------------------------------------------------------------------------------------------
/// Returns the date.
//--------------------------------------------------------------------------------------------------
QString RigWellLogLasFile::date() const
{
    CVF_ASSERT( m_wellLogFile );
    return QString::fromStdString( m_wellLogFile->GetDate() );
}

//--------------------------------------------------------------------------------------------------
/// Returns the well log channel names.
//--------------------------------------------------------------------------------------------------
QStringList RigWellLogLasFile::wellLogChannelNames() const
{
    return m_wellLogChannelNames;
}

//--------------------------------------------------------------------------------------------------
/// Returns the depth values.
//--------------------------------------------------------------------------------------------------
std::vector<double> RigWellLogLasFile::depthValues() const
{
    return values( m_depthLogName );
}

//--------------------------------------------------------------------------------------------------
/// Returns the tvd msl values.
//--------------------------------------------------------------------------------------------------
std::vector<double> RigWellLogLasFile::tvdMslValues() const
{
    return values( m_tvdMslLogName );
}

//--------------------------------------------------------------------------------------------------
/// Returns the tvd rkb values.
//--------------------------------------------------------------------------------------------------
std::vector<double> RigWellLogLasFile::tvdRkbValues() const
{
    return values( m_tvdRkbLogName );
}

//--------------------------------------------------------------------------------------------------
/// Returns the values.
//--------------------------------------------------------------------------------------------------
std::vector<double> RigWellLogLasFile::values( const QString& name ) const
{
    CVF_ASSERT( m_wellLogFile );

    if ( m_wellLogFile->HasContLog( name.toStdString() ) )
    {
        std::vector<double> logValues = m_wellLogFile->GetContLog( name.toStdString() );

        for ( size_t vIdx = 0; vIdx < logValues.size(); vIdx++ )
        {
            if ( m_wellLogFile->IsMissing( logValues[vIdx] ) )
            {
                // Convert missing ("NULL") values to HUGE_VAL
                logValues[vIdx] = HUGE_VAL;
            }
        }

        return logValues;
    }

    return std::vector<double>();
}

//--------------------------------------------------------------------------------------------------
/// Returns the depth unit string.
//--------------------------------------------------------------------------------------------------
QString RigWellLogLasFile::depthUnitString() const
{
    QString unit;

    NRLib::LasWell* lasWell = dynamic_cast<NRLib::LasWell*>( m_wellLogFile );
    if ( lasWell )
    {
        unit = QString::fromStdString( lasWell->depthUnit() );
    }

    return unit;
}

//--------------------------------------------------------------------------------------------------
/// Returns the well log channel unit string.
//--------------------------------------------------------------------------------------------------
QString RigWellLogLasFile::wellLogChannelUnitString( const QString& wellLogChannelName ) const
{
    QString unit;

    NRLib::LasWell* lasWell = dynamic_cast<NRLib::LasWell*>( m_wellLogFile );
    if ( lasWell )
    {
        return QString::fromStdString( lasWell->unitName( wellLogChannelName.toStdString() ) );
    }
    return "";
}

//--------------------------------------------------------------------------------------------------
/// Returns whether tvd msl channel is available.
//--------------------------------------------------------------------------------------------------
bool RigWellLogLasFile::hasTvdMslChannel() const
{
    return !m_tvdMslLogName.isEmpty();
}

//--------------------------------------------------------------------------------------------------
/// Returns whether tvd rkb channel is available.
//--------------------------------------------------------------------------------------------------
bool RigWellLogLasFile::hasTvdRkbChannel() const
{
    return !m_tvdRkbLogName.isEmpty();
}

//--------------------------------------------------------------------------------------------------
/// Returns missing value.
//--------------------------------------------------------------------------------------------------
double RigWellLogLasFile::getMissingValue() const
{
    return m_wellLogFile->GetContMissing();
}
