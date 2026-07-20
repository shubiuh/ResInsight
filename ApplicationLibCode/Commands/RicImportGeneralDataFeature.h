/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2019-     Equinor ASA
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
/// @brief Declares import general data command support.

#pragma once

#include "RiaDefines.h"

#include "cafCmdFeature.h"

#include <QStringList>

#include <memory>
#include <vector>

struct RifReaderSettings;

//==================================================================================================
/// @brief Command feature for import general data.
//==================================================================================================
class RicImportGeneralDataFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

public:
    /// @brief Data used to configure or execute open case results command workflows.
    struct OpenCaseResults
    {
        QStringList eclipseCaseFiles;
        QStringList eclipseInputFiles;
        QStringList eclipseSummaryFiles;
        QStringList roffFiles;
        QStringList vtkFiles;

        std::vector<int> createdCaseIds;

        operator bool() const
        {
            return !( eclipseCaseFiles.empty() && eclipseInputFiles.empty() && eclipseSummaryFiles.empty() && roffFiles.empty() && vtkFiles.empty() );
        }
    };

    /// @return The open Eclipse files from file names.
    static OpenCaseResults openEclipseFilesFromFileNames( const QStringList& fileNames, bool doCreateDefaultPlot, bool createDefaultView );
    /// @return The open Eclipse files from file names.
    static OpenCaseResults openEclipseFilesFromFileNames( const QStringList& fileNames,
                                                          bool               doCreateDefaultPlot,
                                                          bool               createDefaultView,
                                                          RifReaderSettings& readerSettings );
    /// @return The file names from case names.
    static QStringList     fileNamesFromCaseNames( const QStringList& caseNames );
    /// @return The Eclipse file names with.
    static QStringList     getEclipseFileNamesWithDialog( RiaDefines::ImportFileType fileTypes );

    /// @return The file pattern.
    static QString getFilePattern( const std::vector<RiaDefines::ImportFileType>& fileTypes, bool allowWildcard );

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

    /// @return The file pattern.
    static QString getFilePattern( RiaDefines::ImportFileType fileType );

    /// Performs the open file command operation.
    static void openFileDialog( RiaDefines::ImportFileType fileTypes );

    /// @return The open Eclipse case from file names.
    static bool openEclipseCaseFromFileNames( const QStringList& fileNames,
                                              bool               createDefaultView,
                                              std::vector<int>&  createdCaseIds,
                                              RifReaderSettings& readerSettings );

    /// @return The open summary case from file names.
    static bool openSummaryCaseFromFileNames( const QStringList& fileNames, bool doCreateDefaultPlot = true );

    /// @return The open Eclipse input files from file names.
    static bool openEclipseInputFilesFromFileNames( const QStringList& fileNames, bool createDefaultView, std::vector<int>& createdCaseIds );
    /// @return The open grdecl cases from file names.
    static bool openGrdeclCasesFromFileNames( const QStringList& fileNames, bool createDefaultView, std::vector<int>& createdCaseIds );
    /// @return The open grdecl case and properties from file names.
    static bool
        openGrdeclCaseAndPropertiesFromFileNames( const QStringList& fileNames, bool createDefaultView, std::vector<int>& createdCaseIds );

    /// @return The open roff files from file names.
    static bool openRoffFilesFromFileNames( const QStringList& fileNames, bool createDefaultView, std::vector<int>& createdCaseIds );
    /// @return The open roff cases from file names.
    static bool openRoffCasesFromFileNames( const QStringList& fileNames, bool createDefaultView, std::vector<int>& createdCaseIds );
    /// @return The open roff case and properties from file names.
    static bool openRoffCaseAndPropertiesFromFileNames( const QStringList& fileNames, bool createDefaultView, std::vector<int>& createdCaseIds );
};
