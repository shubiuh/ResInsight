/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2019- Equinor ASA
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
/// @brief Declares export object and field keywords command support.

#pragma once

#include "cafCmdFeature.h"

//==================================================================================================
/// @brief Command feature for export object and field keywords.
//==================================================================================================
class RicExportObjectAndFieldKeywordsFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

private:
    /// Exports object keywords.
    static void exportObjectKeywords( const QString& filePath );
    /// Exports object and field keywords.
    static void exportObjectAndFieldKeywords( const QString& filePath );

    /// Writes text to file.
    static void    writeTextToFile( const QString& filePath, const QString& text );
    /// @return The version header text.
    static QString versionHeaderText();
};
