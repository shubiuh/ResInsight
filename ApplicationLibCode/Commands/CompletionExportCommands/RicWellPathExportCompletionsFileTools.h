/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2018 Equinor ASA
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
/// @brief Declares well path export completions file command support.

#pragma once

#include <QFile>
#include <QString>

#include <memory>

class RimWellPath;

/// @brief Utility functions for well path export completions file.
class RicWellPathExportCompletionsFileTools
{
public:
    /// @return The matching well path from export name.
    static const RimWellPath* findWellPathFromExportName( const QString& wellNameForExport );

    /// @return The open file for export.
    static std::shared_ptr<QFile>
        openFileForExport( const QString& folderName, const QString& fileName, const QString& suffix, bool writeInfoHeader );

private:
    /// @return The open file.
    static std::shared_ptr<QFile> openFile( const QString& folderName, const QString& fileName, const QString& suffix );

    /// Creates project file header.
    static QString createProjectFileHeader();
};
