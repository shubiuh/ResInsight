/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2020 Equinor ASA
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
/// @brief Declares facies properties import command support.

#pragma once

#include <map>
#include <vector>

class RimColorLegend;
class RimStimPlanModelTemplate;

/// @brief Utilities for cvf command workflows.
namespace cvf
{
class Color3f;
}

class QString;

//==================================================================================================
/// @brief Utility functions for facies properties import.
//==================================================================================================
class RicFaciesPropertiesImportTools
{
public:
    /// Imports facies properties from file.
    static void importFaciesPropertiesFromFile( const QString&            filePath,
                                                RimStimPlanModelTemplate* stimPlanModelTemplate,
                                                bool                      createColorLegend = false );

    /// Creates color legend match default rock colors.
    static RimColorLegend* createColorLegendMatchDefaultRockColors( const std::map<int, QString>& codeNames );

    /// @return The match default rock colors.
    static std::vector<cvf::Color3f> matchDefaultRockColors( const std::map<int, QString>& codeNames );

private:
    /// Computes edit distance.
    static int  computeEditDistance( const QString& a, const QString& b );
    /// @return The match by name.
    static bool matchByName( const QString& name, RimColorLegend* colorLegend, cvf::Color3f& color );
    /// @return The predefined color match.
    static bool predefinedColorMatch( const QString& name, RimColorLegend* colorLegend, cvf::Color3f& color );
};
