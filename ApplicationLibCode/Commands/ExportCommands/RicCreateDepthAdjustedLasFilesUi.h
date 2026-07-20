/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2023- Equinor ASA
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
/// @brief Declares create depth adjusted LAS files command support.

#pragma once

#include "RiaLasDefines.h"

#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPtrArrayField.h"
#include "cafPdmPtrField.h"
#include "cafPdmUiItem.h"

#include <QList>
#include <QString>
#include <QStringList>

class RimCase;
class RimWellPath;
class RimWellLogLasFile;

//==================================================================================================
/// @brief UI model for configuring create depth adjusted LAS files.
//==================================================================================================
class RicCreateDepthAdjustedLasFilesUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Constructs the command object.
    RicCreateDepthAdjustedLasFilesUi();
    /// Destroys the command object.
    ~RicCreateDepthAdjustedLasFilesUi() override;

    /// @return The selectable values for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;
    /// Responds to a value changed through the command UI.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;
    /// Performs the define editor attribute command operation.
    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;

    /// Sets default values.
    void    setDefaultValues();
    /// @return Whether valid selections.
    bool    hasValidSelections() const;
    /// @return The invalid selections log string.
    QString invalidSelectionsLogString() const;

public:
    caf::PdmField<QString>               exportFolder;
    caf::PdmPtrField<RimCase*>           selectedCase;
    caf::PdmPtrField<RimWellPath*>       sourceWell;
    caf::PdmPtrField<RimWellLogLasFile*> wellLogFile;
    caf::PdmField<std::vector<QString>>  selectedResultProperties;
    caf::PdmPtrArrayField<RimWellPath*>  destinationWells;

protected:
    /// Defines the field ordering used by the command UI.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;

private:
    const QStringList m_depthProperties = QStringList(
        { RiaDefines::propertyNameMeasuredDepth(), RiaDefines::propertyNameTvdMslDepth(), RiaDefines::propertyNameTvdRkbDepth() } );
};
