/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2023 Equinor ASA
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
/// @brief Declares textured section reservoir-data functionality.

#pragma once

#include "cvfArray.h"
#include "cvfColor3.h"
#include "cvfObject.h"
#include "cvfVector3.h"

#include <memory>
#include <vector>

namespace ZGYAccess
{
class SeismicSliceData;
}

namespace cvf
{
class TextureImage;
}

/// @brief Models textured section part for reservoir-data processing.
class RigTexturedSectionPart
{
public:
    RigTexturedSectionPart()
        : isRectValid( false ) {};
    ~RigTexturedSectionPart() {};

    /// Returns or processes all data valid.
    bool allDataValid() const { return isRectValid && ( sliceData != nullptr ) && texture.notNull(); };

public:
    /// Stores rect.
    cvf::Vec3dArray                              rect;
    /// Stores is rect valid.
    bool                                         isRectValid;
    /// Stores slice data.
    std::shared_ptr<ZGYAccess::SeismicSliceData> sliceData;
    /// Stores texture.
    cvf::ref<cvf::TextureImage>                  texture;
};

//==================================================================================================
///
///
//==================================================================================================
/// @brief Models textured section for reservoir-data processing.
class RigTexturedSection : public cvf::Object
{
public:
    /// Enumerates the supported what to update enum values.
    enum class WhatToUpdateEnum
    {
        UPDATE_NONE     = 0,
        UPDATE_TEXTURE  = 1,
        UPDATE_GEOMETRY = 2,
        UPDATE_DATA     = 3,
        UPDATE_ALL      = 4
    };

public:
    RigTexturedSection();
    ~RigTexturedSection() override;

    void setWhatToUpdate( WhatToUpdateEnum updateInfo, int index = -1 );
    bool isValid() const;

    void setSectionPartRect( int index, cvf::Vec3dArray rect );
    void setSectionPartData( int index, std::shared_ptr<ZGYAccess::SeismicSliceData> data );

    int  partsCount() const;
    void resize( int size );

    RigTexturedSectionPart& part( int index );

private:
    std::vector<RigTexturedSectionPart> m_sectionParts;
};
