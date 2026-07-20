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
/// @brief Declares link visible views command support.

#pragma once

#include "cafCmdFeature.h"

#include <vector>

class Rim3dView;

//==================================================================================================
/// @brief Command feature for link visible views.
//==================================================================================================
class RicLinkVisibleViewsFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

public:
    /// Performs the link views command operation.
    static void linkViews( std::vector<Rim3dView*>& views );

    /// @return The matching views.
    static std::vector<Rim3dView*> matchingViews( Rim3dView* primaryView, std::vector<Rim3dView*>& candidates );

protected:
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;

    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

private:
    /// @return The matching linkable visible views.
    static std::vector<Rim3dView*> findLinkableVisibleViews();
};
