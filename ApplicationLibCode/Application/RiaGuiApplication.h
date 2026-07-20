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
/// @brief Declares the graphical ResInsight application class.

#pragma once

#include "RiaApplication.h"
#include "RiaDefines.h"

#include <QApplication>
#include <QMutex>
#include <QPointer>
#include <QProcess>
#include <QString>
#include <QTimer>

#include <iostream>
#include <memory>

class QAction;

class Drawable;

class RIProcess;

class RiaPreferences;
class RiaProjectModifier;
class RiaSocketServer;

class RigEclipseCaseData;

class RimEclipseCase;
class RimEclipseView;
class RimGridView;
class RimProject;
class RimSummaryPlot;
class Rim3dView;
class RimViewWindow;
class RimWellLogPlot;
class RimWellAllocationPlot;

class RiuMainWindow;
class RiuMainWindowBase;
class RiuPlotMainWindow;
class RiuRecentFileActionProvider;
class RiaArgumentParser;

namespace RiaDefines
{
enum class RINavigationPolicy : short;
}

namespace caf
{
class FontHolderInterface;
}

//==================================================================================================
/// @brief Graphical ResInsight application class that owns the main window and the plot window.
///
/// RiaGuiApplication ties together QApplication event processing with the ResInsight application
/// services in RiaApplication. It creates and manages the main 3D viewer window and the plot main
/// window, tracks recent files, and handles window-level lifecycle events. Exactly one instance is
/// expected to exist while the GUI application is running.
///
/// Project lifecycle hooks preserve and restore tree/dock state around model replacement, update
/// window captions and recent-file actions, and ensure display models are rebuilt only after data
/// loading completes. QPointer is used for top-level windows because Qt can delete them in response
/// to user actions. The class also translates shared application operations into dialogs, progress
/// monitoring, and Qt event processing.
//==================================================================================================
class RiaGuiApplication : public QApplication, public RiaApplication
{
    Q_OBJECT

public:
    /// @return true when the shared application instance is a RiaGuiApplication.
    static bool               isRunning();

    /// @return The active GUI application; asserts when called outside GUI mode.
    static RiaGuiApplication* instance();

    /// Constructs QApplication and the shared ResInsight application services.
    RiaGuiApplication( int& argc, char** argv );

    /// Stores persistent window state and releases GUI-owned service objects.
    ~RiaGuiApplication() override;

    /// Saves to the current project path or prompts for a destination when the project is unsaved.
    bool    saveProject();

    /// @return A user-selected project path, or an empty string if the dialog is cancelled.
    QString promptForProjectSaveAsFileName() const;

    /// Saves to @p fileName and performs GUI-specific post-save updates.
    bool    saveProjectAs( const QString& fileName );

    /// Checks running jobs and unsaved changes before application or project closure.
    bool checkWithUserBeforeClose();

    /// Reuses a template project to create snapshots for a sequence of grid files.
    /// @param templateProjectFileName Project whose views and settings define each snapshot.
    /// @param gridFileNames Grid files substituted into the template.
    /// @param snapshotFolderName Destination directory for generated images.
    void runMultiCaseSnapshots( const QString& templateProjectFileName, std::vector<QString> gridFileNames, const QString& snapshotFolderName );

    /// @return Whether shader-based rendering is enabled in system preferences.
    bool useShaders() const;

    /// @return The configured 3D navigation policy.
    RiaDefines::RINavigationPolicy navigationPolicy() const;

    /// Creates, shows, raises, and returns the 3D main window.
    RiuMainWindow* getOrCreateAndShowMainWindow();

    /// @return The existing 3D main window, or nullptr before creation/after deletion.
    RiuMainWindow* mainWindow();

    /// @return The active plot view owned by the plot main window, or nullptr.
    RimViewWindow* activePlotWindow() const;

    /// Creates and returns the plot main window without requiring it to be shown.
    RiuPlotMainWindow* getOrCreateMainPlotWindow();

    /// Creates, shows, raises, and returns the plot main window.
    RiuPlotMainWindow* getOrCreateAndShowMainPlotWindow();

    /// @return The existing plot main window, or nullptr before creation/after deletion.
    RiuPlotMainWindow* mainPlotWindow();

    /// Resolves the persisted main-window identifier used by project UI state.
    RiuMainWindowBase* mainWindowByID( int mainWindowID );

    /// @return The active 3D or plot view according to the active main window.
    static RimViewWindow*     activeViewWindow();

    /// @return The active/visible top-level ResInsight main window.
    static RiuMainWindowBase* activeMainWindow();

    /// @return A suitable parent widget for application-modal dialogs, or nullptr.
    static QWidget*           widgetToUseAsParent();

    /// @return true when the 3D main window exists and is visible.
    bool isMain3dWindowVisible() const;

    /// @return true when the plot main window exists and is visible.
    bool isMainPlotWindowVisible() const;

    /// @return Actions maintained by the recent-file provider for menus and toolbars.
    std::vector<QAction*> recentFileActions() const;

    /// Clears PDM selection and any view-specific selection state.
    static void clearAllSelections();

    /// Applies GUI preferences and refreshes objects affected by font/default changes.
    /// @param oldPreferences Optional previous values used to detect expensive changes.
    /// @param defaultFontObjects Objects whose default fonts should be reapplied.
    void        applyGuiPreferences( const RiaPreferences*                         oldPreferences     = nullptr,
                                     const std::vector<caf::FontHolderInterface*>& defaultFontObjects = {} );

    /// @return Logical DPI of the primary application screen, with a safe fallback when unavailable.
    static int  applicationResolution();

    // Public RiaApplication overrides
    /// Initializes shared services, GUI logging, recent files, and application-wide UI configuration.
    void              initialize() override;

    /// Processes GUI command-line workflows and decides whether normal event processing should continue.
    ApplicationStatus handleArguments( cvf::ProgramOptions* progOpt ) override;

    /// Adds @p fileName to persistent recent-file actions.
    void              addToRecentFiles( const QString& fileName ) override;

    /// Shows formatted text in a GUI message dialog.
    void              showFormattedTextInMessageBoxOrConsole( const QString& errMsg ) override;

protected:
    /// Wraps Qt event delivery to report otherwise uncaught exceptions without terminating silently.
    bool notify( QObject* receiver, QEvent* event ) override;

    /// Warns when background work is active and returns whether closing may continue.
    bool notifyUserAboutRunningJobs();

    /// Prompts to save a modified project and returns whether closing may continue.
    bool askUserToSaveModifiedProject();

    // Protected RiaApplication overrides
    /// Processes pending QApplication events accepted by @p flags.
    void invokeProcessEvents( QEventLoop::ProcessEventsFlags flags = QEventLoop::AllEvents ) override;

    /// Updates recent files and relevant GUI state after importing @p fileName.
    void onFileSuccessfullyLoaded( const QString& fileName, RiaDefines::ImportFileType fileType ) override;

    /// Captures window state and prepares the UI before replacing the project model.
    void onProjectBeingOpened() override;

    /// Displays a project-opening error to the user.
    void onProjectOpeningError( const QString& errMsg ) override;

    /// Rebuilds windows, editors, plot data, and captions after project loading.
    void onProjectOpened() override;

    /// Stores tree/dock state and detaches project-backed UI before closure.
    void onProjectBeingClosed() override;

    /// Resets GUI state after the project model has been closed.
    void onProjectClosed() override;

    /// Captures current window/tree state into the project before serialization.
    void onProjectBeingSaved() override;

    /// Refreshes captions and modified-state indicators after a successful save.
    void onProjectSaved() override;

    /// Connects @p uiProcess to the GUI process monitor.
    void startMonitoringWorkProgress( caf::UiProcess* uiProcess ) override;

    /// Disconnects the completed worker from GUI process monitoring.
    void stopMonitoringWorkProgress() override;

    /// Extension hook invoked after GUI preferences have been applied.
    virtual void onGuiPreferencesChanged() {}

private:
    /// Derives top-level window captions from project path and modified state.
    void setWindowCaptionFromAppState();

    /// Allocates and initializes the 3D main window once.
    void createMainWindow();

    /// Allocates and initializes the plot main window once.
    void createMainPlotWindow();

    /// Serializes project-tree expansion and selection state into the project.
    void storeTreeViewState();

    /// Serializes dock-widget layouts into the project.
    void storeDockState();

    /// Restores dock-widget layouts after windows and project objects exist.
    void restoreDockState();

private slots:
    /// Continues queued multi-case script execution or finalizes worker-process monitoring.
    void slotWorkerProcessFinished( int exitCode, QProcess::ExitStatus exitStatus );

    /// Exits the application when both top-level windows have been closed.
    void onLastWindowClosed();

private:
    QPointer<RiuMainWindow>     m_mainWindow;     ///< Non-owning guarded pointer to the 3D window.
    QPointer<RiuPlotMainWindow> m_mainPlotWindow; ///< Non-owning guarded pointer to the plot window.

    /// Owns recent-file persistence and the QAction objects exposed to GUI menus.
    std::unique_ptr<RiuRecentFileActionProvider> m_recentFileActionProvider;
};
