/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017 Statoil ASA
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
/// @brief Declares export LGR command support.

#pragma once

#include "RigCompletionData.h"

#include "RicLgrSplitType.h"

#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPtrField.h"

#include "cvfVector3.h"

#include <QStringList>

#include <set>

class RimEclipseCase;
class RicCellRangeUi;

//==================================================================================================
/// @brief UI model for configuring export LGR.
//==================================================================================================
class RicExportLgrUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Constructs the command object.
    RicExportLgrUi();

    /// Sets case.
    void setCase( RimEclipseCase* rimCase );
    /// Sets time step.
    void setTimeStep( int timeStep );

    /// @return The refinement.
    cvf::Vec3st                                 refinement() const;
    /// Exports folder.
    QString                                     exportFolder() const;
    /// @return The case to apply.
    RimEclipseCase*                             caseToApply() const;
    /// @return The time step.
    int                                         timeStep() const;
    /// @return The completion types.
    std::set<RigCompletionData::CompletionType> completionTypes() const;
    /// @return The split type.
    Lgr::SplitType                              splitType() const;

    /// Performs the hide export folder field command operation.
    void hideExportFolderField( bool hide );
    /// Sets export folder.
    void setExportFolder( const QString& folder );

private:
    /// Sets default values from case.
    void setDefaultValuesFromCase();

    /// @return The selectable values for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;
    /// Responds to a value changed through the command UI.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;
    /// Defines the field ordering used by the command UI.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;
    /// Performs the define editor attribute command operation.
    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;

private:
    caf::PdmField<QString>            m_exportFolder;
    caf::PdmPtrField<RimEclipseCase*> m_caseToApply;
    caf::PdmField<int>                m_timeStep;
    caf::PdmField<bool>               m_includePerforations;
    caf::PdmField<bool>               m_includeFractures;
    caf::PdmField<bool>               m_includeFishbones;

    caf::PdmField<int> m_refinementI;
    caf::PdmField<int> m_refinementJ;
    caf::PdmField<int> m_refinementK;

    caf::PdmField<Lgr::SplitTypeEnum> m_splitType;
};
