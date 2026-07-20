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
/// @brief Declares copy intersections to all views in case command support.

#pragma once

#include "cafCmdExecuteCommand.h"
#include "cafCmdFeature.h"
#include "cafPdmPointer.h"

class RimCase;
class RimExtrudedCurveIntersection;
class RimBoxIntersection;
class RimIntersectionCollection;

//==================================================================================================
/// @brief Command feature for copy intersections to all views in case.
//==================================================================================================
class RicCopyIntersectionsToAllViewsInCaseFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

protected:
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

    /// Copies intersections to other views.
    void copyIntersectionsToOtherViews( RimCase& gridCase, std::vector<RimExtrudedCurveIntersection*> intersections );
    /// Copies intersection boxes to other views.
    void copyIntersectionBoxesToOtherViews( RimCase& gridCase, std::vector<RimBoxIntersection*> intersectionBoxes );
};
