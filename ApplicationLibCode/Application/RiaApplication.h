/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2011-     Statoil ASA
//  Copyright (C) 2013-     Ceetron Solutions AS
//  Copyright (C) 2011-2012 Ceetron AS
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
//////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Declares the shared application services used by ResInsight executables.

#pragma once

#include "KeyValueStore/RiaKeyValueStore.h"
#include "RiaDefines.h"

#include "cafPdmDeprecation.h"
#include "cafPdmPointer.h"
#include "cvfObject.h"

#include <QEventLoop>
#include <QMap>
#include <QMutex>
#include <QPointer>
#include <QProcess>
#include <QProcessEnvironment>
#include <QString>

#include <memory>

class QAction;

class Drawable;

class RIProcess;

class RiaGrpcServer;
class RiaPreferences;
class RiaProjectModifier;
class RiaSocketServer;

class RigEclipseCaseData;

class RimCommandRouter;
class RimEclipseCase;
class RimEclipseView;
class RimWellPath;
class RimGridView;
class RimProject;
class RimSummaryPlot;
class Rim3dView;
class RimViewWindow;
class RimWellLogLasFile;
class RimWellLogPlot;
class RimWellAllocationPlot;

class RiuMainWindow;
class RiuMainWindowBase;
class RiuPlotMainWindow;
class RiuRecentFileActionProvider;
class RiaArgumentParser;
class RiaOsduConnector;
class RiaSumoConnector;

namespace caf
{
class UiProcess;
}

namespace cvf
{
class ProgramOptions;
class Font;
} // namespace cvf

//==================================================================================================
/// @brief Common application services for GUI and console ResInsight executables.
///
/// RiaApplication owns the active project and other process-wide services, and coordinates project
/// loading, data import, scripting, preferences, and external processes. Derived classes provide
/// presentation- and event-loop-specific behavior through the virtual lifecycle hooks.
///
/// The class is used together with either QApplication or QCoreApplication; it does not own the Qt
/// application object itself. Exactly one RiaApplication instance is expected to exist at a time.
//==================================================================================================
class RiaApplication
{
public:
    /// Additional processing that can be requested while loading a project.
    enum class ProjectLoadAction
    {
        PLA_NONE                 = 0, ///< Load the project without additional processing.
        PLA_CALCULATE_STATISTICS = 1  ///< Recalculate statistics for grid case groups during loading.
    };

    /// Result of processing application command-line arguments.
    enum class ApplicationStatus
    {
        KEEP_GOING = 0, ///< Continue running the application.
        EXIT_COMPLETED, ///< Exit because the requested command completed successfully.
        EXIT_WITH_ERROR ///< Exit because argument processing or command execution failed.
    };

public:
    /// @return The process-wide ResInsight application instance, or nullptr before construction.
    static RiaApplication* instance();

    /// Creates the process-wide application services and registers this object as instance().
    RiaApplication();

    /// Releases the active project, cached fonts, and logging integration.
    virtual ~RiaApplication();

    /// @param includeCrtInfo Include the Microsoft C runtime linkage in the returned string when available.
    /// @return The ResInsight product version string. The returned pointer remains valid between calls.
    static const char* getVersionStringApp( bool includeCrtInfo );

    /// @return true when development-only features are enabled through RESINSIGHT_DEVEL=1.
    static bool        enableDevelopmentFeatures();

    /// Sets the 3D view used as the current application context.
    /// @param reservoirView View to activate; may be nullptr.
    void             setActiveReservoirView( Rim3dView* reservoirView );

    /// @return The active 3D reservoir view, or nullptr when no view is active.
    Rim3dView*       activeReservoirView();

    /// @return The active 3D reservoir view, or nullptr when no view is active.
    const Rim3dView* activeReservoirView() const;

    /// @return The active view as a grid view, or nullptr if it is not a grid view.
    RimGridView*     activeGridView();

    /// @return The comparison grid view under the current pick, otherwise the active grid view.
    RimGridView*     activeMainOrComparisonGridView();

    /// @return The case ID currently supplied to a script, or -1 when no case is selected.
    int              currentScriptCaseId() const;

    /// @return The project owned by the application.
    RimProject*       project();

    /// @return The project-independent command processing hub.
    RimCommandRouter* commandRouter();

    /// Applies the command-line or preference-based OpenMP thread-count setting.
    void setThreadCount() const;

    /// Creates the basic development mock reservoir model.
    void createMockModel();

    /// Creates a development mock model containing result data.
    void createResultsMockModel();

    /// Creates a large development mock model containing result data.
    void createLargeResultsMockModel();

    /// Creates the configurable development mock model.
    void createMockModelCustomized();

    /// Creates a mock Eclipse input model and its property data.
    void createInputMockModel();

    /// Opens a supported project, reservoir, geomechanical, or ensemble input.
    /// @param fileName File or directory to open.
    /// @return true when the input was recognized and loaded successfully.
    bool openFile( const QString& fileName );

    /// @return true when the current project has a persistent project-file path.
    bool    isProjectSavedToDisc() const;

    /// @return The absolute directory containing the current project, or an empty string if unsaved.
    QString currentProjectPath() const;

    /// Resolves a project-relative path against the current project directory.
    /// @param projectRelativePath Relative or absolute path to resolve.
    /// @return An absolute path when a project location is available.
    QString createAbsolutePathFromProjectRelativePath( QString projectRelativePath );

    /// Loads a project using the default load action.
    /// @param projectFileName ResInsight project file to load.
    /// @return true when project loading completes.
    bool loadProject( const QString& projectFileName );

    /// Loads and initializes a project and all referenced external data.
    /// @param projectFileName ResInsight project file to load.
    /// @param loadAction Optional processing performed during loading.
    /// @param projectModifier Optional modifier applied after deserialization and before data loading.
    /// @return true when project loading completes.
    bool loadProject( const QString& projectFileName, ProjectLoadAction loadAction, RiaProjectModifier* projectModifier );

    /// Saves the project to its existing file name.
    /// @param errorMessage Receives a user-facing error message on failure.
    /// @return true when the project was written successfully.
    bool saveProject( QString* errorMessage );

    /// Saves the project to a specified file.
    /// @param fileName Destination project file.
    /// @param errorMessage Receives a user-facing error message on failure.
    /// @return true when the project was written successfully.
    bool saveProjectAs( const QString& fileName, QString* errorMessage );

    /// @return true when @p fileName has a supported ResInsight project extension.
    static bool hasValidProjectFileExtension( const QString& fileName );

    /// Closes the current project and resets project-related caches and UI change tracking.
    void        closeProject();

    /// @param dialogName Stable identifier for the dialog.
    /// @return The most recently used directory for the named dialog, or the startup directory.
    QString lastUsedDialogDirectory( const QString& dialogName );

    /// @param dialogName Stable identifier for the dialog.
    /// @return The named dialog directory, falling back to the current project directory.
    QString lastUsedDialogDirectoryWithFallbackToProjectFolder( const QString& dialogName );

    /// @param dialogName Stable identifier for the dialog.
    /// @param fallbackDirectory Directory to return when no value has been stored.
    /// @return The named dialog directory, falling back to @p fallbackDirectory.
    QString lastUsedDialogDirectoryWithFallback( const QString& dialogName, const QString& fallbackDirectory );

    /// Stores the last directory used by a named file dialog.
    /// @param dialogName Stable identifier for the dialog.
    /// @param directory Directory to remember.
    void    setLastUsedDialogDirectory( const QString& dialogName, const QString& directory );

    /// Imports an ABAQUS ODB geomechanical case.
    /// @param fileName ODB file to import.
    /// @param applyTimeStepFilter Apply the configured time-step filter while importing.
    /// @return true when the case was imported successfully.
    bool openOdbCaseFromFile( const QString& fileName, bool applyTimeStepFilter = false );

    /// Imports well-path geometry files into the current project.
    /// @param wellPathFilePaths Files to import.
    /// @param errorMessages Receives non-fatal import errors.
    /// @return The well paths created by successful imports.
    std::vector<RimWellPath*>       addWellPathsToModel( QList<QString> wellPathFilePaths, QStringList* errorMessages );

    /// Imports formation data and associates it with well paths in the current project.
    /// @param wellPathFormationsFilePaths Formation files to import.
    void                            addWellPathFormationsToModel( QList<QString> wellPathFormationsFilePaths );

    /// Imports LAS well-log files into the current project.
    /// @param wellLogFilePaths Files to import.
    /// @param errorMessages Receives non-fatal import errors.
    /// @return The well-log objects created by successful imports.
    std::vector<RimWellLogLasFile*> addWellLogsToModel( const QList<QString>& wellLogFilePaths, QStringList* errorMessages );

    /// @return The configured script search directories.
    QString scriptDirectories() const;

    /// @return The configured external script editor executable.
    QString scriptEditorPath() const;

    /// @return The configured GNU Octave executable.
    QString             octavePath() const;

    /// @return Command-line arguments configured for GNU Octave.
    QStringList         octaveArguments() const;

    /// @return The environment used when launching GNU Octave.
    QProcessEnvironment octaveProcessEnvironment() const;

    /// @return The configured Python executable.
    QString                     pythonPath() const;

    /// @return The environment used when launching Python. Derived applications may add variables.
    virtual QProcessEnvironment pythonProcessEnvironment() const;

    /// @return The active gRPC server port, or std::nullopt when gRPC is unavailable.
    virtual std::optional<int>  activeGrpcPortNumber() const;

    /// Starts one external worker process.
    /// @param program Executable to start.
    /// @param arguments Arguments passed to the executable.
    /// @param processEnvironment Environment supplied to the child process.
    /// @return true when the process was started; false if another worker is active or startup failed.
    bool launchProcess( const QString& program, const QStringList& arguments, const QProcessEnvironment& processEnvironment );

    /// Runs an external program sequentially for the supplied case IDs.
    /// @param program Executable to start.
    /// @param arguments Arguments reused for each execution.
    /// @param caseIds Case IDs exposed to successive executions.
    /// @param processEnvironment Environment supplied to each child process.
    /// @return true when the first process was started successfully.
    bool launchProcessForMultipleCases( const QString&             program,
                                        const QStringList&         arguments,
                                        const std::vector<int>&    caseIds,
                                        const QProcessEnvironment& processEnvironment );

    /// Terminates the active external worker process, if any.
    void terminateProcess();

    /// Processes Qt events while waiting for all scheduled external work to finish.
    void waitForProcess() const;

    /// @return The application preferences object.
    RiaPreferences* preferences();

    /// Applies preferences to fonts, project paths, editors, and progress reporting.
    void            applyPreferences();

    /// @return Formatted version, copyright, and command-line help text.
    QString commandLineParameterHelp();

    /// Sets the implementation-specific command-line option description.
    /// @param commandLineHelpText Text appended to the standard application help header.
    void    setCommandLineHelpText( const QString& commandLineHelpText );

    /// Stores transient process-local data under @p key.
    /// @param key Cache entry identifier.
    /// @param dataObject Value to store.
    void     setCacheDataObject( const QString& key, const QVariant& dataObject );

    /// @param key Cache entry identifier.
    /// @return Transient data stored under @p key, or an invalid QVariant when absent.
    QVariant cacheDataObject( const QString& key ) const;

    /// Reads and executes commands from a ResInsight command file.
    /// @param commandFile Command file to execute.
    void executeCommandFile( const QString& commandFile );

    /// @return The directory in which the application was started.
    const QString startDir() const;

    /// Sets the directory in which the application was started.
    /// @param startDir Startup directory to remember.
    void          setStartDir( const QString& startDir );

    /// Reads non-empty file-name entries from a text file.
    /// @param listFileName Text file containing one file name per line.
    /// @return File names in their original order, or an empty vector if the file cannot be read.
    static std::vector<QString> readFileListFromTextFile( QString listFileName );

    /// @return The cached default font used for 3D scenes.
    cvf::Font* defaultSceneFont();

    /// @param fontSize Absolute point size of the requested font.
    /// @return A cached scene font with the requested point size.
    cvf::Font* sceneFont( int fontSize );

    /// @return The cached default annotation font.
    cvf::Font* defaultAnnotationFont();

    /// @return The cached default well-label font.
    cvf::Font* defaultWellLabelFont();

    // Public implementation specific overrides
    /// Initializes shared preferences, project state, logging, cloud services, and data loaders.
    /// Derived implementations should call this base implementation before adding their own services.
    virtual void              initialize();

    /// Processes parsed command-line options for the concrete application type.
    /// @param progOpt Parsed command-line options.
    /// @return Whether the application should continue, exit successfully, or exit with an error.
    virtual ApplicationStatus handleArguments( cvf::ProgramOptions* progOpt ) = 0;

    /// Records a successfully opened file in the implementation-specific recent-file list.
    /// @param fileName File to record.
    virtual void              addToRecentFiles( const QString& fileName ) {}

    /// Presents formatted information using a GUI message box or the console.
    /// @param errMsg Formatted text to present.
    virtual void              showFormattedTextInMessageBoxOrConsole( const QString& errMsg ) = 0;

    /// Creates or returns the process-wide OSDU connector.
    /// @return The connector, or nullptr when SSL support is unavailable.
    RiaOsduConnector* makeOsduConnector();

    /// Creates or returns the process-wide SUMO connector.
    /// @return The connector, or nullptr while regression tests are running.
    RiaSumoConnector* makeSumoConnector();

    /// @return The application-wide binary key-value store.
    RiaKeyValueStore<char>* keyValueStore() const;

protected:
    // Protected implementation specific overrides
    /// Allows the concrete Qt application to process pending events.
    /// @param flags Categories of events that may be processed.
    virtual void invokeProcessEvents( QEventLoop::ProcessEventsFlags flags = QEventLoop::AllEvents ) = 0;

    /// Called after a non-project input file has loaded successfully.
    /// @param fileName File that was loaded.
    /// @param fileType Detected import type of the file.
    virtual void onFileSuccessfullyLoaded( const QString& fileName, RiaDefines::ImportFileType fileType ) {}

    /// Called immediately before an existing project is replaced by a loaded project.
    virtual void onProjectBeingOpened() {}

    /// Called after the project model and its external data have been initialized.
    virtual void onProjectOpened()                              = 0;

    /// Reports an error encountered while opening a project.
    /// @param errMsg User-facing description of the error.
    virtual void onProjectOpeningError( const QString& errMsg ) = 0;

    /// Called immediately before the current project is closed.
    virtual void onProjectBeingClosed() {}

    /// Called after the current project has been closed.
    virtual void onProjectClosed() = 0;

    /// Called immediately before the current project is serialized.
    virtual void onProjectBeingSaved() {}

    /// Called after the current project has been serialized successfully.
    virtual void onProjectSaved() {}

    /// Begins implementation-specific monitoring of an external worker process.
    /// @param uiProcess Worker process to monitor; owned by RiaApplication.
    virtual void startMonitoringWorkProgress( caf::UiProcess* uiProcess ) {}

    /// Stops implementation-specific monitoring of the external worker process.
    virtual void stopMonitoringWorkProgress() {}

    /// Loads data for all plots and updates their presentation models.
    void loadAndUpdatePlotData();

    friend class RiaRegressionTestRunner;

    /// Replaces project and preference state with freshly initialized objects.
    void resetProject();

    /// Generates PDM API or command documentation based on the destination extension.
    /// @param outputPath Requested output file; its extension selects the generator.
    /// @param errMsg Receives an error or generator diagnostic message.
    /// @return true when all requested output was generated successfully.
    bool generateCode( const QString& outputPath, QString* errMsg );

    /// @return Deprecation rules applied while reading project files.
    static std::vector<caf::PdmDeprecation> defaultDeprecations();

protected:
    /// Registers lazy loaders for well-path geometry and well-log data.
    void initializeDataLoadController();

    /// Reads the logging level from the original Qt command-line arguments.
    void parseLogLevelFromQtArguments();

    cvf::ref<cvf::Font> m_defaultSceneFont;      ///< Cached font for 3D scene text.
    cvf::ref<cvf::Font> m_defaultAnnotationFont; ///< Cached font for annotations.
    cvf::ref<cvf::Font> m_defaultWellLabelFont;  ///< Cached font for well labels.

    caf::PdmPointer<Rim3dView>  m_activeReservoirView; ///< Current 3D view; cleared if its PDM object is deleted.
    std::unique_ptr<RimProject> m_project;             ///< Project model owned by the application.

    std::unique_ptr<RimCommandRouter> m_commandRouter; ///< Project-independent processing command hub.

    QPointer<RiaSocketServer>       m_socketServer;  ///< Optional legacy socket server.
    std::unique_ptr<caf::UiProcess> m_workerProcess; ///< Active external script process.

    std::unique_ptr<RiaKeyValueStore<char>> m_keyValueStore; ///< Process-wide binary key-value store.

    // Execute for all settings
    std::list<int>                  m_scriptCaseIds;       ///< Remaining case IDs in a multi-case script run.
    int                             m_currentScriptCaseId; ///< Case ID exposed to the active script.
    QString                         m_currentProgram;      ///< Executable reused for a multi-case script run.
    QStringList                     m_currentArguments;    ///< Arguments reused for a multi-case script run.
    std::unique_ptr<RiaPreferences> m_preferences;         ///< Application preference model.

    std::map<QString, QString> m_fileDialogDefaultDirectories; ///< Last directory used by each named dialog.
    QString                    m_startupDefaultDirectory;       ///< Directory used as the initial dialog fallback.
    QString                    m_commandLineHelpText;           ///< Concrete application's command-line help body.
    QMap<QString, QVariant>    m_sessionCache;                  ///< Transient session values such as credentials.

    QString m_preferencesFileName; ///< Optional preferences file that overrides the application settings store.

    bool m_runningWorkerProcess; ///< true while an external worker process is active.

    std::optional<int> m_threadCountFromCommandLine; ///< Command-line override for the OpenMP thread count.
    std::optional<int> m_logLevelFromCommandLine;    ///< Command-line override for the logging level.

private:
    static RiaApplication*     s_riaApplication;
    QPointer<RiaOsduConnector> m_osduConnector;
    QPointer<RiaSumoConnector> m_sumoConnector;
};
