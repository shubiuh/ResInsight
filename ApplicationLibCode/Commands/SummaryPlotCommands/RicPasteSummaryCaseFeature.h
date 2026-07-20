/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017-     Statoil ASA
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
/// @brief Declares paste summary case command support.

#pragma once

#include "cafCmdFeature.h"
#include "cafPdmPointer.h"

#include <vector>

class RimSummaryCase;

//==================================================================================================
/// @brief Command feature for paste summary case.
//==================================================================================================
class RicPasteSummaryCaseFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

private:
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* action ) override;

    /// @return The summary cases.
    static std::vector<caf::PdmPointer<RimSummaryCase>> summaryCases();
    /// Removes or clears from source collection.
    static void                                         removeFromSourceCollection( RimSummaryCase* summaryCase );
};