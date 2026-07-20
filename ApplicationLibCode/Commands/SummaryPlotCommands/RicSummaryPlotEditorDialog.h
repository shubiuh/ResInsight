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
/// @brief Declares summary plot editor command support.

#pragma once

#include <QDialog>

/// @brief Utilities for caf command workflows.
namespace caf
{
class PdmObject;
};

class RicSummaryPlotEditorWidgetCreator;
class RimSummaryPlot;
class RimSummaryMultiPlot;
class RimSummaryCase;

//==================================================================================================
/// @brief Dialog for configuring summary plot editor.
//==================================================================================================
class RicSummaryPlotEditorDialog : public QDialog
{
    Q_OBJECT
public:
    /// Constructs the command object.
    RicSummaryPlotEditorDialog( QWidget* parent );
    /// Destroys the command object.
    ~RicSummaryPlotEditorDialog() override;

    /// Updates from summary plot.
    void updateFromSummaryPlot( RimSummaryPlot* summaryPlot );
    /// Updates from summary multi plot.
    void updateFromSummaryMultiPlot( RimSummaryMultiPlot* summaryPlot );
    /// Updates from default cases.
    void updateFromDefaultCases( const std::vector<caf::PdmObject*> defaultSources );

private slots:
    /// Performs the slot dialog finished command operation.
    void slotDialogFinished();

private:
    RicSummaryPlotEditorWidgetCreator* m_curveCreatorSplitterUi;
};
