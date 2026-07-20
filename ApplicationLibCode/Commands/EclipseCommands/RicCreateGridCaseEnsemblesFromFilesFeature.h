/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2024-     Equinor ASA
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
/// @brief Declares create grid case ensembles from files command support.

#pragma once

#include "cafCmdFeature.h"

#include "RiaEnsembleNameTools.h"

#include <vector>

class RimEclipseCaseEnsemble;
class RimFormationNames;
class RimEclipseCase;

//==================================================================================================
/// @brief Command feature for create grid case ensembles from files.
//==================================================================================================
class RicCreateGridCaseEnsemblesFromFilesFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

    RicCreateGridCaseEnsemblesFromFilesFeature()
        : caf::CmdFeature()
        , m_pathFilter( "*" )
        , m_fileNameFilter( "*" )
    {
    }

public:
    /// Imports single grid case ensemble.
    static RimEclipseCaseEnsemble* importSingleGridCaseEnsemble( const QStringList& fileNames );
    /// Imports single grid case.
    static RimEclipseCase*         importSingleGridCase( const QString& filename );

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

    /// @return The run recursive file search.
    std::pair<QStringList, RiaDefines::EnsembleGroupingMode> runRecursiveFileSearchDialog( const QString& dialogTitle,
                                                                                           const QString& pathCacheName );

private:
    QString m_pathFilter;
    QString m_fileNameFilter;
};
