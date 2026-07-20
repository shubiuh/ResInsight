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
/// @brief Declares import formation names command support.

#pragma once

#include "cafCmdFeature.h"

class RimFormationNames;
class Rim3dView;

//==================================================================================================
/// @brief Command feature for import formation names.
//==================================================================================================
class RicImportFormationNamesFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

    /// Imports formation files.
    static RimFormationNames* importFormationFiles( const QStringList& fileNames );

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

private:
    /// Sets formation cell result and legend.
    void setFormationCellResultAndLegend( Rim3dView* activeView, QString& legendName );
};
