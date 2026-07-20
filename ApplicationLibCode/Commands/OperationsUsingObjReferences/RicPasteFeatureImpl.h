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
/// @brief Declares paste command support.

#pragma once

#include <vector>

class QString;
class QAction;

class RimEclipseCase;
class RimGeoMechCase;
class RimIdenticalGridCaseGroup;

/// @brief Utilities for caf command workflows.
namespace caf
{
class PdmObjectGroup;
class PdmObjectHandle;
} // namespace caf

//==================================================================================================
/// @brief Shared implementation helpers for paste commands.
//==================================================================================================
class RicPasteFeatureImpl
{
public:
    /// Finds objects from clipboard refs.
    static void findObjectsFromClipboardRefs( caf::PdmObjectGroup* objectGroup );

    /// @return The matching grid case group.
    static RimIdenticalGridCaseGroup* findGridCaseGroup( caf::PdmObjectHandle* objectHandle );
    /// @return The matching Eclipse case.
    static RimEclipseCase*            findEclipseCase( caf::PdmObjectHandle* objectHandle );
    /// @return The matching geo mech case.
    static RimGeoMechCase*            findGeoMechCase( caf::PdmObjectHandle* objectHandle );

    /// Sets icon and shortcuts.
    static void setIconAndShortcuts( QAction* action );

    /// Removes or clears clipboard.
    static void clearClipboard();

private:
    /// Performs the populate object group from references command operation.
    static void populateObjectGroupFromReferences( const std::vector<QString>& referenceList, caf::PdmObjectGroup* objectGroup );
    /// Performs the references from clipboard command operation.
    static void referencesFromClipboard( std::vector<QString>& referenceList );
};
