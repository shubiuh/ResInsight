/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2015-     Statoil ASA
//  Copyright (C) 2015-     Ceetron Solutions AS
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
/// @brief Declares well logs import file command support.

#pragma once

#include "cafCmdFeature.h"

#include <vector>

class RimWellLogLasFile;

//==================================================================================================
/// @brief Command feature for well logs import file.
//==================================================================================================
class RicWellLogsImportFileFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;
    /// Imports well log files.
    static std::vector<RimWellLogLasFile*> importWellLogFiles( const QStringList& wellLogFilePaths, QStringList* errorMessages );
    /// @return The well log file name filters.
    static QStringList                     wellLogFileNameFilters();

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
};
