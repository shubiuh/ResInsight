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
/// @brief Declares vde array data packet command support.

#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

//==================================================================================================
//
//
//
//==================================================================================================
/// @brief Supports vde array data packet command workflows.
class VdeArrayDataPacket
{
public:
    enum ElementType
    {
        Unknown = 0,
        Float32 = 1,
        Uint32  = 2,
        Uint8   = 4,
    };

public:
    /// Constructs the command object.
    VdeArrayDataPacket();

    /// @return Whether valid.
    bool isValid() const;
    /// @return The array ID.
    int  arrayId() const;

    /// @return The element type.
    ElementType elementType() const;
    /// @return The element size.
    size_t      elementSize() const;
    /// @return The element count.
    size_t      elementCount() const;
    /// @return The array data.
    const char* arrayData() const;

    /// @return The image width.
    unsigned short imageWidth() const;
    /// @return The image height.
    unsigned short imageHeight() const;
    /// @return The image component count.
    unsigned char  imageComponentCount() const;

    /// @return The full packet size.
    size_t      fullPacketSize() const;
    /// @return The full packet raw ptr.
    const char* fullPacketRawPtr() const;

    /// @return The from float32 arr.
    static std::unique_ptr<VdeArrayDataPacket> fromFloat32Arr( int arrayId, const float* srcArr, size_t srcArrElementCount );
    /// @return The from uint32 arr.
    static std::unique_ptr<VdeArrayDataPacket> fromUint32Arr( int arrayId, const unsigned int* srcArr, size_t srcArrElementCount );
    /// @return The from uint8 image RGB arr.
    static std::unique_ptr<VdeArrayDataPacket> fromUint8ImageRGBArr( int                  arrayId,
                                                                     unsigned short       imageWidth,
                                                                     unsigned short       imageHeight,
                                                                     const unsigned char* srcArr,
                                                                     size_t               srcArrElementCount );

    /// @return The from raw packet buffer.
    static VdeArrayDataPacket fromRawPacketBuffer( const char* rawPacketBuffer, size_t bufferSize, std::string* errString );

private:
    /// @return The assign.
    bool          assign( int            arrayId,
                          ElementType    elementType,
                          size_t         elementCount,
                          unsigned short imageWidth,
                          unsigned short imageHeight,
                          unsigned char  imageCompCount,
                          const char*    arrayDataPtr,
                          size_t         arrayDataSizeInBytes );
    /// @return The size of element.
    static size_t sizeOfElement( ElementType elementType );

private:
    int         m_arrayId;
    ElementType m_elementType;
    size_t      m_elementCount;

    unsigned short m_imageWidth;
    unsigned short m_imageHeight;
    unsigned char  m_imageComponentCount;

    std::vector<char> m_packetBytes;
};

//==================================================================================================
//
//
//
//==================================================================================================
/// @brief Supports vde buffer reader command workflows.
class VdeBufferReader
{
public:
    /// Constructs the command object.
    VdeBufferReader( const char* buffer, size_t bufferSize );

    /// @return The uint32.
    unsigned int   getUint32( size_t byteOffset ) const;
    /// @return The uint16.
    unsigned short getUint16( size_t byteOffset ) const;
    /// @return The uint8.
    unsigned char  getUint8( size_t byteOffset ) const;

private:
    const char*  m_buffer;
    const size_t m_bufferSize;
};

//==================================================================================================
//
//
//
//==================================================================================================
/// @brief Supports vde buffer writer command workflows.
class VdeBufferWriter
{
public:
    /// Constructs the command object.
    VdeBufferWriter( char* buffer, size_t bufferSize );

    /// Sets uint32.
    void setUint32( size_t byteOffset, unsigned int val );
    /// Sets uint16.
    void setUint16( size_t byteOffset, unsigned short val );
    /// Sets uint8.
    void setUint8( size_t byteOffset, unsigned char val );

private:
    char*        m_buffer;
    const size_t m_bufferSize;
};
