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
/// @brief Declares create well targets command support.

#pragma once

#include "Ric3dViewPickEventHandler.h"

#include "cafPdmPointer.h"

class RimWellPathGeometryDef;
class RigWellPath;

//==================================================================================================
/// @brief Handles create well targets events.
//==================================================================================================
class RicCreateWellTargetsPickEventHandler : public Ric3dViewPickEventHandler
{
public:
    /// Constructs the command object.
    RicCreateWellTargetsPickEventHandler( RimWellPathGeometryDef* wellGeometryDef );
    /// Destroys the command object.
    ~RicCreateWellTargetsPickEventHandler();

    /// Registers as.
    void registerAsPickEventHandler() override;

protected:
    /// @return Whether the 3D pick event was handled.
    bool handle3dPickEvent( const Ric3dPickEvent& eventObject ) override;
    /// Performs the notify unregistered command operation.
    void notifyUnregistered() override;

private:
    /// @return Whether grid source object.
    static bool       isGridSourceObject( const cvf::Object* object );
    /// @return The matching hex element intersection.
    static cvf::Vec3d findHexElementIntersection( Rim3dView*             view,
                                                  const RiuPickItemInfo& pickItem,
                                                  const cvf::Vec3d&      domainRayOrigin,
                                                  const cvf::Vec3d&      domainRayEnd );

private:
    caf::PdmPointer<RimWellPathGeometryDef> m_geometryToAddTargetsTo;
};
