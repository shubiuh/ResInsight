/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017     Statoil ASA
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
/// @brief Cached font registry for ResInsight UI fonts.

#pragma once

#include "cafFixedAtlasFont.h"
#include "cafFontTools.h"

#include "cvfObject.h"

#include <map>

namespace caf
{
template <typename T>
class AppEnum;
}

class RimSummaryEnsemble;

//==================================================================================================
/// @brief Manages a global cache of FixedAtlasFont instances keyed by font size.
///
/// RiaFontCache creates and caches FixedAtlasFont objects on first access and provides lookup
/// by either a FontSize enum value or an explicit point size. The cache can be cleared to
/// release font resources. Requested point sizes are first scaled for the active display DPI and
/// then quantized to one of the atlas sizes supported by FixedAtlasFont. Consequently, different
/// requested point sizes can intentionally share the same atlas object.
///
/// The cache is process-wide and is not synchronized; it is intended for use from the GUI thread.
//==================================================================================================
class RiaFontCache
{
public:
    /// Semantic font size used by preferences and UI configuration.
    using FontSize     = caf::FontTools::FontSize;
    /// PDM enum wrapper used to expose FontSize in editors and settings.
    using FontSizeEnum = caf::FontTools::FontSizeEnum;

    /// Returns the cached atlas font corresponding to a semantic font size.
    static cvf::ref<caf::FixedAtlasFont> getFont( FontSize fontSize );

    /// Returns a DPI-adjusted cached atlas font for @p pointSize.
    static cvf::ref<caf::FixedAtlasFont> getFont( int pointSize );

    /// Converts the ordinal font enum stored by 2019 project versions to a modern FontSize value.
    static FontSize                      legacyEnumToPointSize( int enumValue );

    /// Releases all cached font references; existing external references remain valid.
    static void                          clear();

private:
    /// Atlas fonts keyed by the quantized FixedAtlasFont size.
    static std::map<caf::FixedAtlasFont::FontSize, cvf::ref<caf::FixedAtlasFont>> ms_fonts;
};
