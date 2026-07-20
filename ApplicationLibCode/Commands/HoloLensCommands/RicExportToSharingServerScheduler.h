/////////////////////////////////////////////////////////////////////////////////
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
/// @file
/// @brief Declares export to sharing server scheduler command support.

#pragma once

#include <QObject>

class QTimer;

//--------------------------------------------------------------------------------------------------
/// @brief Supports export to sharing server scheduler command workflows.
//--------------------------------------------------------------------------------------------------
class RicExportToSharingServerScheduler : public QObject
{
    Q_OBJECT;

public:
    /// @return The instance.
    static RicExportToSharingServerScheduler* instance();
    /// Performs the schedule update session command operation.
    void                                      scheduleUpdateSession();

private slots:
    /// Performs the slot trigger update session when ready command operation.
    void slotTriggerUpdateSessionWhenReady();

private:
    RicExportToSharingServerScheduler()
        : m_timer( nullptr )
    {
    }

    /// Destroys the command object.
    ~RicExportToSharingServerScheduler() override;

    /// Constructs the command object.
    RicExportToSharingServerScheduler( const RicExportToSharingServerScheduler& o ) = delete;
    /// Provides the corresponding operator for command data.
    void operator=( const RicExportToSharingServerScheduler& o )                    = delete;

    /// Performs the start timer command operation.
    void startTimer( int msecs );
    /// Performs the trigger update session command operation.
    void triggerUpdateSession();

private:
    QTimer* m_timer;
};
