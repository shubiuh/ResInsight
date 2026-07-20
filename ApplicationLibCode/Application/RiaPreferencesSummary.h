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
/// @brief Summary plotting and data-reading preferences.

#pragma once

#include "cafAppEnum.h"
#include "cafPdmChildArrayField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"

#include "RiaDefines.h"
#include "RiaPlotDefines.h"

// Include to make Pdm work for cvf::Color
#include "cafPdmFieldCvfColor.h"

#include <QString>

#include <vector>

//--------------------------------------------------------------------------------------------------
/// @brief Preferences for summary data reading, plotting defaults, and curve styling.
///
/// RiaPreferencesSummary controls the summary data reader backend (RESDATA, OPM_COMMON, or
/// HDF5_OPM_COMMON), enhanced summary file creation, default plot types and layout, restart
/// file import behaviour, history curve style, cross-plot address combinations, and colour
/// settings for history curves. It also maintains the selected default plot-template set and
/// migrates older preference representations in initAfterRead(). Pair-valued fields use their
/// boolean component as an explicit enable/automatic switch rather than overloading numeric values.
//--------------------------------------------------------------------------------------------------
class RiaPreferencesSummary : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Backend used to read summary vectors.
    enum class SummaryReaderMode
    {
        RESDATA,        ///< Legacy resdata summary reader.
        OPM_COMMON,     ///< OPM common binary summary reader.
        HDF5_OPM_COMMON ///< OPM common HDF5 summary reader.
    };
    using SummaryReaderModeType = caf::AppEnum<SummaryReaderMode>;

    /// Policy for following restart-file references during summary import.
    enum class SummaryRestartFilesImportMode
    {
        IMPORT,         ///< Merge referenced restart data into the imported case.
        NOT_IMPORT,     ///< Ignore referenced restart files.
        SEPARATE_CASES  ///< Import restart files as independent summary cases.
    };
    using SummaryRestartFilesImportModeType = caf::AppEnum<SummaryRestartFilesImportMode>;

    /// Default visual representation for history vectors.
    enum class SummaryHistoryCurveStyleMode
    {
        SYMBOLS,
        LINES,
        SYMBOLS_AND_LINES
    };
    using SummaryHistoryCurveStyleModeType = caf::AppEnum<SummaryHistoryCurveStyleMode>;

    /// Source used to create plots automatically after summary import.
    enum class DefaultSummaryPlotType
    {
        NONE,
        DATA_VECTORS,
        PLOT_TEMPLATES
    };
    using DefaultSummaryPlotEnum = caf::AppEnum<DefaultSummaryPlotType>;

    using ColumnCountEnum    = caf::AppEnum<RiaDefines::ColumnCount>;
    using RowCountEnum       = caf::AppEnum<RiaDefines::RowCount>;
    using SummaryReadoutMode = caf::AppEnum<RiaDefines::ReadOutType>;

public:
    /// Initializes reader, import, plotting, template, and layout defaults.
    RiaPreferencesSummary();

    /// @return The summary preference object owned by the current application preferences.
    static RiaPreferencesSummary* current();

    /// @return Selected summary-reader backend.
    SummaryReaderMode summaryDataReader() const;
    /// @return Whether existing enhanced ESMRY data should be preferred when available.
    bool              useEnhancedSummaryDataFiles() const;
    /// @return Whether ESMRY files should be generated from imported summary data.
    bool              createEnhancedSummaryDataFiles() const;

    /// @return Whether HDF5 summary files should be generated.
    bool createH5SummaryDataFiles() const;
    /// @return Worker-thread count used for HDF5 generation.
    int  createH5SummaryDataThreadCount() const;

    /// @return Automatic post-import plot source.
    DefaultSummaryPlotType defaultSummaryPlotType() const;
    /// Returns configured default templates filtered by single-case or ensemble applicability.
    std::vector<QString>   defaultSummaryPlotTemplates( bool returnEnsembleTemplates ) const;
    /// @return true when @p filename is selected as a default plot template.
    bool                   isDefaultSummaryPlotTemplate( QString filename ) const;
    /// Adds @p filename to the selected default templates without requiring direct field access.
    void                   addToDefaultPlotTemplates( QString filename );
    /// Removes @p filename from the selected default templates.
    void                   removeFromDefaultPlotTemplates( QString filename );

    /// Adds restart-import controls to a parent preferences group.
    void appendRestartFileGroup( caf::PdmUiOrdering& uiOrdering ) const;
    /// Adds summary plotting controls to a parent preferences group.
    void appendItemsToPlottingGroup( caf::PdmUiOrdering& uiOrdering );

    /// @return Whether plot labels use a verbose summary timestamp.
    bool showSummaryTimeAsLongString() const;
    /// @return Whether independent summary cases may load concurrently.
    bool useMultipleThreadsWhenLoadingSummaryData() const;
    /// @return Whether restart discovery opens an import-policy dialog.
    bool summaryRestartFilesShowImportDialog() const;
    /// @return Whether the newer summary import pipeline is enabled.
    bool useImprovedSummaryImport() const;

    /// @return Enabled ESMRY time threshold, or the implementation default when automatic.
    int esmryTimeThreshold() const;

    /// @return Restart policy used for direct summary imports.
    SummaryRestartFilesImportMode summaryImportMode() const;
    /// @return Restart policy used when summary data is discovered from a grid import.
    SummaryRestartFilesImportMode gridImportMode() const;
    /// @return Initial text filter for summary-vector selection.
    QString                       defaultSummaryCurvesTextFilter() const;
    /// @return Whether generated curve colors are assigned by phase.
    bool                          colorCurvesByPhase() const;
    /// @return Whether corresponding history vectors are added automatically.
    bool                          appendHistoryVectors() const;

    /// @return Serialized address-pair rules used by cross-plot creation.
    QString crossPlotAddressCombinations() const;

    /// @return Default line/symbol style for history vectors.
    SummaryHistoryCurveStyleMode defaultSummaryHistoryCurveStyle() const;

    /// @return Default number of columns in generated multi-plots.
    RiaDefines::ColumnCount defaultMultiPlotColumnCount() const;
    /// @return Default number of rows per generated multi-plot page.
    RiaDefines::RowCount    defaultMultiPlotRowCount() const;

    /// @return Default summary-plot cursor/readout behavior.
    RiaDefines::ReadOutType defaultSummaryReadoutMode() const;

    /// @return Contrast color used to distinguish history curves.
    cvf::Color3f historyCurveContrastColor() const;

    /// Configures specialized template, color, and check-box/text editors.
    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;
    /// Migrates and normalizes summary preferences after deserialization.
    void initAfterRead() override;

    /// Adds summary settings that affect grid-associated summary imports.
    void appendGridFields( caf::PdmUiOrdering& uiOrdering );

protected:
    /// Defines the standalone summary-preferences layout.
    void                          defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;
    /// Supplies available default plot templates and other dynamic option lists.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;

private:
    caf::PdmField<DefaultSummaryPlotEnum> m_defaultSummaryPlot;      ///< Automatic post-import plot source.
    caf::PdmField<std::vector<QString>>   m_selectedDefaultTemplates;///< Selected default template paths.

    caf::PdmField<bool>                              m_summaryRestartFilesShowImportDialog;///< Prompt for restart import policy.
    caf::PdmField<SummaryRestartFilesImportModeType> m_summaryImportMode;                  ///< Policy for summary imports.
    caf::PdmField<SummaryRestartFilesImportModeType> m_gridImportMode;                     ///< Policy for grid-triggered imports.

    caf::PdmField<QString>                          m_defaultSummaryCurvesTextFilter;    ///< Initial summary-vector text filter.
    caf::PdmField<QString>                          m_crossPlotAddressCombinations;     ///< Serialized cross-plot pairing rules.
    caf::PdmField<SummaryHistoryCurveStyleModeType> m_defaultSummaryHistoryCurveStyle; ///< Default history curve appearance.
    caf::PdmField<bool>                             m_curveColorByPhase;                ///< Color generated curves by phase.
    caf::PdmField<bool>                             m_appendHistoryVectors;             ///< Add history vectors beside simulated vectors.
    caf::PdmField<SummaryReadoutMode>               m_summaryReadoutMode;               ///< Default plot readout behavior.

    caf::PdmField<bool> m_showSummaryTimeAsLongString;              ///< Use verbose formatted summary timestamps.
    caf::PdmField<bool> m_useMultipleThreadsWhenLoadingSummaryCases;///< Parallelize independent summary-case loading.

    caf::PdmField<bool>                 m_createEnhancedSummaryDataFile;///< Generate ESMRY cache files.
    caf::PdmField<bool>                 m_useEnhancedSummaryDataFile;   ///< Prefer existing ESMRY files.
    caf::PdmField<std::pair<bool, int>> m_esmryTimeThreshold;           ///< Optional ESMRY time threshold.

    caf::PdmField<bool> m_createH5SummaryDataFile;       ///< Generate HDF5 summary files.
    caf::PdmField<int>  m_createH5SummaryFileThreadCount;///< HDF5 generation worker count.

    caf::PdmField<SummaryReaderModeType> m_summaryReader;           ///< Selected reader backend.
    caf::PdmField<bool>                  m_useImprovedSummaryImport;///< Enable the newer import pipeline.

    caf::PdmField<ColumnCountEnum> m_defaultColumnCount; ///< Default columns in generated multi-plots.
    caf::PdmField<RowCountEnum>    m_defaultRowsPerPage; ///< Default rows per generated multi-plot page.

    caf::PdmField<cvf::Color3f> m_historyCurveContrastColor; ///< Contrast color used for history curves.
};
