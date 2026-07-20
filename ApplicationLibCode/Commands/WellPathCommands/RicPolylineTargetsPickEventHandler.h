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
/// @brief Declares polyline targets command support.

#pragma once

#include "Ric3dViewPickEventHandler.h"
#include "RimPolylinePickerInterface.h"

//==================================================================================================
/// @brief Handles polyline targets events.
//==================================================================================================
class RicPolylineTargetsPickEventHandler : public Ric3dViewPickEventHandler
{
public:
    /// Constructs the command object.
    RicPolylineTargetsPickEventHandler( RimPolylinePickerInterface* picker );
    /// Destroys the command object.
    ~RicPolylineTargetsPickEventHandler();

    /// Registers as.
    void registerAsPickEventHandler() override;

protected:
    /// @return Whether the 3D pick event was handled.
    bool handle3dPickEvent( const Ric3dPickEvent& eventObject ) override;
    /// Performs the notify unregistered command operation.
    void notifyUnregistered() override;

private:
    RimPolylinePickerInterface* m_objectDef;
};
