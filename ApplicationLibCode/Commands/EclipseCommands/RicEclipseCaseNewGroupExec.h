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
/// @brief Declares Eclipse case new group exec command support.

#pragma once

#include "cafCmdExecuteCommand.h"

//==================================================================================================
/// @brief Supports Eclipse case new group exec command workflows.
//==================================================================================================
class RicEclipseCaseNewGroupExec : public caf::CmdExecuteCommand
{
public:
    /// Constructs the command object.
    RicEclipseCaseNewGroupExec();
    /// Destroys the command object.
    ~RicEclipseCaseNewGroupExec() override;

    /// @return The name.
    QString name() override;
    /// Performs the redo command operation.
    void    redo() override;
    /// Performs the undo command operation.
    void    undo() override;
};
