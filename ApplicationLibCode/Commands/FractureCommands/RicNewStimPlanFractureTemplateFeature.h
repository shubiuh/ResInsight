/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017-2018 Statoil ASA
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
/// @brief Declares new stim plan fracture template command support.

#pragma once

#include "cafCmdFeature.h"

#include <vector>

class RimFracture;
class RimFractureTemplate;
class RimStimPlanFractureTemplate;

//==================================================================================================
/// @brief Command feature for new stim plan fracture template.
//==================================================================================================
class RicNewStimPlanFractureTemplateFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;
    /// Creates new template for fracture and update.
    static void                                      createNewTemplateForFractureAndUpdate( RimFracture* fracture );
    /// Selects fracture template and update.
    static void                                      selectFractureTemplateAndUpdate( RimFractureTemplate* fractureTemplate );
    /// Creates new templates from files.
    static std::vector<RimStimPlanFractureTemplate*> createNewTemplatesFromFiles( const std::vector<QString>& fileNames,
                                                                                  bool reuseExistingTemplatesWithMatchingNames = false );

protected:
    /// Creates new templates.
    static std::vector<RimStimPlanFractureTemplate*> createNewTemplates();
    /// Executes the command for the current selection.
    void                                             onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void                                             setupActionLook( QAction* actionToSetup ) override;

    /// @return The file filter.
    static QString fileFilter();
    /// @return The title.
    static QString title();
    /// @return The default template name.
    static QString defaultTemplateName();
    /// @return The last used dialog fallback.
    static QString lastUsedDialogFallback();
};
