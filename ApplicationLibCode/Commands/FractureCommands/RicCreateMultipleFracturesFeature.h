/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2018-     Equinor ASA
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
/// @brief Declares create multiple fractures command support.

#pragma once

#include "cafCmdFeature.h"

#include "RigBoundingBoxIjk.h"

#include "cafVecIjk.h"
#include "cvfAssert.h"
#include "cvfVector3.h"

#include <QPointer>

class RiuCreateMultipleFractionsUi;
class RimEclipseCase;

/// @brief Utilities for caf command workflows.
namespace caf
{
class PdmUiPropertyViewDialog;
}

//==================================================================================================
/// @brief Command feature for create multiple fractures.
//==================================================================================================
class RicCreateMultipleFracturesFeature : public caf::CmdFeature
{
    Q_OBJECT
    CAF_CMD_HEADER_INIT;

public:
    RicCreateMultipleFracturesFeature() {}

    /// Appends fractures.
    void                            appendFractures();
    /// Performs the replace fractures command operation.
    void                            replaceFractures();
    /// @return The ijk range for grid.
    RigBoundingBoxIjk<caf::VecIjk0> ijkRangeForGrid( RimEclipseCase* gridCase ) const;

private slots:
    /// Performs the slot delete and append fractures command operation.
    void slotDeleteAndAppendFractures();
    /// Performs the slot append fractures command operation.
    void slotAppendFractures();
    /// Performs the slot close command operation.
    void slotClose();

private:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;

    /// @return The multiple fractions.
    RiuCreateMultipleFractionsUi* multipleFractionsUi() const;

private:
    QString m_copyOfObject;
};
