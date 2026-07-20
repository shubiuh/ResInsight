////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2022     Equinor ASA
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
/// @brief Declares save multi plot template feature settings command support.

#pragma once

#include "cafFilePath.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"

//==================================================================================================
/// @brief Supports save multi plot template feature settings command workflows.
//==================================================================================================
class RicSaveMultiPlotTemplateFeatureSettings : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Constructs the command object.
    RicSaveMultiPlotTemplateFeatureSettings();

    /// Sets file path.
    void    setFilePath( const QString& filePath );
    /// @return The file path.
    QString filePath() const;

    /// Sets name.
    void    setName( const QString& name );
    /// @return The name.
    QString name() const;

    /// @return The use placholder for wells.
    bool usePlacholderForWells() const;
    /// @return The use placholder for groups.
    bool usePlacholderForGroups() const;
    /// @return The use placholder for regions.
    bool usePlacholderForRegions() const;

private:
    /// Defines the field ordering used by the command UI.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;
    /// Performs the define editor attribute command operation.
    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;

private:
    caf::PdmField<caf::FilePath> m_filePath;
    caf::PdmField<QString>       m_name;

    caf::PdmField<bool> m_persistObjectNameForWells;
    caf::PdmField<bool> m_persistObjectNameGroups;
    caf::PdmField<bool> m_persistObjectNameRegions;
};
