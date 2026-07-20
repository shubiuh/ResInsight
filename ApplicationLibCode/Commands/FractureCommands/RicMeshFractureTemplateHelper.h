/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2022-     Equinor ASA
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
/// @brief Declares mesh fracture template command support.

#pragma once

#include <QString>

#include <vector>

class RimFracture;
class RimFractureTemplate;
class RimThermalFractureTemplate;

//==================================================================================================
/// @brief Helper functions for mesh fracture template command workflows.
//==================================================================================================
/// @brief Helper functions for mesh fracture template command workflows.
template <class T>
class RicMeshFractureTemplateHelper
{
public:
    /// Creates new template for fracture and update.
    static void createNewTemplateForFractureAndUpdate( RimFracture*   fracture,
                                                       const QString& title,
                                                       const QString& lastUsedDialogFallback,
                                                       const QString& fileFilter,
                                                       const QString& defaultTemplateName );

    /// Selects fracture template and update.
    static void            selectFractureTemplateAndUpdate( RimFractureTemplate* fractureTemplate );
    /// Creates new templates from files.
    static std::vector<T*> createNewTemplatesFromFiles( const std::vector<QString>& fileNames,
                                                        const QString&              defaultTemplateName,
                                                        bool                        reuseExistingTemplatesWithMatchingNames = false );
    /// Creates new templates.
    static std::vector<T*> createNewTemplates( const QString& title,
                                               const QString& lastUsedDialogFallback,
                                               const QString& fileFilter,
                                               const QString& defaultTemplateName );
};

#include "RicMeshFractureTemplateHelper.inl"
