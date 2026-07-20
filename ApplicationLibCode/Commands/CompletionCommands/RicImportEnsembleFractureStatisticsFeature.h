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
/// @brief Declares import ensemble fracture statistics command support.

#pragma once

#include "cafCmdFeature.h"

#include "RiaEnsembleNameTools.h"

#include <QString>

//==================================================================================================
/// @brief Command feature for import ensemble fracture statistics.
//==================================================================================================
class RicImportEnsembleFractureStatisticsFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

public:
    RicImportEnsembleFractureStatisticsFeature() {}

    /// @return The run recursive file search.
    static std::pair<QStringList, RiaDefines::EnsembleGroupingMode> runRecursiveFileSearchDialog( const QString& dialogTitle,
                                                                                                  const QString& pathCacheName );

protected:
    /// Imports single ensemble fracture statistics.
    static void importSingleEnsembleFractureStatistics( const QStringList& fileNames );

    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

private:
    static QString m_pathFilter;
    static QString m_fileNameFilter;
};
