/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2018-     Equinor ASA
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
/// @brief Declares contour map command support.

#pragma once

#include "RicPickEventHandler.h"

//==================================================================================================
/// @brief Handles contour map events.
//==================================================================================================
class RicContourMapPickEventHandler : public RicDefaultPickEventHandler
{
public:
    /// @return The instance.
    static RicContourMapPickEventHandler* instance();

protected:
    /// @return Whether the 3D pick event was handled.
    bool handle3dPickEvent( const Ric3dPickEvent& eventObject ) override;
};
