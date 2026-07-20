/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Interactive 3D distance and polyline measurement state.
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

#pragma once

#include "cafPdmObject.h"

#include "cvfVector3.h"

#include <QPointer>

class Rim3dView;
class RiuMeasurementEventFilter;

//==================================================================================================
/// @brief Owns points collected by the global measurement pick handler for one active 3D view.
///
/// Enabling measurement installs both a pick handler and an Escape-key event filter. Points are
/// expressed in domain coordinates and are cleared when picking moves to another view, preventing
/// distances from combining unrelated coordinate systems. Display geometry is transient and rebuilt
/// by the active view whenever measurement state changes.
//==================================================================================================
class RimMeasurement : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

    using Vec3d = cvf::Vec3d;

public:
    /// Interaction mode used by the measurement pick handler.
    enum MeasurementMode
    {
        MEASURE_DISABLED = 0,
        MEASURE_REGULAR,
        MEASURE_POLYLINE
    };

    /// Creates disabled measurement state with no points.
    RimMeasurement();
    /// Releases the measurement model; mode should normally be disabled before destruction.
    ~RimMeasurement() override;

    /// Activates or disables picking and its Escape-key filter.
    void            setMeasurementMode( MeasurementMode measureMode );
    /// @return Current measurement interaction mode.
    MeasurementMode measurementMode() const;

    /// Appends @p pointInDomainCoord, resetting first if the active source view changed.
    void               addPointInDomainCoords( const Vec3d& pointInDomainCoord );
    /// @return Measurement points in insertion order and source-view domain coordinates.
    std::vector<Vec3d> pointsInDomainCoords() const;

    /// Clears all points and refreshes the measurement display.
    void removeAllPoints();

    /// @return Formatted segment and cumulative geometry information for the current points.
    QString label() const;

private:
    /// Rebuilds measurement geometry in the active reservoir view.
    void updateView() const;

private:
    /// Current picking behavior.
    MeasurementMode            m_measurementMode;
    /// Picked points expressed in the source view's domain coordinate system.
    std::vector<Vec3d>         m_pointsInDomainCoords;
    /// Guarded, non-owning view from which the current points were collected.
    caf::PdmPointer<Rim3dView> m_sourceView;

    /// Qt-owned-later Escape-key filter installed while measurement is enabled.
    QPointer<RiuMeasurementEventFilter> m_eventFilter;
};
