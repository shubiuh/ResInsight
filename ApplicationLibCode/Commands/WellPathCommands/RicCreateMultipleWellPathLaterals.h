/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2021 Equinor ASA
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
/// @brief Declares create multiple well path laterals command support.

#pragma once

#include "cafCmdFeature.h"

#include "RicCreateMultipleWellPathLateralsUi.h"

#include "cvfVector3.h"

#include <memory>

class RimModeledWellPath;

//==================================================================================================
/// @brief Supports create multiple well path laterals command workflows.
//==================================================================================================
class RicCreateMultipleWellPathLaterals : public caf::CmdFeature
{
    Q_OBJECT
    CAF_CMD_HEADER_INIT;

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

private slots:
    /// Performs the slot append fractures command operation.
    void slotAppendFractures();

private:
    /// Updates location of targets.
    void updateLocationOfTargets( RimModeledWellPath* newModeledWellPath, const cvf::Vec3d& sourceLocationOfFirstWellTarget );

    /// Updates location of completions.
    void updateLocationOfCompletions( RimModeledWellPath* newModeledWellPath, const double sourceTieInMeasuredDepth );

private:
    std::unique_ptr<RicCreateMultipleWellPathLateralsUi> m_ui;
};
