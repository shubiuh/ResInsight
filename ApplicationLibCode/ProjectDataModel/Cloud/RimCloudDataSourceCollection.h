/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Project-owned SUMO data-source discovery and ensemble creation model.
//
//  Copyright (C) 2024     Equinor ASA
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

#include "Cloud/RiaSumoConnector.h"

#include <QPointer>
#include <QString>

class RimSummarySumoDataSource;

//==================================================================================================
/// @brief Coordinates SUMO authentication, hierarchical selection, and imported summary sources.
///
/// The collection owns the project-persistent RimSummarySumoDataSource objects while a guarded
/// connector performs authentication and metadata requests. Field, case, and ensemble selectors
/// form a dependency chain: changing an upstream selection clears downstream values. Button-like
/// PDM fields create data sources alone or create corresponding summary ensembles and default plots.
//==================================================================================================
class RimCloudDataSourceCollection : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes selection fields and creates the application-provided SUMO connector.
    RimCloudDataSourceCollection();

    /// @return The cloud-data collection of the current project's active oil field.
    static RimCloudDataSourceCollection* instance();

    /// @return Project-owned SUMO data sources in stored order.
    std::vector<RimSummarySumoDataSource*> sumoDataSources() const;

    /// Creates, loads, and plots one summary ensemble for each entry in @p dataSources.
    static void createEnsemblesFromSelectedDataSources( const std::vector<RimSummarySumoDataSource*>& dataSources );

private:
    /// Handles authentication, dependent-selection resets, and add-button actions.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;

    /// Lazily fetches valid field, case, and ensemble choices from SUMO.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;
    /// Shows selection and creation controls only after authentication succeeds.
    void                          defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;
    /// Assigns action labels to the proxy push-button fields.
    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;

    /// Creates missing sources for selected ensembles and returns only the newly created objects.
    std::vector<RimSummarySumoDataSource*> addDataSources();
    /// Creates data sources and corresponding loaded summary ensembles and plots.
    void                                   addEnsembles();

private:
    /// Proxy action that starts interactive SUMO authentication.
    caf::PdmField<bool>                 m_authenticate;
    /// Selected SUMO field/asset name.
    caf::PdmField<QString>              m_sumoFieldName;
    /// Selected SUMO case identifier within the field.
    caf::PdmField<QString>              m_sumoCaseId;
    /// Selected ensemble names within the case.
    caf::PdmField<std::vector<QString>> m_sumoEnsembleNames;

    /// Proxy action that imports selected metadata as data-source objects.
    caf::PdmField<bool>                                m_addDataSources;
    /// Proxy action that imports sources and creates summary ensembles.
    caf::PdmField<bool>                                m_addEnsembles;
    /// Project-owned imported SUMO source definitions.
    caf::PdmChildArrayField<RimSummarySumoDataSource*> m_sumoDataSources;

    /// Guarded connector supplied by the application; may expire during shutdown.
    QPointer<RiaSumoConnector> m_sumoConnector;
};
