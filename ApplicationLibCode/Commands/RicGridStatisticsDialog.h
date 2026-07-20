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
/// @brief Declares grid statistics command support.

#pragma once

#include "cafPdmPointer.h"

#include <QDialog>

class QLabel;
class QTextEdit;
class QDialogButtonBox;
class QwtPlot;
class QwtPlotMarker;
class QMainWindow;
class QToolBar;
class RimEclipseView;
class RimGridView;
class RigHistogramData;

//==================================================================================================
/// @brief Dialog for configuring grid statistics.
//==================================================================================================
class RicGridStatisticsDialog : public QDialog
{
    Q_OBJECT

public:
    /// Constructs the command object.
    explicit RicGridStatisticsDialog( QWidget* parent );
    /// Destroys the command object.
    ~RicGridStatisticsDialog() override;

    /// Sets label.
    void   setLabel( const QString& labelText );
    /// Updates from rim view.
    void   updateFromRimView( RimGridView* rimView );
    /// @return The screen shot image.
    QImage screenShotImage();

private:
    /// Sets info text.
    void setInfoText( RimGridView* eclipseView );
    /// Sets histogram data.
    void setHistogramData( RimGridView* eclipseView );

private:
    /// Creates and connect toolbar actions.
    void                  createAndConnectToolbarActions();
    /// Removes or clears plot items.
    void                  deletePlotItems( QwtPlot* plot );
    /// Sets markers.
    static void           setMarkers( const RigHistogramData& histData, QwtPlot* plot );
    /// Creates vertical plot marker.
    static QwtPlotMarker* createVerticalPlotMarker( const QColor& color, double xValue );
    /// Performs the adjust text edit height to content command operation.
    void                  adjustTextEditHeightToContent();

private slots:
    /// Performs the slot dialog finished command operation.
    void slotDialogFinished();
    /// Performs the slot screen shot to clipboard command operation.
    void slotScreenShotToClipboard();
    /// Performs the slot screen shot to file command operation.
    void slotScreenShotToFile();

private:
    QToolBar*         m_toolBar;
    QWidget*          m_mainViewWidget;
    QLabel*           m_label;
    QTextEdit*        m_textEdit;
    QwtPlot*          m_historgramPlot;
    QwtPlot*          m_aggregatedPlot;
    QDialogButtonBox* m_buttons;

    caf::PdmPointer<RimGridView> m_currentRimView;
};
