/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2015-     Statoil ASA
//  Copyright (C) 2015-     Ceetron Solutions AS
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
/// @brief Declares export to LAS file command support.

#pragma once

#include "cafCmdFeature.h"

#include <QString>

#include <vector>

class RimWellLogCurve;
class RimWellLogPlot;

//==================================================================================================
/// @brief Command feature for export to LAS file.
//==================================================================================================
class RicExportToLasFileFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

public:
    /// Exports to LAS files.
    static std::vector<QString> exportToLasFiles( const QString&        exportFolder,
                                                  const QString&        filePrefix,
                                                  const RimWellLogPlot* plotWindow,
                                                  bool                  exportTvdRkb,
                                                  bool                  capitalizeFileNames,
                                                  bool                  alwaysOverwrite,
                                                  double                resampleInterval,
                                                  bool                  convertCurveUnits );

    /// Exports to LAS files.
    static std::vector<QString> exportToLasFiles( const QString&                exportFolder,
                                                  const QString&                filePrefix,
                                                  std::vector<RimWellLogCurve*> curves,
                                                  const std::vector<QString>&   wellNames,
                                                  const std::vector<double>&    rkbDiffs,
                                                  bool                          capitalizeFileNames,
                                                  bool                          alwaysOverwrite,
                                                  double                        resampleInterval,
                                                  bool                          convertCurveUnits );

protected:
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
};
