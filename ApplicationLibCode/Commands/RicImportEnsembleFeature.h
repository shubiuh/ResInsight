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
/// @brief Declares import ensemble command support.

#pragma once

#include "RiaEnsembleNameTools.h"
#include "Summary/RiaSummaryDefines.h"

#include "cafCmdFeature.h"

#include <QString>

class RimSummaryCase;
class RimSummaryEnsemble;

//==================================================================================================
/// @brief Command feature for import ensemble.
//==================================================================================================
class RicImportEnsembleFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

public:
    /// Creates summary ensemble.
    static RimSummaryEnsemble* createSummaryEnsemble( std::vector<RimSummaryCase*> cases );
    /// @return The group summary cases.
    static RimSummaryEnsemble* groupSummaryCases( std::vector<RimSummaryCase*>     cases,
                                                  const QString&                   groupName,
                                                  RiaDefines::EnsembleGroupingMode groupingMode,
                                                  bool                             isEnsemble = false );

    /// Imports single ensemble file set.
    static RimSummaryEnsemble* importSingleEnsembleFileSet( const QStringList&               fileNames,
                                                            bool                             useEnsembleNameDialog,
                                                            RiaDefines::EnsembleGroupingMode groupingMode,
                                                            RiaDefines::FileType             fileType,
                                                            const QString&                   defaultEnsembleName = QString() );

private:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

    /// @return The ask for ensemble name.
    static QString             askForEnsembleName( const QString& suggestion );
    /// Imports single ensemble.
    static RimSummaryEnsemble* importSingleEnsemble( const QStringList&               fileNames,
                                                     bool                             useEnsembleNameDialog,
                                                     RiaDefines::EnsembleGroupingMode groupingMode,
                                                     RiaDefines::FileType             fileType,
                                                     const QString&                   defaultEnsembleName = QString() );
};
