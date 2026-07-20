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
/// @brief Declares import summary cases command support.

#pragma once

#include "RiaEnsembleNameTools.h"
#include "Summary/RiaSummaryDefines.h"

#include "RicRecursiveFileSearchDialog.h"
#include "cafCmdFeature.h"

#include <QString>
#include <vector>

class RicSummaryCaseRestartDialogResult;
class RimSummaryCase;

//==================================================================================================
/// @brief Command feature for import summary cases.
//==================================================================================================
class RicImportSummaryCasesFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

public:
    RicImportSummaryCasesFeature() {}

    /// Creates and add summary cases from files.
    static std::pair<bool, std::vector<RimSummaryCase*>> createAndAddSummaryCasesFromFiles( const QStringList& fileName,
                                                                                            bool               doCreateDefaultPlot );

    /// Adds summary cases.
    static void addSummaryCases( const std::vector<RimSummaryCase*>& cases );

    /// @return The run recursive summary case file search.
    static RicRecursiveFileSearchDialogResult runRecursiveSummaryCaseFileSearchDialog( const QString& dialogTitle,
                                                                                       const QString& pathCacheName );

    /// @return The run recursive summary case file search dialog with grouping.
    static RicRecursiveFileSearchDialogResult runRecursiveSummaryCaseFileSearchDialogWithGrouping( const QString& dialogTitle,
                                                                                                   const QString& pathCacheName );

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

private:
    static QString m_pathFilter;
    static QString m_fileNameFilter;
};
