/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2024     Equinor ASA
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
/// @brief Declares set parent well path command support.

#pragma once

#include "cafCmdFeature.h"
#include "cafPdmObject.h"
#include "cafPdmPtrField.h"

#include <QList>

#include <vector>

/// @brief Utilities for caf command workflows.
namespace caf
{
class PdmOptionItemInfo;
}

class RimWellPath;

/// @brief UI model for configuring select well path.
class RicSelectWellPathUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Constructs the command object.
    RicSelectWellPathUi();

    /// Sets well paths.
    void setWellPaths( const std::vector<RimWellPath*>& wellPaths );
    /// Sets selected well.
    void setSelectedWell( RimWellPath* selectedWell );

    /// @return The well path.
    RimWellPath* wellPath() const;

protected:
    /// @return The selectable values for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;

private:
    caf::PdmPtrField<RimWellPath*> m_selectedWellPath;
    std::vector<RimWellPath*>      m_wellPaths;
};

//==================================================================================================
/// @brief Command feature for set parent well path.
//==================================================================================================
class RicSetParentWellPathFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
};
