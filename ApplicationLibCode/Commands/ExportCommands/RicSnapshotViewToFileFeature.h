/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2016 Statoil ASA
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
/// @brief Declares snapshot view to file command support.

#pragma once

#include "cafCmdFeature.h"

class RimPlotWindow;
class RimViewWindow;
class QImage;

//==================================================================================================
/// @brief Command feature for snapshot view to file.
//==================================================================================================
class RicSnapshotViewToFileFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

public:
    /// Saves snapshot as.
    static void saveSnapshotAs( const QString& fileName, RimViewWindow* viewWindow, int width = -1, int height = -1 );
    /// Saves snapshot as.
    static void saveSnapshotAs( const QString& fileName, const QImage& image );
    /// Saves plot PDF report as.
    static void savePlotPdfReportAs( const QString& fileName, RimPlotWindow* plotWindow );

    /// Saves view window to file.
    static void saveViewWindowToFile( RimViewWindow* viewWindow, const QString& defaultFileBaseName = "image" );
    /// Saves image to file.
    static void saveImageToFile( const QImage& image, const QString& defaultFileBaseName = "image" );
    /// @return The generate save file name.
    static QString
        generateSaveFileName( const QString& defaultFileBaseName = "image", bool supportPDF = false, const QString& defaultExtension = "png" );
    /// @return The icon.
    static QIcon   icon();
    /// @return The text.
    static QString text();

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
};
