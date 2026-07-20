/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017 Statoil ASA
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
/// @brief Declares new fishbones subs at measured depth command support.

#pragma once

#include "RicNewFishbonesSubsFeature.h"
#include "RimFishbonesDefines.h"

//==================================================================================================
/// @brief Command feature for new fishbones subs at measured depth.
//==================================================================================================
class RicNewFishbonesSubsAtMeasuredDepthFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

private:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;

    /// Creates fishbones.
    void createFishbones( const RimFishbonesDefines::RicFishbonesSystemParameters& customParameters );

    /// Performs the on drilling standard command operation.
    void onDrillingStandard();
    /// Performs the on drilling extended command operation.
    void onDrillingExtended();
    /// Performs the on acid jetting command operation.
    void onAcidJetting();
};
