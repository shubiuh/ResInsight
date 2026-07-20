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
/// @brief Declares point tangent manipulator command support.

#pragma once

#include "cvfObject.h"

#include "cvfCollection.h"
#include "cvfColor4.h"
#include "cvfVector3.h"

#include <map>

/// @brief Utilities for cvf command workflows.
namespace cvf
{
class ModelBasicList;
class Part;
class DrawableGeo;
class Ray;
class HitItem;
class String;

template <typename>
class Array;
using Vec3fArray = Array<Vec3f>;
using UIntArray  = Array<uint>;

} // namespace cvf

/// @brief Builds and manages visualization parts for point tangent manipulator.
class RicPointTangentManipulatorPartMgr : public cvf::Object
{
public:
    enum class HandleType
    {
        HORIZONTAL_PLANE,
        VERTICAL_AXIS,
        PRESCRIBED_POLYLINE,
        AZIMUTH,
        INCLINATION,
        NONE
    };

public:
    /// Constructs the command object.
    RicPointTangentManipulatorPartMgr();
    /// Destroys the command object.
    ~RicPointTangentManipulatorPartMgr() override;

    /// Sets origin.
    void setOrigin( const cvf::Vec3d& origin );
    /// Sets tangent.
    void setTangent( const cvf::Vec3d& tangent );
    /// Sets handle size.
    void setHandleSize( double handleSize );
    /// Performs the origin and tangent command operation.
    void originAndTangent( cvf::Vec3d* origin, cvf::Vec3d* tangent );
    /// Sets polyline.
    void setPolyline( const std::vector<cvf::Vec3d>& polyline );

    /// @return Whether manipulator active.
    bool isManipulatorActive() const;
    /// Performs the try to activate manipulator command operation.
    void tryToActivateManipulator( const cvf::HitItem* hitItem );
    /// Updates manipulator from ray.
    void updateManipulatorFromRay( const cvf::Ray* ray );
    /// Performs the end manipulator command operation.
    void endManipulator();

    /// Appends parts to model.
    void appendPartsToModel( cvf::ModelBasicList* model );

private:
    /// Creates geometry only.
    void createGeometryOnly();
    /// Performs the recreate all geometry and parts command operation.
    void recreateAllGeometryAndParts();

    /// Creates horizontal plane handle.
    void                       createHorizontalPlaneHandle();
    /// Creates horizontal plane geo.
    cvf::ref<cvf::DrawableGeo> createHorizontalPlaneGeo();

    /// Creates vertical axis handle.
    void                       createVerticalAxisHandle();
    /// Creates vertical axis geo.
    cvf::ref<cvf::DrawableGeo> createVerticalAxisGeo();

    /// Creates polyline handle.
    void                       createPolylineHandle();
    /// Creates polyline geo.
    cvf::ref<cvf::DrawableGeo> createPolylineGeo();

    /// Adds handle part.
    void addHandlePart( cvf::DrawableGeo* geo, const cvf::Color4f& color, HandleType handleId, const cvf::String& partName );

    /// Adds active mode part.
    void addActiveModePart( cvf::DrawableGeo* geo, const cvf::Color4f& color, HandleType handleId, const cvf::String& partName );

    /// Creates triangel drawable geo.
    static cvf::ref<cvf::DrawableGeo> createTriangelDrawableGeo( cvf::Vec3fArray* triangleVertexArray );
    /// Creates indexed triangel drawable geo.
    static cvf::ref<cvf::DrawableGeo> createIndexedTriangelDrawableGeo( cvf::Vec3fArray* triangleVertexArray, cvf::UIntArray* triangleIndices );
    /// Creates part.
    static cvf::ref<cvf::Part> createPart( cvf::DrawableGeo* geo, const cvf::Color4f& color, const cvf::String& partName );

private:
    std::map<HandleType, cvf::ref<cvf::Part>> m_handleParts; // These arrays have the same length
    cvf::Collection<cvf::Part>                m_activeDragModeParts;

    cvf::Vec3d m_origin;
    cvf::Vec3d m_tangent;
    double     m_handleSize;
    bool       m_isGeometryUpdateNeeded;

    std::vector<cvf::Vec3d> m_polyline;

    HandleType m_activeHandle;
    cvf::Vec3d m_initialPickPoint;
    cvf::Vec3d m_tangentOnStartManipulation;
    cvf::Vec3d m_originOnStartManipulation;
};
