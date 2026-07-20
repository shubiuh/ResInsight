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
/// @brief Declares import well paths command support.

#pragma once

#include "RicfCommandObject.h"

#include "cafCmdFeature.h"
#include "cafPdmField.h"

#include <QString>
#include <QStringList>

#include <vector>

class RimWellPath;

//==================================================================================================
/// @brief Supports import well paths command workflows.
//==================================================================================================
class RicImportWellPaths : public caf::CmdFeature, public RicfCommandObject
{
    RICF_HEADER_INIT;

public:
    /// Constructs the command object.
    RicImportWellPaths();
    /// Executes command support.
    caf::PdmScriptResponse execute() override;

    /// Imports well paths.
    static std::vector<RimWellPath*> importWellPaths( const QStringList& wellPathFilePaths, QStringList* errorMessages );

protected:
    /// @return The well path name filters.
    static QStringList wellPathNameFilters();

    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

protected:
    caf::PdmField<QString>              m_wellPathFolder;
    caf::PdmField<std::vector<QString>> m_wellPathFiles;
};
