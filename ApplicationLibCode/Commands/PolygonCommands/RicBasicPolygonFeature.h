////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2024     Equinor ASA
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
/// @brief Declares basic polygon command support.

#pragma once

#include "cafCmdFeature.h"

#include <vector>

class RimPolygon;

//==================================================================================================
/// @brief Command feature for basic polygon.
//==================================================================================================
class RicBasicPolygonFeature : public caf::CmdFeature
{
public:
    /// Constructs the command object.
    RicBasicPolygonFeature( bool multiSelectSupported );

protected:
    /// @return The selected polygons.
    std::vector<RimPolygon*> selectedPolygons() const;
    /// @return Whether the command is available for the current selection.
    bool                     isCommandEnabled() const override;

private:
    bool m_multiSelectSupported;
};
