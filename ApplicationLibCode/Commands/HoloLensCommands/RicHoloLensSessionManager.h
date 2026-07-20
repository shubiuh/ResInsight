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
/// @brief Declares holo lens session manager command support.

#pragma once

#include "RicHoloLensSessionObserver.h"

#include <QPointer>

class RicHoloLensSession;

//==================================================================================================
//
//
//
//==================================================================================================
/// @brief Supports holo lens session manager command workflows.
class RicHoloLensSessionManager : private RicHoloLensSessionObserver
{
public:
    /// @return The instance.
    static RicHoloLensSessionManager* instance();

    /// Creates session.
    bool createSession( const QString& serverUrl, const QString& sessionName, const QString& sessionPinCode );
    /// Creates dummy file backed session.
    bool createDummyFileBackedSession();
    /// Performs the terminate session command operation.
    void terminateSession();

    /// @return The session.
    RicHoloLensSession* session();

    /// Performs the refresh toolbar state command operation.
    static void refreshToolbarState();

private:
    /// Handles session notification.
    void handleSessionNotification( const RicHoloLensSession* session, Notification notification ) override;

private:
    /// Constructs the command object.
    RicHoloLensSessionManager();

private:
    QPointer<RicHoloLensSession> m_session;
};
