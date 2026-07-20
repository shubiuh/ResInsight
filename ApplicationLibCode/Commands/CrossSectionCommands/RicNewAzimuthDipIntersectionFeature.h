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
/// @brief Declares new azimuth dip intersection command support.

#pragma once

#include "cafCmdExecuteCommand.h"
#include "cafCmdFeature.h"
#include "cafPdmPointer.h"

class RimIntersectionCollection;

//==================================================================================================
/// @brief Supports new azimuth dip intersection feature cmd command workflows.
//==================================================================================================
class RicNewAzimuthDipIntersectionFeatureCmd : public caf::CmdExecuteCommand
{
public:
    /// Constructs the command object.
    explicit RicNewAzimuthDipIntersectionFeatureCmd( RimIntersectionCollection* intersectionCollection );
    /// Destroys the command object.
    ~RicNewAzimuthDipIntersectionFeatureCmd() override;

    /// @return The name.
    QString name() override;
    /// Performs the redo command operation.
    void    redo() override;
    /// Performs the undo command operation.
    void    undo() override;

private:
    caf::PdmPointer<RimIntersectionCollection> m_intersectionCollection;
};

//==================================================================================================
/// @brief Command feature for new azimuth dip intersection.
//==================================================================================================
class RicNewAzimuthDipIntersectionFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

public:
    /// Constructs the command object.
    RicNewAzimuthDipIntersectionFeature();

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
};
