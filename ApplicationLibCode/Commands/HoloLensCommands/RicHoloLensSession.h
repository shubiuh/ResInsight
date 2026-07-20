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
/// @brief Declares holo lens session command support.

#pragma once

#include "RicHoloLensRestClient.h"
#include "RicHoloLensSessionObserver.h"

#include "VdeCachingHashedIdFactory.h"
#include "VdePacketDirectory.h"

#include <QPointer>
#include <QString>

#include <vector>

class RimGridView;

//==================================================================================================
//
//
//
//==================================================================================================
/// @brief Supports holo lens session command workflows.
class RicHoloLensSession : public QObject, private RicHoloLensRestResponseHandler
{
public:
    /// Destroys the command object.
    ~RicHoloLensSession() override;

    /// Creates session.
    static RicHoloLensSession* createSession( const QString&              serverUrl,
                                              const QString&              sessionName,
                                              const QByteArray&           sessionPinCode,
                                              RicHoloLensSessionObserver* sessionObserver );
    /// Creates dummy file backed session.
    static RicHoloLensSession* createDummyFileBackedSession();
    /// Performs the destroy session command operation.
    void                       destroySession();

    /// @return Whether session valid.
    bool isSessionValid() const;

    /// Updates session data from view.
    void updateSessionDataFromView( const RimGridView& activeView );

private:
    /// Constructs the command object.
    RicHoloLensSession();

    /// Handles successful create session.
    void handleSuccessfulCreateSession() override;
    /// Handles failed create session.
    void handleFailedCreateSession() override;
    /// Handles successful send meta data.
    void handleSuccessfulSendMetaData( int metaDataSequenceNumber, const QByteArray& jsonServerResponseString ) override;
    /// Handles error.
    void handleError( const QString& errMsg, const QString& url, const QString& serverData ) override;

    /// @return The parse JSON integer array.
    static bool parseJsonIntegerArray( const QByteArray& jsonString, std::vector<int>* integerArr );

    /// Performs the notify observer command operation.
    void notifyObserver( RicHoloLensSessionObserver::Notification notification );

private:
    bool                            m_isSessionValid;
    QPointer<RicHoloLensRestClient> m_restClient;

    int                       m_lastExtractionMetaDataSequenceNumber;
    std::vector<int>          m_lastExtractionAllReferencedPacketIdsArr;
    VdeCachingHashedIdFactory m_cachingIdFactory;
    VdePacketDirectory        m_packetDirectory;

    RicHoloLensSessionObserver* m_sessionObserver;

    QString m_dbgFileExportDestinationFolder;
};
