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
/// @brief Declares vde packet directory command support.

#pragma once

#include "VdeArrayDataPacket.h"

#include <QByteArray>

#include <map>
#include <memory>

//==================================================================================================
//
//
//
//==================================================================================================
/// @brief Supports vde packet directory command workflows.
class VdePacketDirectory
{
public:
    /// Constructs the command object.
    VdePacketDirectory();

    /// Adds packet.
    void                      addPacket( std::unique_ptr<VdeArrayDataPacket> packet );
    /// @return The lookup packet.
    const VdeArrayDataPacket* lookupPacket( int arrayId ) const;

    /// Removes or clears command support.
    void clear();
    /// Performs the prune unreferenced packets command operation.
    void pruneUnreferencedPackets( const std::vector<int>& packetIdsInUseArr );

    /// @return The packets as combined buffer.
    bool getPacketsAsCombinedBuffer( const std::vector<int>& packetIdsToGet, QByteArray* combinedPacketArr ) const;

private:
    using IdToPacketMap_T = std::map<int, std::unique_ptr<VdeArrayDataPacket>>;

    IdToPacketMap_T m_idToPacketMap;
};
