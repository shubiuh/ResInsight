/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2020- Equinor ASA
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
/// @brief Declares stack selected curves command support.

#pragma once

#include "RicfCommandObject.h"
#include "RimStackablePlotCurve.h"

#include "cafCmdFeature.h"
#include "cafPdmPtrArrayField.h"

#include <vector>
//==================================================================================================
/// @brief Command feature for stack selected curves.
//==================================================================================================
class RicStackSelectedCurvesFeature : public caf::CmdFeature, public RicfCommandObject
{
    RICF_HEADER_INIT;

public:
    /// Constructs the command object.
    RicStackSelectedCurvesFeature();
    /// Executes command support.
    caf::PdmScriptResponse execute() override;

    /// @return The plot curves from selection.
    static std::vector<RimStackablePlotCurve*> plotCurvesFromSelection( const std::vector<caf::PdmUiItem*>& selectedItems );
    /// @return The subset of plot curves from stacking.
    static std::vector<RimStackablePlotCurve*> subsetOfPlotCurvesFromStacking( const std::vector<RimStackablePlotCurve*>& plotCurves,
                                                                               bool                                       isStacked );

protected:
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

private:
    caf::PdmPtrArrayField<RimStackablePlotCurve*> m_curves;
};
