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
/// @brief Declares export Eclipse sector model command support.

#pragma once

#include "cafCmdFeature.h"

#include "cafVecIjk.h"

#include "cvfArray.h"
#include "cvfVector3.h"

#include <expected>

class RimEclipseView;
class RimEclipseCase;
class RicExportEclipseSectorModelUi;
class RifOpmFlowDeckFile;
class RigSimWellData;

/// @brief Utilities for OPM command workflows.
namespace Opm
{
class DeckRecord;
class DeckKeyword;
} // namespace Opm

//==================================================================================================
/// @brief Command feature for export Eclipse sector model.
//==================================================================================================
class RicExportEclipseSectorModelFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

public:
    /// Performs the open dialog and execute command command operation.
    static void openDialogAndExecuteCommand( RimEclipseView* view );
    /// Executes command.
    static void executeCommand( RimEclipseView* view, const RicExportEclipseSectorModelUi& exportSettings, const QString& logPrefix );
    /// @return The selected view.
    static RimEclipseView* selectedView();

protected:
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

private:
    /// Exports grid.
    static void exportGrid( RimEclipseView* view, const RicExportEclipseSectorModelUi& exportSettings );
    /// Exports faults.
    static void exportFaults( RimEclipseView* view, const RicExportEclipseSectorModelUi& exportSettings );
    /// Exports parameters.
    static void exportParameters( RimEclipseView* view, const RicExportEclipseSectorModelUi& exportSettings );
};
