/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2026-     Equinor ASA
//
//  ResInsight is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  ResInsight is distributed in the hope that it will be useful, but WITHOUT
//  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
//  FITNESS FOR A PARTICULAR PURPOSE.
//
//  See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
//  for more details.
//
/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Declares import grid and summary ensemble command support.

#pragma once

#include "Summary/RiaSummaryDefines.h"

#include <QDialog>
#include <QMap>
#include <QStandardItemModel>
#include <QString>
#include <QStringList>

class QCheckBox;
class QComboBox;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTreeView;

/// @brief Data used to configure or execute import grid and summary ensemble dialog result command workflows.
struct RicImportGridAndSummaryEnsembleDialogResult
{
    bool                             ok;
    QStringList                      gridFiles;
    QStringList                      summaryFiles;
    QString                          rootDir;
    QString                          pathFilter;
    QString                          filePattern;
    RiaDefines::EnsembleGroupingMode groupingMode;
    bool                             createGridEnsemble;
    bool                             createSummaryEnsemble;
};

//==================================================================================================
/// @brief Dialog for configuring import grid and summary ensemble.
//==================================================================================================
class RicImportGridAndSummaryEnsembleDialog : public QDialog
{
    Q_OBJECT

public:
    static RicImportGridAndSummaryEnsembleDialogResult
        runDialog( QWidget* parent, bool defaultGridChecked, bool defaultSummaryChecked, const QString& initialDir = {} );

private:
    /// Constructs the command object.
    explicit RicImportGridAndSummaryEnsembleDialog( QWidget* parent );

    /// @return The clean path filter.
    QString cleanPathFilter() const;
    /// @return The root dir with separator.
    QString rootDirWithSeparator() const;
    /// @return The path filter without root.
    QString pathFilterWithoutRoot() const;
    /// @return The file pattern.
    QString filePattern() const;
    /// Updates effective filter.
    void    updateEffectiveFilter();
    /// Sets ok button enabled.
    void    setOkButtonEnabled( bool enabled );

    /// @return The matching matching files.
    QStringList findMatchingFiles( const QStringList& extensions );

    /// Updates file list widget.
    void        updateFileListWidget();
    /// Removes or clears file list.
    void        clearFileList();
    /// @return The check for multiple filenames and uncheck outliers.
    QStringList checkForMultipleFilenamesAndUncheckOutliers();

    /// @return The ensemble grouping mode.
    RiaDefines::EnsembleGroupingMode ensembleGroupingMode() const;

private slots:
    /// Performs the slot path filter changed command operation.
    void slotPathFilterChanged( const QString& text );
    /// Performs the slot browse clicked command operation.
    void slotBrowseClicked();
    /// Performs the slot use realization star clicked command operation.
    void slotUseRealizationStarClicked();
    /// Performs the slot search clicked command operation.
    void slotSearchClicked();
    /// Performs the slot filter tree view clicked command operation.
    void slotFilterTreeViewClicked();
    /// Performs the slot ok clicked command operation.
    void slotOkClicked();
    /// Performs the slot cancel clicked command operation.
    void slotCancelClicked();
    /// Shows event.
    void showEvent( QShowEvent* event ) override;

private:
    /// @brief Data used to configure or execute realization files command workflows.
    struct RealizationFiles
    {
        QString gridFile;
        QString summaryFile;
    };

    QComboBox*   m_pathFilterField;
    QPushButton* m_browseButton;
    QLineEdit*   m_filePatternField;
    QCheckBox*   m_useRealizationStarCheckBox;
    QComboBox*   m_ensembleGroupingMode;
    QLabel*      m_effectiveFilterLabel;
    QPushButton* m_searchButton;

    QCheckBox* m_createGridEnsembleCheckBox;
    QCheckBox* m_createSummaryEnsembleCheckBox;

    QGroupBox*   m_outputGroup;
    QLineEdit*   m_treeFilterLineEdit;
    QPushButton* m_treeFilterButton;
    QTreeView*   m_fileTreeView;

    QDialogButtonBox* m_buttons;

    QStandardItemModel m_filePathModel;

    QMap<QString, RealizationFiles> m_foundRealizations;

    bool m_blockItemUpdates;
};
