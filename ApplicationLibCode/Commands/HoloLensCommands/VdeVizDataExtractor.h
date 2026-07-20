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
/// @brief Declares vde viz data extractor command support.

#pragma once

#include "cvfArray.h"
#include "cvfColor3.h"
#include "cvfString.h"
#include "cvfTextureImage.h"
#include "cvfVector3.h"

#include <QString>

#include <memory>

class VdeArrayDataPacket;
class VdePacketDirectory;
class VdeExportPart;
class VdeCachingHashedIdFactory;

class RimGridView;

//==================================================================================================
//
//
//
//==================================================================================================
/// @brief Data used to configure or execute vde mesh command workflows.
struct VdeMesh
{
    QString meshSourceObjTypeStr;
    QString meshSourceObjName;

    cvf::Color3f color;
    float        opacity;
    /// @return The none.
    QString      cullFaceModeStr; // front, back or none (or empty)

    int                          verticesPerPrimitive;
    cvf::cref<cvf::Vec3fArray>   vertexArr;
    cvf::cref<cvf::Vec2fArray>   texCoordArr;
    std::vector<cvf::uint>       connArr;
    cvf::cref<cvf::TextureImage> texImage;

    VdeMesh()
        : color( 1, 1, 1 )
        , opacity( 1 )
        , verticesPerPrimitive( -1 )
    {
    }
};

//==================================================================================================
//
// The set of array IDs that are needed for a mesh
//
//==================================================================================================
/// @brief Data used to configure or execute vde mesh array ids command workflows.
struct VdeMeshArrayIds
{
    int vertexArrId;
    int connArrId;
    int texImageArrId;
    int texCoordsArrId;

    VdeMeshArrayIds()
        : vertexArrId( -1 )
        , connArrId( -1 )
        , texImageArrId( -1 )
        , texCoordsArrId( -1 )
    {
    }
};

//==================================================================================================
//
//
//
//==================================================================================================
/// @brief Supports vde viz data extractor command workflows.
class VdeVizDataExtractor
{
public:
    /// Constructs the command object.
    VdeVizDataExtractor( const RimGridView& view, VdeCachingHashedIdFactory* cachingIdFactory );

    /// Performs the extract view contents command operation.
    void extractViewContents( QString* modelMetaJsonStr, std::vector<int>* allReferencedArrayIds, VdePacketDirectory* packetDirectory );

private:
    /// Builds mesh array.
    static std::vector<std::unique_ptr<VdeMesh>> buildMeshArray( const std::vector<VdeExportPart>& exportPartsArr );
    /// Creates mesh from export part.
    static std::unique_ptr<VdeMesh>              createMeshFromExportPart( const VdeExportPart& exportPart );
    /// Creates model meta JSON string.
    static QString                               createModelMetaJsonString( const std::vector<std::unique_ptr<VdeMesh>>&           meshArr,
                                                                            const std::vector<VdeMeshArrayIds>&                    meshContentIdsArr,
                                                                            const std::vector<std::pair<cvf::Vec3f, cvf::String>>& labelAndPositionsArr );
    /// Performs the debug compare packets command operation.
    static void debugComparePackets( const VdeArrayDataPacket& packetA, const VdeArrayDataPacket& packetB );

private:
    const RimGridView&         m_view;
    VdeCachingHashedIdFactory* m_cachingIdFactory;
};
