/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2024-     Equinor ASA
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
/// @brief Declares create contour map polygon command support.

#pragma once

#include <QImage>
#include <QString>

#include <vector>

class RigContourMapProjection;
class RimPolygon;

//==================================================================================================
/// @brief Utility functions for create contour map polygon.
//==================================================================================================
namespace RicCreateContourMapPolygonTools
{
/// @return The convert binary to image.
QImage convertBinaryToImage( const std::vector<std::vector<int>>& data, QColor color, int transparency );
/// @return The convert binary to grayscale image.
QImage convertBinaryToGrayscaleImage( const std::vector<std::vector<int>>& data, int colorValue );

/// Exports vector as image.
void exportVectorAsImage( const std::vector<std::vector<int>>& data, int transparency, const QString& filename );
/// Exports vector as grayscale image.
void exportVectorAsGrayscaleImage( const std::vector<std::vector<int>>& data, const QString& filename );

/// @return The convert image to binary.
std::vector<std::vector<int>> convertImageToBinary( QImage image );
/// @return The convert to binary image.
std::vector<std::vector<int>> convertToBinaryImage( const RigContourMapProjection* contourMapProjection );

/// Creates polygon objects.
void createPolygonObjects( std::vector<std::vector<int>> image, const RigContourMapProjection* contourMapProjection );

/// @return The matching current contour map projection.
const RigContourMapProjection* findCurrentContourMapProjection();

}; // namespace RicCreateContourMapPolygonTools
