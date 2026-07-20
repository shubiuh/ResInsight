/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2019-     Equinor ASA
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
/// @brief Declares export contour map to text command support.

#pragma once

#include "cafPdmField.h"
#include "cafPdmObject.h"

//==================================================================================================
/// @brief UI model for configuring export contour map to text.
//==================================================================================================
class RicExportContourMapToTextUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Constructs the command object.
    RicExportContourMapToTextUi();

    /// Exports file name.
    QString exportFileName() const;
    /// Sets export file name.
    void    setExportFileName( const QString& exportFileName );
    /// Exports local coordinates.
    bool    exportLocalCoordinates() const;
    /// @return The undefined value label.
    QString undefinedValueLabel() const;
    /// @return The exclude undefined values.
    bool    excludeUndefinedValues() const;

protected:
    /// Performs the define editor attribute command operation.
    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;

private:
    caf::PdmField<QString> m_exportFileName;
    caf::PdmField<bool>    m_exportLocalCoordinates;
    caf::PdmField<QString> m_undefinedValueLabel;
    caf::PdmField<bool>    m_excludeUndefinedValues;
};
