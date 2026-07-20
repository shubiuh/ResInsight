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
/// @brief Configurable external process with synchronous and asynchronous execution support.
#pragma once

#include "cafFilePath.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"

#include <QPointer>
#include <QString>
#include <QStringList>

#include <utility>
#include <vector>

class RimProcessMonitor;
class QProcess;

/// @brief Owns the configuration, execution object, and monitor for one external command.
///
/// Commands ending in platform script extensions are automatically routed through the appropriate
/// interpreter. Each instance receives a process-wide identifier used by RimProcessQueue and log
/// messages. QProcess is created per run and released with deleteLater(), while the monitor is owned
/// for the full RimProcess lifetime. RimProcess itself remains owned by its caller.
class RimProcess : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Creates a process and takes ownership of @p monitor, or creates a default monitor when null.
    RimProcess( bool logStdOutErr = true, RimProcessMonitor* monitor = nullptr );
    /// Releases the owned monitor; callers must stop active execution before destroying this object.
    ~RimProcess() override;

    /// Sets the user-facing description shown by PDM editors.
    void setDescription( QString desc );
    /// Sets the executable or script path and inserts an interpreter when required.
    void setCommand( QString cmdStr );
    /// Appends one trimmed argument without performing shell tokenization.
    void addParameter( QString paramStr );
    /// Appends each argument in @p parameterList in order.
    void addParameters( QStringList parameterList );

    /// Adds or replaces an environment value in the child process environment.
    void addEnvironmentVariable( QString name, QString value );

    /// Sets the directory from which the child process will execute.
    void setWorkingDirectory( QString directory );

    /// @return Human-readable command line with arguments containing spaces quoted.
    QString commandLine() const;

    /// @return Executable passed to QProcess, possibly an inferred script interpreter.
    QString     command() const;
    /// @return Ordered arguments passed directly to QProcess.
    QStringList parameters() const;
    /// @return Unique identifier assigned when this instance was constructed.
    size_t      ID() const;

    /// Runs synchronously while pumping non-input Qt events; returns true for exit code zero.
    bool execute( bool enableStdOut = true, bool enableStdErr = true );

    /// Starts asynchronous execution and connects enabled output channels to the monitor.
    bool start( bool enableStdOut = true, bool enableStdErr = true );
    /// Schedules the completed QProcess for Qt deletion and resets the run state.
    void cleanUpAfterRun();
    /// Forcefully kills a running child process.
    void terminate();
    /// Reports a synthetic crash completion when a queued process cannot be started.
    void notifyErrorFinish();

    /// @return Non-empty standard-error lines captured by the monitor.
    QStringList stdErr() const;
    /// @return Non-empty standard-output lines captured by the monitor.
    QStringList stdOut() const;

protected:
    /// Uses the process description as its label in generic PDM views.
    caf::PdmFieldHandle* userDescriptionField() override;

private:
    /// @return Interpreter required for the command's script suffix, if any.
    QString optionalCommandInterpreter() const;
    /// @return Interpreter flags inserted before the script path, if any.
    QString optionalPreParameters() const;
    /// Quotes @p argument for display when it contains spaces.
    QString handleSpaces( QString argument ) const;

    /// @return Whether the configured command is a Windows .cmd or .bat script.
    bool isWindowsBatchFile() const;

    /// Executable ultimately passed to QProcess.
    caf::PdmField<QString>       m_command;
    /// Ordered command arguments, including inferred interpreter arguments.
    QStringList                  m_arguments;
    /// User-facing process description.
    caf::PdmField<QString>       m_description;
    /// Unique process identifier used for queue operations and logging.
    caf::PdmField<size_t>        m_id;
    /// Optional child-process working directory.
    caf::PdmField<caf::FilePath> m_workDir;

    /// Environment overrides layered on top of the system environment.
    std::vector<std::pair<QString, QString>> m_environmentVariables;

    /// Monotonically increasing identifier source; zero remains the invalid queue identifier.
    static size_t      m_nextProcessId;
    /// Owned receiver of QProcess status and output signals.
    RimProcessMonitor* m_monitor;
    /// Controls command, output, and lifecycle logging.
    bool               m_enableLogging;
    /// Guarded pointer to the QProcess allocated for the current run.
    QPointer<QProcess> m_qProcess;
};
