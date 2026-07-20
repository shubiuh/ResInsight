/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2018-     Equinor ASA
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
/// @brief Declares polyline3d command support.

#pragma once

#include "cafPdmUi3dObjectEditorHandle.h"
#include "cafPdmUiFieldEditorHandle.h"

#include <memory>

/// @brief Utilities for caf command workflows.
namespace caf
{
class PickEventHandler;
};

class RicPolylineTarget3dEditor;

//==================================================================================================
/// @brief Supports polyline3d editor attribute command workflows.
//==================================================================================================
class RicPolyline3dEditorAttribute : public caf::PdmUiEditorAttribute
{
public:
    RicPolyline3dEditorAttribute()
        : enablePicking( false )
    {
    }

public:
    bool                                   enablePicking;
    std::shared_ptr<caf::PickEventHandler> pickEventHandler;
};

//==================================================================================================
/// @brief Interactive editor for polyline3d.
//==================================================================================================
class RicPolyline3dEditor : public caf::PdmUi3dObjectEditorHandle
{
    CAF_PDM_UI_3D_OBJECT_EDITOR_HEADER_INIT;
    Q_OBJECT
public:
    /// Constructs the command object.
    RicPolyline3dEditor();
    /// Destroys the command object.
    ~RicPolyline3dEditor() override;

protected:
    /// Configures and update.
    void configureAndUpdateUi( const QString& uiConfigName ) override;

private:
    std::vector<RicPolylineTarget3dEditor*> m_targetEditors;
    RicPolyline3dEditorAttribute            m_attribute;
};
