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
/// @brief Declares text annotation3d command support.

#pragma once

#include "WellPathCommands/PointTangentManipulator/Ric3dObjectEditorHandle.h"

class RicPointTangentManipulator;

#include "cvfObject.h"
#include "cvfVector3.h"

/// @brief Utilities for cvf command workflows.
namespace cvf
{
class ModelBasicList;
}

class QString;
#include <QPointer>

/// @brief Interactive editor for text annotation3d.
class RicTextAnnotation3dEditor : public Ric3dObjectEditorHandle
{
    CAF_PDM_UI_3D_OBJECT_EDITOR_HEADER_INIT;
    Q_OBJECT
public:
    /// Constructs the command object.
    RicTextAnnotation3dEditor();
    /// Destroys the command object.
    ~RicTextAnnotation3dEditor() override;

protected:
    /// Configures and update.
    void configureAndUpdateUi( const QString& uiConfigName ) override;
    /// Performs the cleanup before setting PDM object command operation.
    void cleanupBeforeSettingPdmObject() override;

private slots:
    /// Performs the slot label updated command operation.
    void slotLabelUpdated( const cvf::Vec3d& origin, const cvf::Vec3d& dummy );
    /// Performs the slot anchor updated command operation.
    void slotAnchorUpdated( const cvf::Vec3d& origin, const cvf::Vec3d& dummy );

private:
    /// Updates point.
    void updatePoint( caf::PdmUiFieldHandle* uiField, const cvf::Vec3d& newPos );

    QPointer<RicPointTangentManipulator> m_labelManipulator;
    QPointer<RicPointTangentManipulator> m_anchorManipulator;
    cvf::ref<cvf::ModelBasicList>        m_cvfModel;
};
