/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2015-     Statoil ASA
//  Copyright (C) 2015-     Ceetron Solutions AS
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
/// @brief Declares new sim well intersection command support.

#pragma once

#include "cafCmdExecuteCommand.h"
#include "cafCmdFeature.h"
#include "cafPdmPointer.h"

class RimIntersectionCollection;
class RimSimWellInView;

//==================================================================================================
/// @brief Supports new sim well intersection cmd command workflows.
//==================================================================================================
class RicNewSimWellIntersectionCmd : public caf::CmdExecuteCommand
{
public:
    /// Constructs the command object.
    RicNewSimWellIntersectionCmd( RimIntersectionCollection* intersectionCollection, RimSimWellInView* simWell );
    /// Destroys the command object.
    ~RicNewSimWellIntersectionCmd() override;

    /// @return The name.
    QString name() override;
    /// Performs the redo command operation.
    void    redo() override;
    /// Performs the undo command operation.
    void    undo() override;

private:
    caf::PdmPointer<RimIntersectionCollection> m_intersectionCollection;
    caf::PdmPointer<RimSimWellInView>          m_simWell;
};

//==================================================================================================
/// @brief Command feature for new sim well intersection.
//==================================================================================================
class RicNewSimWellIntersectionFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
};
