/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2016-     Statoil ASA
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
/// @brief Declares paste ensemble curve set command support.

#pragma once

#include "cafCmdFeature.h"
#include "cafPdmPointer.h"

#include <vector>

class RimEnsembleCurveSet;
class RimEnsembleCurveSetCollection;

//==================================================================================================
/// @brief Command feature for paste ensemble curve set.
//==================================================================================================
class RicPasteEnsembleCurveSetFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

private:
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

    /// Copies curve set and add to collection.
    static RimEnsembleCurveSet* copyCurveSetAndAddToCollection( RimEnsembleCurveSetCollection* coll, const RimEnsembleCurveSet* sourceCurveSet );

    /// @return The ensemble curve sets on clipboard.
    static std::vector<caf::PdmPointer<RimEnsembleCurveSet>> ensembleCurveSetsOnClipboard();
};
