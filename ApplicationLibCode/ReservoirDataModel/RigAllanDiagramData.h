/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) Statoil ASA
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
/// @brief Declares storage and handling of allan diagram data.

#pragma once

#include "cvfObject.h"
#include <QString>

#include <map>

/// @brief Stores allan diagram data.
class RigAllanDiagramData : public cvf::Object
{
public:
    RigAllanDiagramData();
    ~RigAllanDiagramData() override;

    /// Returns or processes formation combination to category.
    const std::map<std::pair<int, int>, int>& formationCombinationToCategory() { return m_formationCombinationToCategory; }

    /// Returns or processes formation index combination from category.
    std::pair<int, int> formationIndexCombinationFromCategory( int category )
    {
        for ( auto it : m_formationCombinationToCategory )
        {
            if ( it.second == category )
            {
                return it.first;
            }
        }

        return std::make_pair( -1, -1 );
    }

    /// Sets formation combination to categorymap.
    void setFormationCombinationToCategorymap( const std::map<std::pair<int, int>, int>& mapping )
    {
        m_formationCombinationToCategory = mapping;
    }

private:
    std::map<std::pair<int, int>, int> m_formationCombinationToCategory;
};
