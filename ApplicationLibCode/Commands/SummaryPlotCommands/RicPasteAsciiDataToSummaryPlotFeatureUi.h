/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017 Statoil ASA
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
/// @brief Declares paste ASCII data to summary plot feature command support.

#pragma once

#include "RimPlotCurve.h"

#include "RifAsciiDataParseOptions.h"
#include "RifCsvUserDataParser.h"
#include "RifEclipseSummaryAddressDefines.h"

#include "cafAppEnum.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"

#include <QLocale>
#include <QString>

#include <memory>

//==================================================================================================
/// @brief UI model for configuring paste ASCII data to summary plot feature.
//==================================================================================================
class RicPasteAsciiDataToSummaryPlotFeatureUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    enum UiMode
    {
        UI_MODE_NONE,
        UI_MODE_IMPORT,
        UI_MODE_PASTE,
        UI_MODE_SILENT
    };

    enum DecimalSeparator
    {
        DECIMAL_COMMA,
        DECIMAL_DOT,
    };

    using DecimalSeparatorEnum = caf::AppEnum<DecimalSeparator>;

    enum DateFormat
    {
        DATE_DDMMYYYY_DOT_SEPARATED,
        DATE_DDMMYYYY_DASH_SEPARATED,
        DATE_DDMMYYYY_SLASH_SEPARATED,
        DATE_YYYYMMDD_DOT_SEPARATED,
        DATE_YYYYMMDD_DASH_SEPARATED,
        DATE_YYYYMMDD_SLASH_SEPARATED,
        DATE_MMDDYYYY_SLASH_SEPARATED,
        DATE_MMDDYY_SLASH_SEPARATED
    };

    using DateFormatEnum = caf::AppEnum<DateFormat>;

    enum TimeFormat
    {
        TIME_NONE,
        TIME_HHMM,
        TIME_HHMMSS,
        TIME_HHMMSSZZZ,
    };

    using TimeFormatEnum = caf::AppEnum<TimeFormat>;

    enum CellSeparator
    {
        CELL_COMMA,
        CELL_TAB,
        CELL_SEMICOLON
    };

    using CellSeparatorEnum = caf::AppEnum<CellSeparator>;

public:
    /// Constructs the command object.
    RicPasteAsciiDataToSummaryPlotFeatureUi();

    /// Sets UI mode import.
    void setUiModeImport( const QString& fileName );
    /// Sets UI mode paste text.
    void setUiModePasteText( const QString& text );

    /// @return The UI mode import.
    UiMode                         uiModeImport() const;
    /// @return The parse options.
    const RifAsciiDataParseOptions parseOptions() const;
    /// Sets create new plot.
    void                           setCreateNewPlot();

    /// @return The context string.
    QString contextString() const;

    /// @return The date format from string.
    static DateFormat dateFormatFromString( const QString& dateString );

protected:
    /// Defines the field ordering used by the command UI.
    void                          defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;
    /// @return The selectable values for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;
    /// Performs the define editor attribute command operation.
    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;
    /// Responds to a value changed through the command UI.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;

private:
    /// Performs the initialize command operation.
    void initialize( RifCsvUserDataParser* parser );
    /// Updates preview text and date format.
    void updatePreviewTextAndDateFormat();

private:
    UiMode m_uiMode;

    caf::PdmField<QString>              m_plotTitle;
    caf::PdmField<QString>              m_curvePrefix;
    caf::PdmField<DecimalSeparatorEnum> m_decimalSeparator;
    caf::PdmField<DateFormatEnum>       m_dateFormat;
    caf::PdmField<TimeFormatEnum>       m_timeFormat;
    caf::PdmField<bool>                 m_useCustomDateFormat;
    caf::PdmField<QString>              m_customDateTimeFormat;
    caf::PdmField<CellSeparatorEnum>    m_cellSeparator;
    caf::PdmField<QString>              m_timeSeriesColumnName;

    caf::PdmField<caf::AppEnum<RiuQwtPlotCurveDefines::LineStyleEnum>> m_curveLineStyle;
    caf::PdmField<caf::AppEnum<RiuPlotCurveSymbol::PointSymbolEnum>>   m_curveSymbol;
    caf::PdmField<float>                                               m_curveSymbolSkipDistance;

    bool                   m_createNewPlot;
    caf::PdmField<QString> m_previewText;

    std::unique_ptr<RifCsvUserDataParser> m_parser;
};
