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
/// @brief Declares new thermal fracture template command support.

#pragma once

#include "cafCmdFeature.h"

#include <vector>

class RimFracture;
class RimFractureTemplate;
class RimThermalFractureTemplate;

//==================================================================================================
/// @brief Command feature for new thermal fracture template.
//==================================================================================================
class RicNewThermalFractureTemplateFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;
    /// Creates new template for fracture and update.
    static void                                     createNewTemplateForFractureAndUpdate( RimFracture* fracture );
    /// Selects fracture template and update.
    static void                                     selectFractureTemplateAndUpdate( RimFractureTemplate* fractureTemplate );
    /// Creates new templates from files.
    static std::vector<RimThermalFractureTemplate*> createNewTemplatesFromFiles( const std::vector<QString>& fileNames,
                                                                                 bool reuseExistingTemplatesWithMatchingNames = false );

protected:
    /// Creates new templates.
    static std::vector<RimThermalFractureTemplate*> createNewTemplates();
    /// Executes the command for the current selection.
    void                                            onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void                                            setupActionLook( QAction* actionToSetup ) override;

    /// @return The file filter.
    static QString fileFilter();
    /// @return The title.
    static QString title();
    /// @return The default template name.
    static QString defaultTemplateName();
    /// @return The last used dialog fallback.
    static QString lastUsedDialogFallback();
};
