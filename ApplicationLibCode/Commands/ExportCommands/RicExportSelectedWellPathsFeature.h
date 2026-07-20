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
/// @brief Declares export selected well paths command support.

#pragma once

#include "cafCmdFeature.h"

#include <QFile>

#include <memory>

class RigWellPath;
class RigWellPathGeom;
class RimWellPath;
class RicExportWellPathsUi;
class QTextStream;

//==================================================================================================
/// @brief Shared file and text-stream handles used while exporting selected well paths.
//==================================================================================================
using QFilePtr       = std::shared_ptr<QFile>;
using QTextStreamPtr = std::shared_ptr<QTextStream>;

//==================================================================================================
/// @brief Command feature for export selected well paths.
//==================================================================================================
class RicExportSelectedWellPathsFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

    /// Exports well paths to file.
    static void exportWellPathsToFile( const std::vector<RimWellPath*>& wellPaths );
    /// Exports well path.
    static void exportWellPath( const RimWellPath* wellPath, double mdStepSize, const QString& folder, bool writeProjectInfo = true );

    /// @return The open.
    static RicExportWellPathsUi* openDialog();
    /// @return The open file for export.
    static QFilePtr              openFileForExport( const QString& folderName, const QString& fileName );
    /// Creates output file stream.
    static QTextStreamPtr        createOutputFileStream( QFile& file );

    /// Writes well path geometry to stream.
    static void writeWellPathGeometryToStream( QTextStream&       stream,
                                               const RimWellPath* wellPath,
                                               const QString&     exportName,
                                               double             mdStepSize,
                                               bool               writeProjectInfo = true );

    /// Writes well path geometry to stream.
    static void writeWellPathGeometryToStream( QTextStream&       stream,
                                               const RigWellPath& wellPath,
                                               const QString&     exportName,
                                               double             mdStepSize,
                                               bool               showTextMdRkb,
                                               double             rkbOffset,
                                               bool               writeProjectInfo );

private:
    /// Writes well path geometry to stream.
    static void writeWellPathGeometryToStream( QTextStream&               stream,
                                               const QString&             exportName,
                                               const std::vector<double>& xValues,
                                               const std::vector<double>& yValues,
                                               const std::vector<double>& tvdValues,
                                               const std::vector<double>& mdValues,
                                               bool                       showTextMdRkb,
                                               bool                       writeProjectInfo );

    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
};
