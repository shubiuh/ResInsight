/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Application event filter that cancels interactive measurement with Escape.
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

#pragma once

#include "cafPdmPointer.h"

#include <QObject>

class QEvent;
class RimMeasurement;

/// @brief Converts an Escape key press into disabling the associated RimMeasurement.
///
/// The parent relationship is guarded by caf::PdmPointer because the filter is registered on the
/// application rather than parented as a QObject child. Non-Escape events continue through Qt's
/// normal event-filter chain.
class RiuMeasurementEventFilter : public QObject
{
    Q_OBJECT
public:
    /// Creates an unregistered filter associated with @p parent.
    explicit RiuMeasurementEventFilter( RimMeasurement* parent );

    /// Installs this filter on the GUI application.
    void registerFilter();
    /// Removes this filter from the GUI application.
    void unregisterFilter();

protected:
    /// Consumes Escape presses and disables measurement; forwards all other events.
    bool eventFilter( QObject* obj, QEvent* event ) override;

private:
    /// Guarded, non-owning measurement state to disable.
    caf::PdmPointer<RimMeasurement> m_parent;
};
