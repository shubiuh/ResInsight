/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) Statoil ASA
//  Copyright (C) Ceetron Solutions AS
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
/// @brief Declares extraction of well log extractor.

#pragma once

#include "RigHexIntersectionTools.h"
#include "RigWellLogExtractionTools.h"

#include "cvfObject.h"
#include "cvfStructGrid.h"
#include "cvfVector3.h"

#include <map>
#include <optional>
#include <vector>

//==================================================================================================
///
//==================================================================================================
/// @brief Models well path cell intersection info for reservoir-data processing.
struct WellPathCellIntersectionInfo
{
    /// Stores glob cell index.
    size_t     globCellIndex;
    /// Stores start point.
    cvf::Vec3d startPoint;
    /// Stores end point.
    cvf::Vec3d endPoint;
    /// Stores start md.
    double     startMD;
    /// Stores end md.
    double     endMD;
    /// Stores intersection lengths in cell cs.
    cvf::Vec3d intersectionLengthsInCellCS;

    /// Stores intersected cell face in.
    cvf::StructGridInterface::FaceType intersectedCellFaceIn;
    /// Stores intersected cell face out.
    cvf::StructGridInterface::FaceType intersectedCellFaceOut;

    /// Returns or processes start tvd.
    double startTVD() const { return -startPoint.z(); }
    /// Returns or processes end tvd.
    double endTVD() const { return -endPoint.z(); }
};

class RigWellPath;

//==================================================================================================
///
//==================================================================================================
/// @brief Extracts well log extractor.
class RigWellLogExtractor : public cvf::Object
{
public:
    RigWellLogExtractor( const RigWellPath* wellpath, const std::string& wellCaseErrorMsgName );
    ~RigWellLogExtractor() override;

    const std::vector<double>&                             cellIntersectionMDs() const;
    const std::vector<double>&                             cellIntersectionTVDs() const;
    const std::vector<size_t>&                             intersectedCellsGlobIdx() const;
    const std::vector<cvf::Vec3d>&                         intersections() const;
    const std::vector<cvf::StructGridInterface::FaceType>& intersectedCellFaces() const;

    const RigWellPath* wellPathGeometry() const;

    std::vector<WellPathCellIntersectionInfo> cellIntersectionInfosAlongWellPath() const;

    void resampleIntersections( double maxDistanceBetweenIntersections );

    std::optional<double> averageMdForCell( size_t cellIndex ) const;

protected:
    static void insertIntersectionsInMap( const std::vector<HexIntersectionInfo>&                   intersections,
                                          cvf::Vec3d                                                p1,
                                          double                                                    md1,
                                          cvf::Vec3d                                                p2,
                                          double                                                    md2,
                                          double                                                    tolerance,
                                          std::map<RigMDCellIdxEnterLeaveKey, HexIntersectionInfo>* uniqueIntersections );

    void populateReturnArrays( std::map<RigMDCellIdxEnterLeaveKey, HexIntersectionInfo>& uniqueIntersections );
    void appendIntersectionToArrays( double measuredDepth, const HexIntersectionInfo& intersection, QStringList* errorMessages );

    /// Calculates length in cell.
    virtual cvf::Vec3d calculateLengthInCell( size_t cellIndex, const cvf::Vec3d& startPoint, const cvf::Vec3d& endPoint ) const = 0;

protected:
    /// Stores well path geometry.
    cvf::cref<RigWellPath> m_wellPathGeometry;
    /// Stores well case error msg name.
    std::string            m_wellCaseErrorMsgName;

private:
    std::vector<cvf::Vec3d>                         m_intersections;
    std::vector<size_t>                             m_intersectedCellsGlobIdx;
    std::vector<cvf::StructGridInterface::FaceType> m_intersectedCellFaces;
    std::vector<double>                             m_intersectionMeasuredDepths;
    std::vector<double>                             m_intersectionTVDs;
};
