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
/// @brief Declares import facies command support.

#pragma once

#include "cafCmdFeature.h"

class RimColorLegend;
/// @brief Utilities for cvf command workflows.
namespace cvf
{
class Color3f;
}

//==================================================================================================
/// @brief Command feature for import facies.
//==================================================================================================
class RicImportFaciesFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

private:
    /// Computes edit distance.
    static int  computeEditDistance( const QString& a, const QString& b );
    /// @return The match by name.
    static bool matchByName( const QString& name, RimColorLegend* colorLegend, cvf::Color3f& color );
    /// @return The predefined color match.
    static bool predefinedColorMatch( const QString& name, RimColorLegend* colorLegend, cvf::Color3f& color );
};
