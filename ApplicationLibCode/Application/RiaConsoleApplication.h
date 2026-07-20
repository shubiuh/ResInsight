////////////////////////////////////////////////////////////////////////////////
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
/// @brief Declares the headless (console) ResInsight application class.
#pragma once

#include "RiaApplication.h"

#include <QCoreApplication>

#include <QPointer>
#include <QTimer>

namespace cvf
{
class ProgramOptions;
}

//==================================================================================================
/// @brief Headless ResInsight application used for automation, batch processing, and scripting.
///
/// The class combines Qt's non-GUI event loop with the shared services in RiaApplication. It parses
/// console-specific command-line workflows, including project/case replacement, command files,
/// code generation, and custom preference files. Project callbacks keep plot data and queued Qt
/// work current even though no windows are created.
///
/// On Windows, initialization attaches to the parent console when possible and preserves redirected
/// streams. Messages are routed to standard output and errors to the ResInsight logging system.
//==================================================================================================
class RiaConsoleApplication : public QCoreApplication, public RiaApplication
{
    Q_OBJECT

public:
    /// @return The active application cast to console mode; asserts if called in GUI mode.
    static RiaConsoleApplication* instance();

    /// Constructs Qt and shared ResInsight application state from the original arguments.
    RiaConsoleApplication( int& argc, char** argv );

    /// Shuts down console-specific services before shared application destruction.
    ~RiaConsoleApplication() override;

    // Public RiaApplication overrides
    /// Initializes shared state, console logging, and the legacy socket server.
    void              initialize() override;

    /// Executes requested headless workflows and reports whether the event loop should continue.
    ApplicationStatus handleArguments( cvf::ProgramOptions* progOpt ) override;

    /// Writes formatted application text to standard output.
    void              showFormattedTextInMessageBoxOrConsole( const QString& errMsg ) override;

protected:
    // Protected implementation specific overrides
    /// Pumps the QCoreApplication event loop using @p flags.
    void invokeProcessEvents( QEventLoop::ProcessEventsFlags flags = QEventLoop::AllEvents ) override;

    /// Records a project-opening failure through the logging system.
    void onProjectOpeningError( const QString& errMsg ) override;

    /// Loads plot data and drains pending events after a project opens.
    void onProjectOpened() override;

    /// Drains pending events after project-owned objects are closed.
    void onProjectClosed() override;
};
