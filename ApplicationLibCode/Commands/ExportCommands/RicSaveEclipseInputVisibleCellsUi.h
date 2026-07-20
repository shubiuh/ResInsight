/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017-     Statoil ASA
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
/// @brief Declares save Eclipse input visible cells command support.

#pragma once

#include "cafAppEnum.h"
#include "cafPdmChildArrayField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"

//==================================================================================================
/// @brief UI model for configuring save Eclipse input visible cells.
//==================================================================================================
class RicSaveEclipseInputVisibleCellsUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    // Values are exposed in gRPC .proto. Do not change without also changing .proto
    enum ExportKeyword
    {
        FLUXNUM,
        MULTNUM,
        ACTNUM,
    };

    using ExportKeywordEnum = caf::AppEnum<ExportKeyword>;

public:
    /// Constructs the command object.
    RicSaveEclipseInputVisibleCellsUi();
    /// Destroys the command object.
    ~RicSaveEclipseInputVisibleCellsUi() override;

    caf::PdmField<QString>           exportFilename;
    caf::PdmField<ExportKeywordEnum> exportKeyword;
    caf::PdmField<int>               visibleActiveCellsValue;
    caf::PdmField<int>               hiddenActiveCellsValue;
    caf::PdmField<int>               inactiveCellsValue;
    caf::PdmField<bool>              writeEchoInGrdeclFiles;

protected:
    /// Performs the define editor attribute command operation.
    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;
    /// Defines the field ordering used by the command UI.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;
    /// Responds to a value changed through the command UI.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;

private:
    /// @return The default export path.
    QString getDefaultExportPath() const;

private:
    bool exportFilenameManuallyChanged;
};
