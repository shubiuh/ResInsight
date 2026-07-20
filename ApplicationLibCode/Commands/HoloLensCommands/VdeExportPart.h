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
/// @brief Declares vde export part command support.

#pragma once

#include "cvfObject.h"
#include "cvfPart.h"
#include "cvfTextureImage.h"

#include <QString>

//==================================================================================================
/// @brief Supports vde export part command workflows.
//==================================================================================================
class VdeExportPart
{
public:
    enum SourceObjectType
    {
        OBJ_TYPE_GRID,
        OBJ_TYPE_PIPE,
        OBJ_TYPE_UNKNOWN
    };

    enum Winding
    {
        CLOCKWISE,
        COUNTERCLOCKWISE
    };

    enum CullFace
    {
        CF_NONE,
        CF_FRONT,
        CF_BACK
    };

    enum Role
    {
        GEOMETRY,
        MESH_LINES
    };

public:
    /// Constructs the command object.
    VdeExportPart( cvf::Part* part );

    /// Sets texture image.
    void setTextureImage( const cvf::TextureImage* textureImage );
    /// Sets source object type.
    void setSourceObjectType( SourceObjectType sourceObjectType );
    /// Sets source object name.
    void setSourceObjectName( const QString& sourceObjectName );
    /// Sets source object cell set type.
    void setSourceObjectCellSetType( const QString& sourceObjectCellSetType );
    /// Sets color.
    void setColor( const cvf::Color3f& color );
    /// Sets opacity.
    void setOpacity( float opacity );
    /// Sets winding.
    void setWinding( Winding winding );
    /// Sets cull face.
    void setCullFace( CullFace cullFace );
    /// Sets role.
    void setRole( Role role );

    /// @return The part.
    const cvf::Part*         part() const;
    /// @return The texture image.
    const cvf::TextureImage* textureImage() const;

    /// @return The source object name.
    QString          sourceObjectName() const;
    /// @return The source object cell set type.
    QString          sourceObjectCellSetType() const;
    /// @return The source object type.
    SourceObjectType sourceObjectType() const;
    /// @return The color.
    cvf::Color3f     color() const;
    /// @return The opacity.
    float            opacity() const;
    /// @return The winding.
    Winding          winding() const;
    /// @return The cull face.
    CullFace         cullFace() const;
    /// @return The role.
    Role             role() const;

private:
    cvf::cref<cvf::Part>         m_part;
    cvf::cref<cvf::TextureImage> m_textureImage;

    QString          m_sourceObjectName;
    QString          m_sourceObjectCellSetType;
    SourceObjectType m_sourceObjectType;
    cvf::Color3f     m_color;
    float            m_opacity;
    Winding          m_winding;
    CullFace         m_cullFace;
    Role             m_role;
};
