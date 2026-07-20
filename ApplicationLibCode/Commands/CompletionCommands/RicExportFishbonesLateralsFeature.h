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
/// @brief Declares export fishbones laterals command support.

#pragma once

#include <QFile>

#include "cafCmdFeature.h"

#include <memory>

class RimFishbonesCollection;
class RimWellPath;
class QTextStream;

//==================================================================================================
/// @brief Positive-infinity sentinel used for unbounded numeric command settings.
//==================================================================================================
#ifndef DOUBLE_INF
#define DOUBLE_INF std::numeric_limits<double>::infinity()
#endif

//==================================================================================================
/// @brief Command feature for export fishbones laterals.
//==================================================================================================
class RicExportFishbonesLateralsFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

    // static void exportFishboneLaterals(const RimWellPath* wellPath, QTextStream& stream, double mdStepSize);

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;

private:
    /// @return The selected fishbones collection.
    static RimFishbonesCollection* selectedFishbonesCollection();
};
