/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2015-     Statoil ASA
//  Copyright (C) 2019-     Equinor ASA
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
/// @brief Declares command support command support.

#pragma once

#include "RiuPickItemInfo.h"

#include "cafCmdFeature.h"
#include "cafPickEventHandler.h"

#include "cvfObject.h"
#include "cvfVector3.h"

/// @brief Utilities for cvf command workflows.
namespace cvf
{
class Part;
}

class Rim3dView;

//==================================================================================================
/// @brief Supports 3D pick event command workflows.
//==================================================================================================
class Ric3dPickEvent : public caf::PickEvent
{
public:
    Ric3dPickEvent( const std::vector<RiuPickItemInfo>& pickItemInfos, Rim3dView* view, Qt::KeyboardModifiers keyboardModifiers )
        : m_pickItemInfos( pickItemInfos )
        , m_view( view )
        , m_keyboardModifiers( keyboardModifiers )
    {
    }

    std::vector<RiuPickItemInfo> m_pickItemInfos;
    Rim3dView*                   m_view;
    Qt::KeyboardModifiers        m_keyboardModifiers;
};

//==================================================================================================
/// A static always-on pick handler used in the RiuViewerCommand
//==================================================================================================
class RicDefaultPickEventHandler
{
public:
    /// @return Whether the 3D pick event was handled.
    virtual bool handle3dPickEvent( const Ric3dPickEvent& eventObject ) = 0;
};
