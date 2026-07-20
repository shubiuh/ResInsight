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
/// @brief Declares new multi plot command support.

#pragma once

#include "RicfCommandObject.h"

#include "cafCmdFeature.h"
#include "cafPdmField.h"

#include <vector>

class RimPlot;

//==================================================================================================
/// @brief Command feature for new multi plot.
//==================================================================================================
class RicNewMultiPlotFeature : public caf::CmdFeature, public RicfCommandObject
{
    RICF_HEADER_INIT;

public:
    /// Constructs the command object.
    RicNewMultiPlotFeature();

    /// Executes command support.
    caf::PdmScriptResponse execute() override;

protected:
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

    /// @return The selected plots.
    static std::vector<RimPlot*> selectedPlots();

private:
    caf::PdmField<std::vector<uint64_t>> m_plots;
};
