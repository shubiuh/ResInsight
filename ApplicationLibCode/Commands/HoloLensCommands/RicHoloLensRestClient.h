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
/// @brief Declares holo lens rest client command support.

#pragma once

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QObject>

//==================================================================================================
//
//
//
//==================================================================================================
/// @brief Supports holo lens rest response handler command workflows.
class RicHoloLensRestResponseHandler
{
public:
    /// Handles successful create session.
    virtual void handleSuccessfulCreateSession() = 0;
    /// Handles failed create session.
    virtual void handleFailedCreateSession()     = 0;

    /// Handles successful send meta data.
    virtual void handleSuccessfulSendMetaData( int metaDataSequenceNumber, const QByteArray& jsonServerResponseString ) = 0;

    /// Handles error.
    virtual void handleError( const QString& errMsg, const QString& url, const QString& serverData ) = 0;
};

//==================================================================================================
//
//
//
//==================================================================================================
/// @brief Supports holo lens rest client command workflows.
class RicHoloLensRestClient : public QObject
{
    Q_OBJECT

public:
    /// Constructs the command object.
    RicHoloLensRestClient( QString serverUrl, QString sessionName, RicHoloLensRestResponseHandler* responseHandler );

    /// Removes or clears response handler.
    void clearResponseHandler();

    /// Performs the dbg disable certificate verification command operation.
    void dbgDisableCertificateVerification();

    /// Creates session.
    void createSession( const QByteArray& sessionPinCode );
    /// Removes or clears session.
    void deleteSession();
    /// Performs the send meta data command operation.
    void sendMetaData( int metaDataSequenceNumber, const QString& jsonMetaDataString );
    /// Performs the send binary data command operation.
    void sendBinaryData( const QByteArray& binaryDataArr, QByteArray dbgTagString );

private:
    /// Adds bearer authentication header to request.
    void           addBearerAuthenticationHeaderToRequest( QNetworkRequest* request ) const;
    /// @return The detect and handle error reply.
    bool           detectAndHandleErrorReply( QString operationName, QNetworkReply* reply );
    /// @return The network error code as string.
    static QString networkErrorCodeAsString( QNetworkReply::NetworkError nwErr );
    /// @return The current time stamp_ms.
    static qint64  getCurrentTimeStamp_ms();

private slots:
    /// Performs the slot create session finished command operation.
    void slotCreateSessionFinished();
    /// Performs the slot delete session finished command operation.
    void slotDeleteSessionFinished();
    /// Performs the slot send meta data finished command operation.
    void slotSendMetaDataFinished();
    /// Performs the slot send binary data finished command operation.
    void slotSendBinaryDataFinished();
    /// Performs the slot dbg upload progress command operation.
    void slotDbgUploadProgress( qint64 bytesSent, qint64 bytesTotal );

    /// Performs the slot ssl errors command operation.
    void slotSslErrors( const QList<QSslError>& errors );

private:
    QNetworkAccessManager           m_accessManager;
    QString                         m_serverUrl;
    QString                         m_sessionName;
    RicHoloLensRestResponseHandler* m_responseHandler;

    bool m_dbgDisableCertificateVerification; // Debug option to disable certificate verification. Needed in order to
                                              // work with self-signed certifiactes

    QByteArray m_bearerToken;
};
