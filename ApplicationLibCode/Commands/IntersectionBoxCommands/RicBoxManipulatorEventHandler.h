/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2013-     Statoil ASA
//  Copyright (C) 2013-     Ceetron Solutions AS
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
/// @brief Declares box manipulator command support.

#pragma once

#include "cvfObject.h"
#include "cvfVector3.h"

#include <QObject>
#include <QPointer>

/// @brief Utilities for cvf command workflows.
namespace cvf
{
class Model;
class ModelBasicList;
}; // namespace cvf

/// @brief Utilities for caf command workflows.
namespace caf
{
class BoxManipulatorPartManager;
class Viewer;
}; // namespace caf

class QMouseEvent;

//==================================================================================================
//
//
//==================================================================================================
/// @brief Handles box manipulator events.
class RicBoxManipulatorEventHandler : public QObject
{
    Q_OBJECT

public:
    /// Constructs the command object.
    explicit RicBoxManipulatorEventHandler( caf::Viewer* viewer );
    /// Destroys the command object.
    ~RicBoxManipulatorEventHandler() override;

    /// Registers in additional viewer.
    void registerInAdditionalViewer( caf::Viewer* viewer );

    /// Sets origin.
    void setOrigin( const cvf::Vec3d& origin );
    /// Sets size.
    void setSize( const cvf::Vec3d& size );

    /// Appends parts to model.
    void appendPartsToModel( cvf::ModelBasicList* model );

signals:
    /// Performs the notify redraw command operation.
    void notifyRedraw();
    /// Performs the notify update command operation.
    void notifyUpdate( const cvf::Vec3d& origin, const cvf::Vec3d& size );

protected:
    /// @return The event filter.
    bool eventFilter( QObject* obj, QEvent* event ) override;

private:
    QPointer<caf::Viewer>              m_viewer;
    std::vector<QPointer<caf::Viewer>> m_otherViewers;

    cvf::ref<caf::BoxManipulatorPartManager> m_partManager;
};
