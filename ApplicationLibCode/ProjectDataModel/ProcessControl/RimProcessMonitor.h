/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2021    Equinor ASA
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
/// @brief Qt signal receiver that captures and logs external-process output.
#pragma once

#include <QObject>
#include <QProcess>

#include <QString>
#include <QStringList>

/// @brief Accumulates line-oriented output and forwards process completion to RimProcessQueue.
///
/// The monitor is connected to a single active QProcess at a time. Empty lines are discarded and
/// retained output is independent of optional logging. Completion always informs the global queue,
/// allowing another waiting process to start.
class RimProcessMonitor : public QObject
{
    Q_OBJECT

public:
    /// Associates monitor messages with @p processId and optionally enables logging.
    explicit RimProcessMonitor( size_t processId, bool logStdOutErr = true );

    /// Discards all output retained from the previous execution.
    void        clearStdOutErr();
    /// @return Captured non-empty standard-output lines.
    QStringList stdOut() const;
    /// @return Captured non-empty standard-error lines.
    QStringList stdErr() const;

    /// Reassigns the identifier used in prefixes and queue callbacks.
    void setProcessId( size_t processId );

signals:

public slots:
    /// Logs a human-readable description of a QProcess runtime error.
    virtual void error( QProcess::ProcessError error );
    /// Logs completion and releases the process's slot in RimProcessQueue.
    virtual void finished( int exitCode, QProcess::ExitStatus exitStatus );
    /// Drains complete standard-error lines from the sending QProcess.
    virtual void readyReadStandardError();
    /// Drains complete standard-output lines from the sending QProcess.
    virtual void readyReadStandardOutput();
    /// Logs successful startup when logging is enabled.
    virtual void started();

protected:
    /// Prefixes @p message with the stable process identifier.
    QString     addPrefix( QString message );
    /// Identifier shared with the corresponding RimProcess.
    size_t      m_processId;
    /// Whether captured lifecycle and output messages are forwarded to RiaLogging.
    bool        m_logStdOutErr;
    /// Captured standard-output lines in arrival order.
    QStringList m_stdOut;
    /// Captured standard-error lines in arrival order.
    QStringList m_stdErr;
};
