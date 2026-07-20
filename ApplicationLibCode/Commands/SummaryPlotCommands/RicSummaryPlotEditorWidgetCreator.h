/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017-     Statoil ASA
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
/// @brief Declares summary plot editor widget creator command support.

#pragma once

#include "cafPdmUiFormLayoutObjectEditor.h"

#include <memory>
#include <vector>

class RimSummaryPlot;
class RimSummaryMultiPlot;
class RimSummaryCase;
class RicSummaryPlotEditorUi;

class QMinimizePanel;
class QSplitter;
class QString;
class QVBoxLayout;
class QHBoxLayout;
class QBoxLayout;

/// @brief Utilities for caf command workflows.
namespace caf
{
class PdmObject;
class PdmUiItem;
class PdmUiTreeView;
} // namespace caf

//==================================================================================================
/// @brief Supports summary plot editor widget creator command workflows.
//==================================================================================================
class RicSummaryPlotEditorWidgetCreator : public caf::PdmUiFormLayoutObjectEditor
{
    Q_OBJECT

public:
    /// Constructs the command object.
    RicSummaryPlotEditorWidgetCreator( QWidget* parent );
    /// Destroys the command object.
    ~RicSummaryPlotEditorWidgetCreator() override;

    /// Updates from summary plot.
    void updateFromSummaryPlot( RimSummaryPlot* summaryPlot );
    /// Updates from summary multi plot.
    void updateFromSummaryMultiPlot( RimSummaryMultiPlot* summaryMultiPlot );
    /// Updates from default sources.
    void updateFromDefaultSources( const std::vector<caf::PdmObject*> defaultSources );

private:
    /// Performs the recursively configure and update top level UI ordering command operation.
    void recursivelyConfigureAndUpdateTopLevelUiOrdering( const caf::PdmUiOrdering& topLevelUiOrdering, const QString& uiConfigName ) override;

    /// Creates widget.
    QWidget* createWidget( QWidget* parent ) override;

    /// @return The or create curve tree group.
    QMinimizePanel* getOrCreateCurveTreeGroup();
    /// @return The or create plot widget.
    QWidget*        getOrCreatePlotWidget();

    /// @return The matching group by keyword.
    static caf::PdmUiGroup*
        findGroupByKeyword( const std::vector<caf::PdmUiItem*>& topLevelUiItems, const QString& keyword, const QString& uiConfigName );

    /// Configures and update fields.
    void configureAndUpdateFields( int                                 widgetStartIndex,
                                   QBoxLayout*                         layout,
                                   const std::vector<caf::PdmUiItem*>& topLevelUiItems,
                                   const QString&                      uiConfigName );

    /// Creates group box with content.
    QMinimizePanel* createGroupBoxWithContent( caf::PdmUiGroup* group, const QString& uiConfigName );
signals:
    /// Performs the signal close button pressed command operation.
    void signalCloseButtonPressed();

private:
    QPointer<QVBoxLayout> m_layout;
    QPointer<QSplitter>   m_firstColumnSplitter;

    QPointer<QMinimizePanel> m_curvesPanel;

    QPointer<QHBoxLayout> m_firstRowLayout;
    QPointer<QHBoxLayout> m_secondRowLayout;
    QPointer<QVBoxLayout> m_lowerLeftLayout;
    QPointer<QVBoxLayout> m_lowerRightLayout;

    QPointer<QHBoxLayout> m_bottomFieldLayout;

    QPointer<caf::PdmUiTreeView> m_curveTreeView;

    QWidget* m_parentWidget;

    std::unique_ptr<RicSummaryPlotEditorUi> m_summaryCurveCreator;
};
