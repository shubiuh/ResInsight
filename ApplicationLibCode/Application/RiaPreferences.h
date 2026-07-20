/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2019-     Equinor ASA
//  Copyright (C) 2011-2018 Statoil ASA
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
/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Application-wide preferences model.

#pragma once

#include "RiaDateTimeDefines.h"
#include "RiaDefines.h"
#include "RiaFontCache.h"

#include "cafAppEnum.h"
#include "cafPdmChildField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"

// Include to make Pdm work for cvf::Color
#include "cafPdmFieldCvfColor.h"

#include <QPageLayout>
#include <QPageSize>
#include <QStringList>

#include <map>
#include <string>

class RiaPreferencesSummary;
class RiaPreferencesGeoMech;
class RiaPreferencesSystem;
class RiaPreferencesOsdu;
class RiaPreferencesGrid;
class RiaPreferencesSumo;
class RiaPreferencesOpm;
class RiaPreferencesOpenTelemetry;

//--------------------------------------------------------------------------------------------------
/// @brief Root preferences object aggregating all ResInsight setting groups.
///
/// RiaPreferences is the top-level PDM preferences object. It owns child preference objects for
/// grid, geo-mechanical, summary, system, OSDU, SUMO, OPM, and OpenTelemetry settings, and
/// exposes convenience accessors for frequently used values such as font sizes, colors, script
/// paths, page layout, and 3D view defaults.
///
/// Values can be read from Qt's application settings store or imported from an explicit PDM/XML
/// file. Child objects keep subsystem-specific fields isolated while this root defines the tabbed
/// editor layout and performs cross-field migration and validation after deserialization. Public
/// PDM fields are retained for settings consumed directly by several UI and scripting components.
//--------------------------------------------------------------------------------------------------
class RiaPreferences : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// PDM wrappers used to serialize and edit font and page-layout enums.
    using FontSizeEnum        = RiaFontCache::FontSizeEnum;
    using PageSizeEnum        = caf::AppEnum<QPageSize::PageSizeId>;
    using PageOrientationEnum = caf::AppEnum<QPageLayout::Orientation>;

    /// @return Whether newly created grid views initially enable fault display.
    bool enableFaultsByDefault() const;

public:
    /// Constructs the root fields and all owned subsystem preference objects.
    RiaPreferences();

    /// Releases owned child preference objects through their PDM child fields.
    ~RiaPreferences() override;

    /// @return The root preferences owned by the current application.
    static RiaPreferences* current();

    /// @return Stable preferences tab names in display order.
    QStringList tabNames();

    /// @return Whether command-based undo/redo integration is enabled.
    bool useUndoRedo() const;

    /// @return Configured Qt date format string.
    const QString& dateFormat() const;
    /// @return Configured Qt time format string.
    const QString& timeFormat() const;
    /// Combines configured date/time formats using the requested components.
    QString
        dateTimeFormat( RiaDefines::DateFormatComponents dateComponents = RiaDefines::DateFormatComponents::DATE_FORMAT_YEAR_MONTH_DAY,
                        RiaDefines::TimeFormatComponents timeComponents = RiaDefines::TimeFormatComponents::TIME_FORMAT_HOUR_MINUTE_SECOND ) const;

    /// @return Maximum directory recursion depth used while discovering scripts.
    int maxScriptFoldersDepth() const;
    /// @return Maximum directory recursion depth used while discovering plot templates.
    int maxPlotTemplateFoldersDepth() const;

    /// @return Configured plot-template root folders.
    QStringList plotTemplateFolders() const;
    /// Appends @p folder to the persisted plot-template roots.
    void        appendPlotTemplateFolders( const QString& folder );
    /// @return Absolute path of the most recently used plot template.
    QString     lastUsedPlotTemplateAbsolutePath() const;
    /// Stores @p templatePath as the most recently used plot template.
    void        setLastUsedPlotTemplatePath( const QString& templatePath );
    /// @return Whether exported PDFs are opened in the system viewer.
    bool        openExportedPdfInViewer() const;
    /// @return Whether exported GRDECL files contain ECHO directives.
    bool        writeEchoInGrdeclFiles() const;
    /// @return Whether file dialogs prefer the most recently used directory.
    bool        useRecentlyUsedFolderAsDefault() const;

    /// @return Selected application color theme.
    RiaDefines::ThemeEnum guiTheme() const;

    /// @return Font size selected for each application font role.
    std::map<RiaDefines::FontSettingType, RiaFontCache::FontSize> defaultFontSizes() const;

    /// Writes the complete PDM preference tree to Qt's application settings store.
    void        writePreferencesToApplicationStore();
    /// @return Default page size, orientation, and margins for printable plots.
    QPageLayout defaultPageLayout() const;
    /// @return Configured page margins in the layout's expected units.
    QMarginsF   margins() const;

    /// @return Target sample spacing used while importing surfaces.
    double surfaceImportResamplingDistance() const;

    /// @return Pattern used to recognize or generate multi-lateral well names.
    QString        multiLateralWellNamePattern() const;
    /// @return Built-in multi-lateral well-name pattern.
    static QString defaultMultiLateralWellNamePattern();

    /// @return Folder used for persisted grid-calculation expressions.
    QString gridCalculationExpressionFolder() const;
    /// @return Folder used for persisted summary-calculation expressions.
    QString summaryCalculationExpressionFolder() const;

    // 3D view
    /// @return Default mesh rendering mode for new 3D views.
    RiaDefines::MeshModeType       defaultMeshModeType() const;
    /// @return Default mouse/navigation policy for 3D views.
    RiaDefines::RINavigationPolicy navigationPolicy() const;
    /// @return Default vertical exaggeration for new 3D views.
    double                         defaultScaleFactorZ() const;
    /// @return Whether new views show a legend background.
    bool                           showLegendBackground() const;
    /// @return Whether new views show their information box.
    bool                           showInfoBox() const;
    /// @return Whether new views show the grid bounding box.
    bool                           showGridBox() const;

    // Script paths
    /// @return Configured Python executable for external scripts.
    QString pythonExecutable() const;

    // Octave
    /// @return Configured GNU Octave executable.
    QString octaveExecutable() const;
    /// @return Whether Octave prints its startup header for script execution.
    bool    octaveShowHeaderInfoWhenExecutingScripts() const;
    /// @return Optional Octave socket port as text, or an empty string when automatic.
    QString octavePortNumber() const;

    /// @return Whether saving a project also retains a backup copy.
    bool storeBackupOfProjectFiles() const;

    /// @return Owned geo-mechanical import and visualization preferences.
    RiaPreferencesGeoMech* geoMechPreferences() const;
    /// @return Owned summary-data and plotting preferences.
    RiaPreferencesSummary* summaryPreferences() const;
    /// @return Owned platform, logging, and rendering-system preferences.
    RiaPreferencesSystem* systemPreferences() const;
    /// @return Owned OSDU connection preferences.
    RiaPreferencesOsdu* osduPreferences() const;
    /// @return Owned SUMO connection preferences.
    RiaPreferencesSumo* sumoPreferences() const;
    /// @return Owned Eclipse-grid import preferences.
    RiaPreferencesGrid* gridPreferences() const;
    /// @return Owned OPM Flow integration preferences.
    RiaPreferencesOpm* opmPreferences() const;
    /// @return Owned telemetry export preferences.
    RiaPreferencesOpenTelemetry* openTelemetryPreferences() const;

    /// Imports the preference tree from @p fileName, overriding current field values.
    void importPreferenceValuesFromFile( const QString& fileName );
    /// Exports the current preference tree to @p fileName.
    void exportPreferenceValuesToFile( const QString& fileName );

public:
    /// Enables the embedded gRPC automation server at application startup.
    caf::PdmField<bool> enableGrpcServer;
    /// TCP port used by the embedded gRPC automation server.
    caf::PdmField<int>  defaultGrpcPortNumber;

    /// Semicolon-separated root directories searched for user scripts.
    caf::PdmField<QString> scriptDirectories;
    /// External editor launched for script source files.
    caf::PdmField<QString> scriptEditorExecutable;
    /// Enables verbose diagnostic output from Python script execution.
    caf::PdmField<bool>    showPythonDebugInfo;

    /// Default line color for grid cells in newly created views.
    caf::PdmField<cvf::Color3f> defaultGridLineColors;
    /// Default line color for fault faces in newly created views.
    caf::PdmField<cvf::Color3f> defaultFaultGridLineColors;
    /// Default clear color for 3D viewers.
    caf::PdmField<cvf::Color3f> defaultViewerBackgroundColor;
    /// Default label color for wells in 3D viewers.
    caf::PdmField<cvf::Color3f> defaultWellLabelColor;

    /// Default font size for text rendered in 3D scenes.
    caf::PdmField<FontSizeEnum> defaultSceneFontSize;
    /// Default font size for well labels.
    caf::PdmField<FontSizeEnum> defaultWellLabelFontSize;
    /// Default font size for annotations.
    caf::PdmField<FontSizeEnum> defaultAnnotationFontSize;
    /// Default font size for plot text.
    caf::PdmField<FontSizeEnum> defaultPlotFontSize;

    /// Allows HoloLens connections to accept certificates that cannot be verified.
    caf::PdmField<bool>    holoLensDisableCertificateVerification;
    /// Field delimiter used by plain-text and CSV exporters.
    caf::PdmField<QString> csvTextExportFieldSeparator;

protected:
    /// Configures field-specific widgets, including file and directory selectors.
    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;
    /// Builds the tab and group layout used by the preferences dialog.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;
    /// Supplies selectable values for fields whose options depend on the runtime environment.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;
    /// Migrates and normalizes values after settings have been deserialized.
    void                          initAfterRead() override;
    /// Applies immediate side effects required when selected preferences change in the UI.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;

private:
    /// @name Stable preferences-dialog tab labels
    /// These helpers keep tab keys identical in construction and UI-ordering code.
    /// @{
    static QString tabNameGeneral();
    static QString tabNameGrid();
    static QString tabNameSummary();
    static QString tabNameGeomech();
    static QString tabNamePlotting();
    static QString tabNameScripting();
    static QString tabNameSystem();
    static QString tabNameImportExport();
    static QString tabNameOpmFlow();
    static QString tabNameExperimental();
    /// @}

    /// @return A suitable default print margin for the physical size of @p pageSizeId.
    static double defaultMarginSize( QPageSize::PageSizeId pageSizeId );

private:
    /// Date presentation pattern used throughout the UI and exports.
    caf::PdmField<QString> m_dateFormat;
    /// Time presentation pattern used throughout the UI and exports.
    caf::PdmField<QString> m_timeFormat;

    /// Enables the application-wide command undo stack.
    caf::PdmField<bool> m_useUndoRedo;

    /// Selected light, dark, or system-derived GUI theme.
    caf::PdmField<caf::AppEnum<RiaDefines::ThemeEnum>> m_guiTheme;

    /// Recursion limit for scanning script directory trees.
    caf::PdmField<int> m_maxScriptFoldersDepth;

    /// Default physical paper size used by PDF and print exporters.
    caf::PdmField<PageSizeEnum>        m_pageSize;
    /// Default portrait or landscape paper orientation.
    caf::PdmField<PageOrientationEnum> m_pageOrientation;
    /// Default left page margin.
    caf::PdmField<double>              m_pageLeftMargin;
    /// Default right page margin.
    caf::PdmField<double>              m_pageRightMargin;
    /// Default top page margin.
    caf::PdmField<double>              m_pageTopMargin;
    /// Default bottom page margin.
    caf::PdmField<double>              m_pageBottomMargin;
    /// Opens a generated PDF using the operating-system viewer.
    caf::PdmField<bool>                m_openExportedPdfInViewer;
    /// Emits ECHO control records when exporting GRDECL data.
    caf::PdmField<bool>                m_writeEchoInGrdeclFiles;
    /// Reuses the last visited directory as the next file-dialog start directory.
    caf::PdmField<bool>                m_useRecentlyUsedFolderAsDefault;

    /// Serialized list of root directories searched for plot templates.
    caf::PdmField<QString>       m_plotTemplateFolders;
    /// Recursion limit for scanning plot-template directory trees.
    caf::PdmField<int>           m_maxPlotTemplateFoldersDepth;
    /// Path of the most recently selected plot template.
    caf::PdmField<caf::FilePath> m_lastUsedPlotTemplate;

    /// Default directory for grid-calculation expression files.
    caf::PdmField<caf::FilePath> m_gridCalculationExpressionFolder;
    /// Default directory for summary-calculation expression files.
    caf::PdmField<caf::FilePath> m_summaryCalculationExpressionFolder;

    /// Executable used to run GNU Octave scripts.
    caf::PdmField<QString>                  m_octaveExecutable;
    /// Controls whether Octave startup information is retained in script output.
    caf::PdmField<bool>                     m_octaveShowHeaderInfoWhenExecutingScripts;
    /// Optional explicitly enabled Octave server port and its textual value.
    caf::PdmField<std::pair<bool, QString>> m_octavePortNumber;

    /// Executable used to run Python scripts.
    caf::PdmField<QString> m_pythonExecutable;

    /// Creates a recoverable backup before replacing a project file.
    caf::PdmField<bool> m_storeBackupOfProjectFile;

    /// Target spacing used when imported surface data is resampled.
    caf::PdmField<double> m_surfaceImportResamplingDistance;

    /// Naming pattern used to associate imported multi-lateral well paths.
    caf::PdmField<QString> m_multiLateralWellPattern;

    /// Owned settings specific to Eclipse-grid import.
    caf::PdmChildField<RiaPreferencesGrid*> m_gridPreferences;

    /// Owned settings specific to geo-mechanical data.
    caf::PdmChildField<RiaPreferencesGeoMech*> m_geoMechPreferences;

    /// Owned settings for OPM Flow execution and discovery.
    caf::PdmChildField<RiaPreferencesOpm*> m_opmPreferences;

    /// Owned settings for summary readers, calculations, and plots.
    caf::PdmChildField<RiaPreferencesSummary*> m_summaryPreferences;

    /// Owned settings for logging, rendering, paths, and platform integration.
    caf::PdmChildField<RiaPreferencesSystem*> m_systemPreferences;

    /// Owned OSDU service connection settings.
    caf::PdmChildField<RiaPreferencesOsdu*> m_osduPreferences;

    /// Owned SUMO service connection settings.
    caf::PdmChildField<RiaPreferencesSumo*> m_sumoPreferences;

    /// Owned OpenTelemetry endpoint and resource settings.
    caf::PdmChildField<RiaPreferencesOpenTelemetry*> m_openTelemetryPreferences;

    /// Default surface/mesh representation for new 3D views.
    caf::PdmField<caf::AppEnum<RiaDefines::MeshModeType>>       m_defaultMeshModeType;
    /// Mouse interaction policy used by new 3D views.
    caf::PdmField<caf::AppEnum<RiaDefines::RINavigationPolicy>> m_navigationPolicy;
    /// Initial vertical exaggeration used by new 3D views.
    caf::PdmField<double>                                       m_defaultScaleFactorZ;
    /// Shows an opaque background behind legends in new views.
    caf::PdmField<bool>                                         m_showLegendBackground;
    /// Enables fault geometry in new grid views.
    caf::PdmField<bool>                                         m_enableFaultsByDefault;
    /// Shows the view information overlay in new views.
    caf::PdmField<bool>                                         m_showInfoBox;
    /// Shows the grid bounding box in new views.
    caf::PdmField<bool>                                         m_showGridBox;

    /// Cached ordered tab labels used when constructing the preferences editor.
    QStringList m_tabNames;
};
