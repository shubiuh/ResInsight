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
/// @brief Declares vde caching hashed ID factory command support.

#pragma once

#include <cstddef>
#include <cstdint>

#include <map>

//==================================================================================================
//
//
//
//==================================================================================================
/// @brief Supports vde caching hashed ID factory command workflows.
class VdeCachingHashedIdFactory
{
public:
    enum ArrayRole
    {
        VertexArr,
        ConnArr,
        TexImage,
        TexCoordsArr
    };

public:
    /// Constructs the command object.
    VdeCachingHashedIdFactory();

    /// @return The or create ID for float arr.
    int getOrCreateIdForFloatArr( ArrayRole arrayRole, const void* floatArr, size_t elementCount );
    /// @return The or create ID for uint32 arr.
    int getOrCreateIdForUint32Arr( ArrayRole arrayRole, const unsigned int* uint32Arr, size_t elementCount );
    /// @return The or create ID for uint8 arr.
    int getOrCreateIdForUint8Arr( ArrayRole arrayRole, const unsigned char* uint8Arr, size_t elementCount );

    /// @return The last assigned ID.
    int lastAssignedId() const;

private:
    enum ElementType
    {
        Float32,
        Uint32,
        Uint8,
    };

    /// @brief Data used to configure or execute key command workflows.
    struct Key
    {
        std::pair<uint64_t, uint64_t> hashVal;
        ArrayRole                     role;
        ElementType                   elementType;
        size_t                        elementCount;

        /// Provides the corresponding operator for command data.
        bool operator<( const Key& other ) const;
    };

private:
    /// @return The or create ID for arr of type.
    int getOrCreateIdForArrOfType( ArrayRole arrayRole, ElementType elementType, size_t elementSizeInBytes, const void* data, size_t elementCount );

private:
    std::map<Key, int> m_keyToIdMap;
    int                m_lastUsedId;
};
