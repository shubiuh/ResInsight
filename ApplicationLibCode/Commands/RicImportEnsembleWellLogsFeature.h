/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2021-     Equinor ASA
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
/// @brief Declares import ensemble well logs command support.

#pragma once

#include "cafCmdFeature.h"

#include "RiaEnsembleNameTools.h"

#include <QString>

class RimEnsembleWellLogs;

//==================================================================================================
/// @brief Command feature for import ensemble well logs.
//==================================================================================================
class RicImportEnsembleWellLogsFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

    /// Constructs the command object.
    RicImportEnsembleWellLogsFeature();

    /// Creates ensemble well logs from files.
    static std::vector<RimEnsembleWellLogs*> createEnsembleWellLogsFromFiles( const QStringList&               fileNames,
                                                                              RiaDefines::EnsembleGroupingMode groupingMode );

private:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

    /// @return The run recursive file search.
    std::pair<QStringList, RiaDefines::EnsembleGroupingMode> runRecursiveFileSearchDialog( const QString& dialogTitle,
                                                                                           const QString& pathCacheName );

    /// Creates single ensemble well logs from files.
    static RimEnsembleWellLogs* createSingleEnsembleWellLogsFromFiles( const QStringList&               fileNames,
                                                                       RiaDefines::EnsembleGroupingMode groupingMode );

private:
    QString m_pathFilter;
    QString m_fileNameFilter;
};
