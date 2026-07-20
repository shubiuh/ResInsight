/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2015-     Statoil ASA
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
/// @brief Implements flow diag result frames reservoir-data functionality.

#include <cstdlib>

#include "RigFlowDiagResultFrames.h"

//--------------------------------------------------------------------------------------------------
/// Creates a RigFlowDiagResultFrames instance.
//--------------------------------------------------------------------------------------------------
RigFlowDiagResultFrames::RigFlowDiagResultFrames( size_t frameCount )
{
    m_dataForEachFrame.resize( frameCount );
}

//--------------------------------------------------------------------------------------------------
/// Returns the frame count.
//--------------------------------------------------------------------------------------------------
size_t RigFlowDiagResultFrames::frameCount() const
{
    return m_dataForEachFrame.size();
}

//--------------------------------------------------------------------------------------------------
/// Returns the frame data.
//--------------------------------------------------------------------------------------------------
std::vector<double>& RigFlowDiagResultFrames::frameData( size_t frameIndex )
{
    return m_dataForEachFrame[frameIndex];
}

//--------------------------------------------------------------------------------------------------
/// Returns the frame data.
//--------------------------------------------------------------------------------------------------
const std::vector<double>& RigFlowDiagResultFrames::frameData( size_t frameIndex ) const
{
    return m_dataForEachFrame[frameIndex];
}
