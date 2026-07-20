/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2011-2012 Statoil ASA, Ceetron AS
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
/// @brief Implements local grid reservoir-data functionality.

#include "RigLocalGrid.h"

RigLocalGrid::RigLocalGrid( RigMainGrid* mainGrid )
    : RigGridBase( mainGrid )
    , m_parentGrid( nullptr )
    , m_isTempGrid( false )
    , m_associatedWellPathName( "" )
{
}

RigLocalGrid::~RigLocalGrid()
{
}

//--------------------------------------------------------------------------------------------------
/// Returns the parent grid.
//--------------------------------------------------------------------------------------------------
RigGridBase* RigLocalGrid::parentGrid() const
{
    return m_parentGrid;
}

//--------------------------------------------------------------------------------------------------
/// Sets parent grid.
//--------------------------------------------------------------------------------------------------
void RigLocalGrid::setParentGrid( RigGridBase* parentGrid )
{
    m_parentGrid = parentGrid;
}

//--------------------------------------------------------------------------------------------------
/// Sets as temp grid.
//--------------------------------------------------------------------------------------------------
void RigLocalGrid::setAsTempGrid( bool isTemp )
{
    m_isTempGrid = isTemp;
}

//--------------------------------------------------------------------------------------------------
/// Returns whether temp grid.
//--------------------------------------------------------------------------------------------------
bool RigLocalGrid::isTempGrid() const
{
    return m_isTempGrid;
}

//--------------------------------------------------------------------------------------------------
/// Sets associated well path name.
//--------------------------------------------------------------------------------------------------
void RigLocalGrid::setAssociatedWellPathName( const std::string& wellPathName )
{
    m_associatedWellPathName = wellPathName;
}

//--------------------------------------------------------------------------------------------------
/// Returns the associated well path name.
//--------------------------------------------------------------------------------------------------
const std::string& RigLocalGrid::associatedWellPathName() const
{
    return m_associatedWellPathName;
}
