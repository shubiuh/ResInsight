/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Oil-field collection of Eclipse cases, groups, and grid ensembles.
//
//  Copyright (C) 2011-     Statoil ASA
//  Copyright (C) 2013-     Ceetron Solutions AS
//  Copyright (C) 2011-2012 Ceetron AS
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

#pragma once

#include "cafPdmChildArrayField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPointer.h"

#include "cvfObject.h"

class RigEclipseCaseData;
class RigGridManager;
class RigMainGrid;
class RimEclipseCase;
class RimIdenticalGridCaseGroup;
class RimWellPathCollection;
class RimEclipseCaseEnsemble;
class RimReservoirGridEnsemble;

//==================================================================================================
/// @brief Owns every Eclipse grid case organization under an oil field.
///
/// Standalone cases, identical-grid groups, file-based ensembles, and reservoir-grid ensembles are
/// stored in separate child arrays. A shared RigGridManager registers compatible grids so cases can
/// reuse geometry and case-to-case operations can identify identical grids efficiently.
//==================================================================================================
class RimEclipseCaseCollection : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes empty case arrays and the runtime grid manager.
    RimEclipseCaseCollection();
    ~RimEclipseCaseCollection() override;

    /// Owned standalone Eclipse cases.
    caf::PdmChildArrayField<RimEclipseCase*>            cases;
    /// Owned groups whose cases share identical grid geometry.
    caf::PdmChildArrayField<RimIdenticalGridCaseGroup*> caseGroups;
    /// Owned Eclipse case ensembles assembled from file sets.
    caf::PdmChildArrayField<RimEclipseCaseEnsemble*>    caseEnsembles;
    /// Owned reservoir-grid ensemble definitions.
    caf::PdmChildArrayField<RimReservoirGridEnsemble*>  reservoirGridEnsembles;

    /// Closes runtime data for every contained case without deleting project definitions.
    void close();

    /// Creates a new identical-grid group using @p mainCase as its reference case.
    RimIdenticalGridCaseGroup* createIdenticalCaseGroupFromMainCase( RimEclipseCase* mainCase );
    /// Moves @p rimReservoir into @p caseGroup and registers compatible grid sharing.
    void                       insertCaseInCaseGroup( RimIdenticalGridCaseGroup* caseGroup, RimEclipseCase* rimReservoir );
    /// Detaches @p rimReservoir from every group that references it.
    void                       removeCaseFromAllGroups( RimEclipseCase* rimReservoir );

    /// Recalculates statistics cases for all identical-grid groups.
    void recomputeStatisticsForAllCaseGroups();

    /// @return Command feature names used to construct the collection's import submenu.
    static QStringList importMenuFeatureNames();

private:
    /// Adds import and collection-management commands to context menus.
    void appendMenuItems( caf::CmdFeatureMenuBuilder& menuBuilder ) const override;

private:
    /// Registers @p rimEclipseCase and returns the shared main grid selected by the manager.
    RigMainGrid*             registerCaseInGridCollection( RimEclipseCase* rimEclipseCase );
    /// Runtime registry for detecting and sharing identical grid geometry.
    cvf::ref<RigGridManager> m_gridCollection;
};
