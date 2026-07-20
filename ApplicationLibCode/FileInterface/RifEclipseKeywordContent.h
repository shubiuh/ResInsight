/////////////////////////////////////////////////////////////////////////////////

/// @file
/// Defines parsed text and numeric content for one Eclipse deck keyword.
//
//  Copyright (C) 2021     Equinor ASA
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

#pragma once

#include <string>
#include <vector>

/// Parsed representation of an Eclipse keyword occurrence.
///
/// Both original text and expanded floating-point values are retained so callers
/// can choose lossless rewriting or numerical processing. The byte offset links the
/// occurrence back to its source file for diagnostics and selective reparsing.
class RifEclipseKeywordContent
{
public:
    std::string        keyword; ///< Keyword token without its data payload.
    std::string        content; ///< Original textual payload.
    std::vector<float> values;  ///< Parsed and repetition-expanded numeric values.
    size_t             offset;  ///< Byte offset of the occurrence in the source stream.
};
