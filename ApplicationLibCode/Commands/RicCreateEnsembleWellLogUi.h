/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2021-     Equinor ASA
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
/// @brief Declares create ensemble well log command support.

#pragma once

#include "RiaDefines.h"

#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPtrField.h"

#include <QString>
#include <QStringList>

class RigEclipseCaseData;
class RimWellPath;

//==================================================================================================
/// @brief UI model for configuring create ensemble well log.
//==================================================================================================
class RicCreateEnsembleWellLogUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    enum class WellPathSource
    {
        FILE,
        PROJECT_WELLS
    };

    /// Constructs the command object.
    RicCreateEnsembleWellLogUi();
    /// Destroys the command object.
    ~RicCreateEnsembleWellLogUi() override;
    /// @return The tab names.
    const QStringList& tabNames() const;

    /// @return The auto create ensemble well logs.
    bool autoCreateEnsembleWellLogs() const;

    /// @return The time step.
    int     timeStep() const;
    /// @return The well path file path.
    QString wellPathFilePath() const;

    /// @return The well path source.
    WellPathSource wellPathSource() const;
    /// Sets well path source.
    void           setWellPathSource( WellPathSource wellPathSource );

    /// Sets well path from project.
    void         setWellPathFromProject( RimWellPath* wellPath );
    /// @return The well path from project.
    RimWellPath* wellPathFromProject() const;

    /// @return The properties.
    std::vector<std::pair<QString, RiaDefines::ResultCatType>> properties() const;

    /// @return The properties.
    static std::vector<std::pair<QString, RiaDefines::ResultCatType>> properties( const std::vector<QString>& resultNames,
                                                                                  const std::vector<RiaDefines::ResultCatType>& resultCategories,
                                                                                  const RigEclipseCaseData* caseData );

    /// Sets case data.
    void setCaseData( RigEclipseCaseData* caseData );

protected:
    /// Defines the field ordering used by the command UI.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;

    /// @return The selectable values for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;
    /// Performs the define editor attribute command operation.
    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;

    /// @return The valid result categories.
    std::vector<RiaDefines::ResultCatType> validResultCategories() const;

private:
    caf::PdmField<caf::FilePath>                m_wellFilePath;
    caf::PdmField<caf::AppEnum<WellPathSource>> m_wellPathSource;
    caf::PdmPtrField<RimWellPath*>              m_wellPath;
    caf::PdmField<bool>                         m_autoCreateEnsembleWellLogs;

    caf::PdmField<std::vector<QString>> m_selectedKeywords;
    caf::PdmField<int>                  m_timeStep;

    QStringList         m_tabNames;
    RigEclipseCaseData* m_caseData;
};
