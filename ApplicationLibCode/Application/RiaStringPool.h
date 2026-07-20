/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2026     Equinor ASA
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
/// @brief Thread-safe string pool for memory-efficient shared string storage.

#pragma once

#include <cstdint>
#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <vector>

//==================================================================================================
/// @brief Thread-safe process-wide interning table for frequently repeated strings.
///
/// Each distinct string is stored once and represented externally by a compact IndexType. Lookups
/// use a shared lock; insertion upgrades to an exclusive lock and repeats the lookup to handle a
/// concurrent insertion. Index zero is reserved for the empty string and is available through
/// getEmptyIndex().
///
/// Entries are append-only and indices remain stable for the lifetime of the process. The reference
/// returned by getString() must not be retained across later insertions because vector growth can
/// invalidate references after the method releases its lock.
//==================================================================================================
class RiaStringPool
{
public:
    /// Compact external identifier for an interned string.
    using IndexType                          = uint32_t;
    /// Sentinel that can represent a missing index; it is never inserted into the pool.
    static constexpr IndexType INVALID_INDEX = static_cast<IndexType>( -1 );

    /// @return The process-wide string pool.
    static RiaStringPool& instance();

    /// Finds or interns @p str.
    /// @return A stable index that can be passed to getString().
    IndexType getIndex( const std::string& str );

    /// Resolves an interned index.
    /// @throws std::out_of_range when @p index is not present in the pool.
    /// @return A reference valid until a later insertion reallocates the internal vector.
    const std::string& getString( IndexType index ) const;

    /// @return The stable index assigned to the empty string during construction.
    IndexType getEmptyIndex() const { return m_emptyIndex; }

private:
    /// Creates the pool and reserves index zero for the empty string.
    RiaStringPool();

    /// The singleton owns synchronization and storage and cannot be copied.
    RiaStringPool( const RiaStringPool& )            = delete;
    RiaStringPool& operator=( const RiaStringPool& ) = delete;

    mutable std::shared_mutex m_mutex; ///< Guards both containers and supports concurrent reads.

    std::vector<std::string>                   m_strings;      ///< Append-only index-to-string table.
    std::unordered_map<std::string, IndexType> m_stringToIndex; ///< String-to-index interning lookup.
    IndexType                                  m_emptyIndex;    ///< Index reserved for the empty string.
};
