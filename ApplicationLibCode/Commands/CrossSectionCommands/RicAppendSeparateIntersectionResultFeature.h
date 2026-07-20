/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2019-     Equinor ASA
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
/// @brief Declares append separate intersection result command support.

#pragma once

#include "cafCmdExecuteCommand.h"
#include "cafCmdFeature.h"
#include "cafPdmPointer.h"

class RimIntersectionResultsDefinitionCollection;

//==================================================================================================
/// @brief Supports append separate intersection result feature cmd command workflows.
//==================================================================================================
class RicAppendSeparateIntersectionResultFeatureCmd : public caf::CmdExecuteCommand
{
public:
    /// Constructs the command object.
    explicit RicAppendSeparateIntersectionResultFeatureCmd( RimIntersectionResultsDefinitionCollection* intersectionCollection );
    /// Destroys the command object.
    ~RicAppendSeparateIntersectionResultFeatureCmd() override;

    /// @return The name.
    QString name() override;
    /// Performs the redo command operation.
    void    redo() override;
    /// Performs the undo command operation.
    void    undo() override;

private:
    caf::PdmPointer<RimIntersectionResultsDefinitionCollection> m_intersectionCollection;
};

//==================================================================================================
/// @brief Command feature for append separate intersection result.
//==================================================================================================
class RicAppendSeparateIntersectionResultFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
};
