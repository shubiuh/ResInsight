/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2026  Equinor ASA
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
/// @brief Declares type safe index reservoir-data functionality.

#pragma once

#include <cstddef>
#include <functional>

template <typename Tag>
/// @brief Models type safe index for reservoir-data processing.
class TypeSafeIndex
{
public:
    // Default constructor
    constexpr TypeSafeIndex() noexcept
        : m_value( 0 )
    {
    }

    // Explicit constructor from size_t (prevents implicit conversion)
    /// Returns or processes type safe index.
    explicit constexpr TypeSafeIndex( size_t value ) noexcept
        : m_value( value )
    {
    }

    // Explicit conversion to size_t
    /// Implements the operatorsize_t operation.
    explicit constexpr operator size_t() const noexcept { return m_value; }

    // Getter for when you need the value
    /// Returns or processes value.
    constexpr size_t value() const noexcept { return m_value; }

    // Comparison operators
    /// Implements the operator== operation.
    constexpr bool operator==( const TypeSafeIndex& other ) const noexcept  = default;
    /// Implements the operator<=> operation.
    constexpr auto operator<=>( const TypeSafeIndex& other ) const noexcept = default;

    // Arithmetic operators (if needed)
    /// Implements the operator++ operation.
    constexpr TypeSafeIndex& operator++() noexcept
    {
        ++m_value;
        return *this;
    }

private:
    size_t m_value;
};

// Hash support for using in std::unordered_map, etc.
namespace std
{
template <typename Tag>
/// @brief Models hash for reservoir-data processing.
struct hash<TypeSafeIndex<Tag>>
{
    /// Implements the operator operation.
    size_t operator()( const TypeSafeIndex<Tag>& idx ) const noexcept { return hash<size_t>{}( idx.value() ); }
};

} // namespace std

// Define specific types using tag structs

/// @brief Models reservoir cell index tag for reservoir-data processing.
struct ReservoirCellIndexTag
{
};
// Global cell index in the full IJK reservoir grid
/// Type alias used for reservoir cell index.
using ReservoirCellIndex = TypeSafeIndex<ReservoirCellIndexTag>;

/// @brief Models active cell index tag for reservoir-data processing.
struct ActiveCellIndexTag
{
};
// Index into the subset of active cells. This is usually the index used to access results data.
/// Type alias used for active cell index.
using ActiveCellIndex = TypeSafeIndex<ActiveCellIndexTag>;
