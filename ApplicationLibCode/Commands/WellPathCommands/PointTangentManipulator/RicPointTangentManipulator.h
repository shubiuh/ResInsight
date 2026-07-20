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
/// @brief Declares point tangent manipulator command support.

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
class Viewer;
};

class QMouseEvent;

class RicPointTangentManipulatorPartMgr;

//==================================================================================================
//
//
//==================================================================================================
/// @brief Supports point tangent manipulator command workflows.
class RicPointTangentManipulator : public QObject
{
    Q_OBJECT

public:
    /// Constructs the command object.
    explicit RicPointTangentManipulator( caf::Viewer* viewer );
    /// Destroys the command object.
    ~RicPointTangentManipulator() override;

    /// Sets origin.
    void setOrigin( const cvf::Vec3d& origin );
    /// Sets tangent.
    void setTangent( const cvf::Vec3d& tangent );
    /// Sets handle size.
    void setHandleSize( double handleSize );
    /// Sets polyline.
    void setPolyline( const std::vector<cvf::Vec3d>& polyline );

    /// Appends parts to model.
    void appendPartsToModel( cvf::ModelBasicList* model );

signals:
    /// Performs the notify selected command operation.
    void notifySelected();
    /// Performs the notify drag finished command operation.
    void notifyDragFinished();
    /// Performs the notify update command operation.
    void notifyUpdate( const cvf::Vec3d& origin, const cvf::Vec3d& tangent );

protected:
    /// @return The event filter.
    bool eventFilter( QObject* obj, QEvent* event ) override;

private:
    QPointer<caf::Viewer> m_viewer;
    bool                  m_isDraggingInComparisonView = false;

    cvf::ref<RicPointTangentManipulatorPartMgr> m_partManager;
};
