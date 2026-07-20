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
/// @brief Declares save Eclipse result as input property exec command support.

#pragma once

#include "cafCmdExecuteCommand.h"
#include "cafPdmPointer.h"

class RimEclipseCellColors;

//==================================================================================================
/// @brief Supports save Eclipse result as input property exec command workflows.
//==================================================================================================
class RicSaveEclipseResultAsInputPropertyExec : public caf::CmdExecuteCommand
{
public:
    /// Constructs the command object.
    explicit RicSaveEclipseResultAsInputPropertyExec( RimEclipseCellColors* cellColors );
    /// Destroys the command object.
    ~RicSaveEclipseResultAsInputPropertyExec() override;

    /// @return The name.
    QString name() override;
    /// Performs the redo command operation.
    void    redo() override;
    /// Performs the undo command operation.
    void    undo() override;

private:
    caf::PdmPointer<RimEclipseCellColors> m_cellColors;
};
