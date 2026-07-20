/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017     Statoil ASA
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
/// @brief Declares select plot template command support.

#pragma once

#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPtrArrayField.h"

#include <QString>
#include <vector>

class RimPlotTemplateFileItem;
class RimPlotTemplateFolderItem;

//==================================================================================================
/// @brief UI model for configuring select plot template.
//==================================================================================================
class RicSelectPlotTemplateUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Constructs the command object.
    RicSelectPlotTemplateUi();

    /// Sets multi select mode.
    void setMultiSelectMode( bool multiSelect );
    /// Sets initial selection.
    void setInitialSelection( const std::vector<QString>& selectedTemplates );

    /// @return The selected plot templates.
    std::vector<RimPlotTemplateFileItem*> selectedPlotTemplates();

private:
    /// @return The selectable values for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;

    /// Performs the define editor attribute command operation.
    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;

private:
    caf::PdmPtrArrayField<RimPlotTemplateFileItem*> m_selectedPlotTemplates;
    bool                                            m_useMultiSelect;
};
