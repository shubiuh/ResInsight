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
/// @brief Declares vde file command support.

#pragma once

#include "VdeVizDataExtractor.h"

#include <QString>

class VdeArrayDataPacket;
class VdePacketDirectory;

class RimGridView;

//==================================================================================================
//
//
//
//==================================================================================================
/// @brief Exports vde file data.
class VdeFileExporter
{
public:
    /// Constructs the command object.
    VdeFileExporter( QString absOutputFolder );

    /// Exports to file.
    bool exportToFile( const QString& modelMetaJsonStr, const VdePacketDirectory& packetDirectory, const std::vector<int>& packetIdsToExport );
    /// Exports view contents.
    bool exportViewContents( const RimGridView& view );

private:
    /// Writes model meta JSON file.
    static bool writeModelMetaJsonFile( const QString& modelMetaJsonStr, QString fileName );
    /// Writes data packet to file.
    bool        writeDataPacketToFile( int arrayId, const VdeArrayDataPacket& packet ) const;

private:
    QString m_absOutputFolder;
};
