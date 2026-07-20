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
/// @brief Declares holo lens session observer command support.

#pragma once

class RicHoloLensSession;

//==================================================================================================
//
//
//
//==================================================================================================
/// @brief Supports holo lens session observer command workflows.
class RicHoloLensSessionObserver
{
public:
    enum Notification
    {
        CreateSessionSucceeded,
        CreateSessionFailed,
        GeneralError
    };

public:
    virtual ~RicHoloLensSessionObserver() {}
    /// Handles session notification.
    virtual void handleSessionNotification( const RicHoloLensSession* session, Notification notification ) = 0;
};
