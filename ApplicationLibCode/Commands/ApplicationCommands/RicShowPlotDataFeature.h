/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017 Statoil ASA
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
/// @brief Declares show plot data command support.

#pragma once

#include "cafCmdFeature.h"

#include <memory>
#include <vector>

class RimPlotWindow;
class RimTabbedTextProvider;

//==================================================================================================
/// @brief Command feature for show plot data.
//==================================================================================================
class RicShowPlotDataFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

protected:
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

private:
    /// Gets selection.
    void getSelection( std::vector<RimPlotWindow*>& selection ) const;

public:
    /// Shows tabbed text window.
    static void showTabbedTextWindow( std::unique_ptr<RimTabbedTextProvider> textProvider );
    /// Shows text window.
    static void showTextWindow( const QString& title, const QString& text );
};
