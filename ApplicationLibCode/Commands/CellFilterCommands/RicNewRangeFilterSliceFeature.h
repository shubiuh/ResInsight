/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2020 Equinor ASA
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
/// @brief Declares new range filter slice command support.

#pragma once

#include "cafCmdFeature.h"

//==================================================================================================
/// @brief Command feature for new range filter slice.
//==================================================================================================
class RicNewRangeFilterSliceFeature : public caf::CmdFeature
{
protected:
    /// Constructs the command object.
    RicNewRangeFilterSliceFeature( QString cmdText, QString radialText, int sliceDirection );

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

    /// @return Whether main grid radial.
    static bool isMainGridRadial();

    QString m_sliceText;
    QString m_radialText;
    int     m_sliceDirection;
};
