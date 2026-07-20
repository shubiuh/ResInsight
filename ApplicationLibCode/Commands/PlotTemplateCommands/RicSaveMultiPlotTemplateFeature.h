////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2022     Equinor ASA
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
/// @brief Declares save multi plot template command support.

#pragma once

#include "cafCmdFeature.h"

#include <set>

class RimSummaryMultiPlot;
class RicSaveMultiPlotTemplateFeatureSettings;

//==================================================================================================
/// @brief Command feature for save multi plot template.
//==================================================================================================
class RicSaveMultiPlotTemplateFeature : public caf::CmdFeature
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
    /// Creates text from object.
    static QString createTextFromObject( RimSummaryMultiPlot* summaryPlot, const RicSaveMultiPlotTemplateFeatureSettings& settings );

    /// Performs the replace strings command operation.
    static void replaceStrings( const std::set<QString>& sourceStrings,
                                const QString&           fieldKeyword,
                                const QString&           placeholderText,
                                QString&                 objectAsText );

private:
    /// @return The selected summary plot.
    RimSummaryMultiPlot* selectedSummaryPlot() const;
};
