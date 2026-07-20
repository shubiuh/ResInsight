/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2021     Equinor ASA
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
/// @brief System-level preferences for ResInsight.

#pragma once

#include "cafAppEnum.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"

//--------------------------------------------------------------------------------------------------
/// @brief System and debugging preferences for ResInsight.
///
/// RiaPreferencesSystem controls Eclipse text file reader mode, shader usage, HUD visibility,
/// thread count, GTest filtering, experimental feature toggles, PDF export settings, and other
/// developer- or system-level options. It also owns migration from the legacy free-text feature
/// list to the structured experimental-feature selection. Several fields intentionally expose
/// diagnostics normally hidden from end users and are grouped separately in the preferences UI.
//--------------------------------------------------------------------------------------------------
class RiaPreferencesSystem : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Strategy used when parsing large Eclipse text input files.
    enum class EclipseTextFileReaderMode
    {
        MEMORY_MAPPED_FILE, ///< Map the file into memory for random-access parsing.
        FILE,               ///< Read through conventional file I/O.
    };
    /// PDM enum wrapper used by settings serialization and option editors.
    using EclipseTextFileReaderModeType = caf::AppEnum<EclipseTextFileReaderMode>;

public:
    /// Initializes system defaults, feature metadata, and field editor attributes.
    RiaPreferencesSystem();

    /// @return The system preference object owned by the current application preferences.
    static RiaPreferencesSystem* current();

    /// Enables or disables developer class names appended to generated UI labels.
    void    setAppendClassNameToUiText( bool enable );
    /// @return Whether PDM class names are appended to generated UI labels.
    bool    appendClassNameToUiText() const;
    /// @return Whether PDM field keywords are appended to tooltip text.
    bool    appendFieldKeywordToToolTipText() const;
    /// @return Whether internal view identifiers are shown in the project tree.
    bool    showViewIdInProjectTree() const;
    /// @return Whether the regression/developer test toolbar is visible.
    bool    showTestToolbar() const;
    /// @return Whether fracture debug details are written with exported files.
    bool    includeFractureDebugInfoFile() const;
    /// @return Whether users are warned when project state changed unexpectedly.
    bool    showProjectChangedDialog() const;
    /// @return Default destination for HoloLens exports.
    QString holoLensExportFolder() const;
    /// @return Whether OpenGL shader rendering is enabled.
    bool    useShaders() const;
    /// @return Whether the 3D heads-up information overlay is visible.
    bool    show3dInformation() const;
    /// @return GoogleTest filter used by in-application test execution.
    QString gtestFilter() const;
    /// @return Whether long-running operations display progress UI.
    bool    showProgressBar() const;
    /// @return Whether PDF export displays its options dialog.
    bool    showPdfExportDialog() const;
    /// @return Scaling factor applied to exported PDF content.
    double  exportPdfScalingFactor() const;

    /// @return Configured maximum worker threads, or std::nullopt when automatic selection is enabled.
    std::optional<int> threadCount() const;

    /// @return Lower bound used when tessellating angular/radial cells.
    int minimumAngularCellCount() const;

    /// @return Selected Eclipse text-file reading strategy.
    EclipseTextFileReaderMode eclipseTextFileReaderMode() const;

    /// @return true when @p keyword is present in the configured logging keyword list.
    bool isLoggingActivatedForKeyword( const QString& keyword ) const;

    /// @return true when the stable experimental-feature @p keyword is enabled.
    bool isFeatureEnabled( const QString& keyword ) const;

    /// Adds the registered experimental-feature selector and description fields to @p uiOrdering.
    void appendExperimentalFeaturesItems( caf::PdmUiOrdering& uiOrdering );

protected:
    /// Groups system, rendering, export, diagnostics, and experimental settings for the preferences UI.
    void                          defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;

    /// Supplies registered experimental features as options for the enabled-feature field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;

    /// Configures specialized editors and read-only/transient presentation behavior.
    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;

    /// Synchronizes the selected feature description when feature selection changes.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;

    /// Migrates legacy feature keywords and normalizes state after settings deserialization.
    void initAfterRead() override;

private:
    caf::PdmField<bool> m_appendClassNameToUiText;          ///< Developer class names in UI labels.
    caf::PdmField<bool> m_appendFieldKeywordToToolTipText;  ///< PDM keywords in tooltips.
    caf::PdmField<bool> m_showViewIdInProjectTree;          ///< Internal view IDs in project-tree labels.
    caf::PdmField<bool> m_useShaders;                       ///< Shader-based 3D rendering toggle.
    caf::PdmField<bool> m_showHud;                          ///< 3D heads-up information overlay toggle.

    caf::PdmField<bool> m_showProjectChangedDialog; ///< Warning dialog for externally changed project state.

    caf::PdmField<bool>    m_showTestToolbar;              ///< Developer/regression toolbar visibility.
    caf::PdmField<bool>    m_includeFractureDebugInfoFile; ///< Include detailed fracture diagnostics in exports.
    caf::PdmField<QString> m_holoLensExportFolder;         ///< Default HoloLens export directory.

    caf::PdmField<bool>   m_showPdfExportDialog; ///< Show PDF options before exporting.
    caf::PdmField<double> m_exportScalingFactor; ///< PDF export content scaling.

    caf::PdmField<bool> m_showProgressBar; ///< Global progress UI toggle.

    caf::PdmField<int> m_mimimumAngularCellCount; ///< Minimum tessellation count for angular cells.

    caf::PdmField<QString> m_gtestFilter; ///< GoogleTest selection expression.

    caf::PdmField<EclipseTextFileReaderModeType> m_eclipseReaderMode; ///< Eclipse text parser I/O strategy.

    caf::PdmField<QString> m_keywordsForLogging; ///< Delimited subsystem keywords with verbose timing/logging.

    caf::PdmField<std::vector<QString>> m_enabledFeatures;            ///< Persisted stable feature keywords.
    caf::PdmField<QString>              m_currentFeatureKeyword;      ///< Transient highlighted feature keyword.
    caf::PdmField<QString>              m_selectedFeatureDescription; ///< Transient description shown below the list.
    caf::PdmField<QString>              m_featureKeywords;            ///< Legacy free-text field retained for migration.

    /// Optional user override for the maximum number of worker threads.
    caf::PdmField<std::pair<bool, QString>> m_maximumNumberOfThreads;
};
