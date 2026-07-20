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
/// @brief UI for inspecting and clearing cached results from memory.

#pragma once

#include "cafPdmChildArrayField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPtrField.h"
#include "cafPdmUiItem.h"

class RimCase;
class Rim3dView;
class RigFemResultAddress;
class RigEclipseResultAddress;

//==================================================================================================
/// @brief PDM-backed tool for reporting and releasing loaded result arrays from selected cases.
///
/// The object builds a UI from a selected reservoir or geomechanical case and exposes only results
/// that are currently resident in memory. Results referenced by enabled views or result observers
/// are marked as in use so the editor can prevent unsafe selection. Clearing releases cached arrays;
/// it does not remove result definitions from the project or delete source files.
///
/// Option values store indices into address vectors rebuilt by calculateValueOptions(). Selection
/// conversion therefore asserts that those vectors and the UI option list remain synchronized.
//==================================================================================================
class RiaMemoryCleanup : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes the PDM fields used by the cleanup editor.
    RiaMemoryCleanup();

    /// Selects the case associated with @p view and refreshes available results.
    void setPropertiesFromView( Rim3dView* view );

    /// Releases the checked result arrays from the selected case and clears the selection.
    void clearSelectedResultsFromMemory();

    /// Creates and displays a process-wide report of loaded Eclipse result memory.
    static void showMemoryReport();

protected:
    /// Refreshes result options when the selected case changes.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;

private:
    /// Converts selected option indices to geomechanical result addresses.
    std::vector<RigFemResultAddress>     selectedGeoMechResults() const;

    /// Converts selected option indices to Eclipse result addresses.
    std::vector<RigEclipseResultAddress> selectedEclipseResults() const;

    /// Finds geomechanical results observed by enabled project objects.
    std::set<RigFemResultAddress>        findGeoMechCaseResultsInUse() const;

    /// Finds Eclipse results referenced by result definitions in the selected case.
    std::set<RigEclipseResultAddress>    findEclipseResultsInUse() const;

    /// Builds case choices or loaded-result choices for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;

    /// Defines the case selector, checked result list, cleanup action, and report action.
    void                          defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;

    /// @return A summary/details pair describing loaded Eclipse result memory.
    static std::pair<QString, QString> createMemoryReport();

private:
    caf::PdmPtrField<RimCase*>         m_case;            ///< Case whose loaded results are inspected.
    caf::PdmField<std::vector<size_t>> m_resultsToDelete; ///< Checked indices into the active address vector.

    std::vector<RigFemResultAddress>     m_geomResultAddresses;   ///< GeoMech option-index mapping.
    std::vector<RigEclipseResultAddress> m_eclipseResultAddresses; ///< Eclipse option-index mapping.
};
