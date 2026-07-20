/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017-     Statoil ASA
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
/// @brief Declares ASCII export summary plot command support.

#pragma once

#include "RiaDateTimeDefines.h"
#include "cafCmdFeature.h"

class RimSummaryPlot;

//==================================================================================================
/// @brief Command feature for ASCII export summary plot.
//==================================================================================================
class RicAsciiExportSummaryPlotFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

public:
    /// @return The default export dir.
    static QString defaultExportDir();
    /// @return The file name from user.
    static QString getFileNameFromUserDialog( const QString& fileNameCandidate, const QString& defaultDir );
    /// Exports text to file.
    static bool    exportTextToFile( const QString& fileName, const QString& text );

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

private:
    /// Exports ASCII for summary plot.
    static bool exportAsciiForSummaryPlot( const QString&             fileName,
                                           const RimSummaryPlot*      selectedSummaryPlots,
                                           RiaDefines::DateTimePeriod resamplingPeriod,
                                           bool                       showTimeAsLongString );
};
