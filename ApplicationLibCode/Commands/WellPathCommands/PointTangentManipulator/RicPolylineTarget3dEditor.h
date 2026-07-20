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
/// @brief Declares polyline target3d command support.

#pragma once

#include "Ric3dObjectEditorHandle.h"

class RicPointTangentManipulator;

#include "cvfObject.h"
#include "cvfVector3.h"

#include <QPointer>

/// @brief Utilities for cvf command workflows.
namespace cvf
{
class ModelBasicList;
}

class QString;

/// @brief Interactive editor for polyline target3d.
class RicPolylineTarget3dEditor : public Ric3dObjectEditorHandle
{
    CAF_PDM_UI_3D_OBJECT_EDITOR_HEADER_INIT;
    Q_OBJECT
public:
    /// Constructs the command object.
    RicPolylineTarget3dEditor();
    /// Destroys the command object.
    ~RicPolylineTarget3dEditor() override;

protected:
    /// Configures and update.
    void configureAndUpdateUi( const QString& uiConfigName ) override;
    /// Performs the cleanup before setting PDM object command operation.
    void cleanupBeforeSettingPdmObject() override;

private slots:
    /// Performs the slot updated command operation.
    void slotUpdated( const cvf::Vec3d& origin, const cvf::Vec3d& tangent );
    /// Performs the slot selected in3 D command operation.
    void slotSelectedIn3D();
    /// Performs the slot drag finished command operation.
    void slotDragFinished();

private:
    QPointer<RicPointTangentManipulator> m_manipulator;
    cvf::ref<cvf::ModelBasicList>        m_cvfModel;
};
