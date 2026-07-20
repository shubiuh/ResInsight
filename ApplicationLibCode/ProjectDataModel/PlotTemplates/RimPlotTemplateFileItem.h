/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Project-tree representation of one discovered summary plot-template file.
//
//  Copyright (C) 2011-2012 Statoil ASA, Ceetron AS
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

#include "cafFilePath.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"

//==================================================================================================
/// @brief Presents a plot-template path with type and default-selection icon state.
///
/// `.rpt` files represent ordinary templates and `.erpt` files represent ensemble templates. The
/// item does not own or parse the file; it stores its absolute location and reflects whether that
/// path is selected as a default in summary preferences.
//==================================================================================================
class RimPlotTemplateFileItem : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes an empty standard-template item.
    RimPlotTemplateFileItem();
    ~RimPlotTemplateFileItem() override;

    /// Stores @p filePath and derives the UI name and template-type icon from it.
    void    setFilePath( const QString& filePath );
    /// @return Stored absolute template path.
    QString absoluteFilePath() const;

    /// @return Whether the stored filename has the `.erpt` ensemble-template suffix.
    bool isEnsembleTemplate() const;

    /// @return Whether preferences currently select this path as a default template.
    bool isDefaultTemplate() const;

    /// Adds or removes the default-selection overlay on the current type icon.
    void updateIconState();

private:
    /// Absolute path of the discovered template file.
    caf::PdmField<caf::FilePath> m_absoluteFileName;
};
