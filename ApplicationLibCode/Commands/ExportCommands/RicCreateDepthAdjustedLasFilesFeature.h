/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2023- Equinor ASA
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
/// @brief Declares create depth adjusted LAS files command support.

#pragma once

#include "cafCmdFeature.h"

class RimEclipseCase;
class RimGeoMechCase;
class RimWellPath;
class RimWellLogLasFile;

//==================================================================================================
/// @brief Command feature for create depth adjusted LAS files.
//==================================================================================================
class RicCreateDepthAdjustedLasFilesFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

public:
    /// Constructs the command object.
    RicCreateDepthAdjustedLasFilesFeature() = default;

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

private:
    /// Creates depth adjusted well log file from Eclipse case.
    void createDepthAdjustedWellLogFileFromEclipseCase( RimEclipseCase*                 eclipseCase,
                                                        RimWellPath*                    sourceWell,
                                                        RimWellLogLasFile*              soureWellLogFile,
                                                        const std::vector<RimWellPath*> destinationWells,
                                                        const std::vector<QString>&     selectedResultProperties,
                                                        const QString&                  exportFolder );

    /// Creates depth adjusted well log file from geo mech case.
    void createDepthAdjustedWellLogFileFromGeoMechCase( RimGeoMechCase*                 geoMechCase,
                                                        RimWellPath*                    sourceWell,
                                                        RimWellLogLasFile*              soureWellLogFile,
                                                        const std::vector<RimWellPath*> destinationWells,
                                                        const std::vector<QString>&     selectedResultProperties,
                                                        const QString&                  exportFolder );
};
