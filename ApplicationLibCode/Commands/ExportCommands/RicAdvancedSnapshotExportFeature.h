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
/// @brief Declares advanced snapshot export command support.

#pragma once

#include "cafCmdFeature.h"

class RimAdvancedSnapshotExportDefinition;
class RimProject;
class Rim3dView;
class RimGridView;
class RimEclipseView;

//==================================================================================================
/// @brief Command feature for advanced snapshot export.
//==================================================================================================
class RicAdvancedSnapshotExportFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

protected:
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

public:
    /// Exports multiple snapshots.
    static void exportMultipleSnapshots( const QString& folder, RimProject* project );

    /// Exports view variations.
    static void exportViewVariations( Rim3dView* rimView, RimAdvancedSnapshotExportDefinition* msd, const QString& folder );

private:
    /// Exports view variations to folder.
    static void    exportViewVariationsToFolder( RimGridView* rimView, RimAdvancedSnapshotExportDefinition* msd, const QString& folder );
    /// @return The result name.
    static QString resultName( Rim3dView* rimView );
    /// Removes or clears view from view collection.
    static void    removeViewFromViewCollection( RimEclipseView* view );
};
