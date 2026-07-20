/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2020-     Equinor ASA
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
/// @brief Declares elastic properties import command support.

#pragma once

#include <QString>
#include <vector>

class RimStimPlanModelTemplate;

//--------------------------------------------------------------------------------------------------
/// @brief Utility functions for elastic properties import.
//--------------------------------------------------------------------------------------------------
class RicElasticPropertiesImportTools
{
public:
    /// Imports elastic properties from file.
    static void importElasticPropertiesFromFile( const QString&            filePath,
                                                 RimStimPlanModelTemplate* stimPlanModelTemplate,
                                                 const QString&            formationWildCard = "1*" );

private:
    // Hidden to avoid instantiation
    RicElasticPropertiesImportTools();

    /// @return The formation names.
    static std::vector<QString> getFormationNames();
};
