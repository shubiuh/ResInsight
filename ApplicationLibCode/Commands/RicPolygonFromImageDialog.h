/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2024-     Equinor ASA
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
/// @brief Declares polygon from image command support.

#pragma once

#include <QCheckBox>
#include <QDialog>
#include <QFileDialog>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QImage>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QSlider>
#include <QSpinBox>
#include <QVBoxLayout>

/// @brief Dialog for configuring polygon from image.
class RicPolygonFromImageDialog : public QDialog
{
    Q_OBJECT

public:
    /// Constructs the command object.
    RicPolygonFromImageDialog( QWidget* parent = nullptr );

    /// @return The processed image data.
    std::vector<std::vector<int>> processedImageData() const;
    /// Sets source image data.
    void                          setSourceImageData( std::vector<std::vector<int>> imageData );

    /// @return The area threshold.
    int areaThreshold() const;

public slots:
    /// Updates and show images.
    void updateAndShowImages();

private slots:
    /// Performs the perform dilation command operation.
    void performDilation();
    /// Performs the perform erosion command operation.
    void performErosion();
    /// Shows images.
    void showImages();

private:
    /// @return The kernel adjusted size.
    int  kernelAdjustedSize() const;
    /// Computes final.
    void computeFinal();
    /// Performs the resize and center command operation.
    void resizeAndCenterDialog( double scale );

    /// Performs the resize event command operation.
    void resizeEvent( QResizeEvent* event ) override;

private:
    QSpinBox*       kernelSpinBox;
    QSlider*        transparencySlider;
    QGraphicsView*  graphicsView;
    QGraphicsScene* graphicsScene;

    QCheckBox* showInput;
    QCheckBox* showDilated;
    QCheckBox* showEroded;
    QCheckBox* showFinal;

    QLineEdit* areaThresholdLineEdit;

    std::vector<std::vector<int>> sourceData, dilatedData, erodedData, processedData;
};
