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
/// @brief Declares recursive file search command support.

#pragma once

#include "Rim3dOverlayInfoConfig.h"

#include "cafPdmPointer.h"

#include "RiaEnsembleNameTools.h"
#include "Summary/RiaSummaryDefines.h"

#include <QDialog>
#include <QStandardItemModel>

class QLabel;
class QLineEdit;
class QTextEdit;
class QDialogButtonBox;
class QPushButton;
class QMainWindow;
class QGroupBox;
class QComboBox;
class QCheckBox;
class QTreeView;

struct RicRecursiveFileSearchDialogResult;

//==================================================================================================
/// @brief Dialog for configuring recursive file search.
//==================================================================================================
class RicRecursiveFileSearchDialog : public QDialog
{
    Q_OBJECT
public:
    enum Status
    {
        SEARCHING_FOR_DIRS,
        SEARCHING_FOR_FILES,
        NO_FILES_FOUND
    };

    // ESMRY is available as option in the dialog to be able to search for ESMRY files. Note that the use of ESMRY files in this dialog use
    // RiaDefines::FileType::SMSPEC, as ESMRY files can be created from SMSPEC files.
    enum class FileType
    {
        GRDECL,
        EGRID,
        GRID,
        SMSPEC,
        ESMRY,
        STIMPLAN_FRACTURE,
        LAS,
        SURFACE,
        STIMPLAN_SUMMARY,
        REVEAL_SUMMARY
    };

    /// @return The run recursive search.
    static RicRecursiveFileSearchDialogResult runRecursiveSearchDialog( QWidget*                     parent,
                                                                        const QString&               caption,
                                                                        const QString&               dir,
                                                                        const QString&               pathFilter,
                                                                        const QString&               fileNameFilter,
                                                                        const std::vector<FileType>& fileTypes );

    /// @return The file name for type.
    static QString     fileNameForType( FileType fileType );
    /// @return The file extension for type.
    static QStringList fileExtensionForType( FileType fileType );

    /// @return The map summary file type.
    static RiaDefines::FileType mapSummaryFileType( RicRecursiveFileSearchDialog::FileType fileType );

    /// @return The first level items.
    static QList<QStandardItem*> firstLevelItems( QStandardItem* rootItem );
    /// Sets checked state child items.
    static void                  setCheckedStateChildItems( QStandardItem* parentItem, Qt::CheckState checkState );
    /// Finds items matching.
    static void findItemsMatching( QStandardItem* parentItem, const QString& substring, QList<QStandardItem*>& matchingItems );
    /// Performs the populate combo box history from registry command operation.
    static void populateComboBoxHistoryFromRegistry( QComboBox* comboBox, const QString& registryKey );

private:
    /// Constructs the command object.
    RicRecursiveFileSearchDialog( QWidget* parent, const std::vector<FileType>& fileTypes );
    /// Destroys the command object.
    ~RicRecursiveFileSearchDialog() override;

    /// @return The clean text from path filter field.
    QString  cleanTextFromPathFilterField() const;
    /// @return The root dir with end separator.
    QString  rootDirWithEndSeparator() const;
    /// @return The path filter without start separator.
    QString  pathFilterWithoutStartSeparator() const;
    /// @return The file name filter.
    QString  fileNameFilter() const;
    /// @return The file type.
    FileType fileType() const;

    /// @return The file extensions.
    QStringList fileExtensions() const;
    /// @return The extension from file name filter.
    QString     extensionFromFileNameFilter() const;

    /// @return The ensemble grouping mode.
    RiaDefines::EnsembleGroupingMode ensembleGroupingMode() const;

    /// Sets ok button enabled.
    void setOkButtonEnabled( bool enabled );
    /// Performs the warning if invalid characters command operation.
    void warningIfInvalidCharacters();
    /// Updates effective filter.
    void updateEffectiveFilter();
    /// Updates status.
    void updateStatus( Status status, const QString& extraText = "" );

    /// Updates file list widget.
    void updateFileListWidget();
    /// Removes or clears file list.
    void clearFileList();
    /// Adds to tree view.
    void addToTreeView( const QString& ensembleName, const QStringList& fileNames );

    // File search methods

    QStringList    findMatchingFiles();
    /// Creates file name filter list.
    QStringList    createFileNameFilterList();
    /// @return The replace with realization star.
    static QString replaceWithRealizationStar( const QString& text );

    /// @return The file type to extension strings.
    static QStringList fileTypeToExtensionStrings( const std::vector<RicRecursiveFileSearchDialog::FileType>& fileTypes );

private slots:
    /// Performs the slot path filter changed command operation.
    void slotPathFilterChanged( const QString& text );
    /// Performs the slot file filter changed command operation.
    void slotFileFilterChanged( const QString& text );
    /// Performs the slot file extension changed command operation.
    void slotFileExtensionChanged( const QString& text );
    /// Performs the slot file type changed command operation.
    void slotFileTypeChanged( int );
    /// Performs the slot browse button clicked command operation.
    void slotBrowseButtonClicked();
    /// Performs the slot use realization star clicked command operation.
    void slotUseRealizationStarClicked();
    /// Performs the slot find or cancel button clicked command operation.
    void slotFindOrCancelButtonClicked();
    /// Performs the slot filter tree view clicked command operation.
    void slotFilterTreeViewClicked();

    /// Performs the slot file list custom menu requested command operation.
    void slotFileListCustomMenuRequested( const QPoint& point );
    /// Performs the slot copy file item text command operation.
    void slotCopyFileItemText();
    /// Performs the slot toggle file list items command operation.
    void slotToggleFileListItems();
    /// Performs the slot turn off file list items command operation.
    void slotTurnOffFileListItems();
    /// Performs the slot turn on file list items command operation.
    void slotTurnOnFileListItems();

    /// Performs the slot dialog ok clicked command operation.
    void slotDialogOkClicked();
    /// Performs the slot dialog cancel clicked command operation.
    void slotDialogCancelClicked();

    /// Shows event.
    void showEvent( QShowEvent* event ) override;

private:
    QLabel*      m_pathFilterLabel;
    QComboBox*   m_pathFilterField;
    QPushButton* m_browseButton;
    QCheckBox*   m_useRealizationStarCheckBox;

    QLabel*    m_fileFilterLabel;
    QComboBox* m_fileFilterField;

    QLabel*    m_fileTypeLabel;
    QComboBox* m_fileTypeField;

    QLabel*    m_fileExtensionLabel;
    QLineEdit* m_fileExtensionField;

    QLabel*      m_effectiveFilterLabel;
    QLabel*      m_effectiveFilterContentLabel;
    QPushButton* m_findOrCancelButton;

    QComboBox* m_ensembleGroupingMode;

    QGroupBox* m_outputGroup;

    QLabel*      m_treeViewFilterLabel;
    QLineEdit*   m_treeViewFilterLineEdit;
    QPushButton* m_treeViewFilterButton;

    QTreeView*         m_fileTreeView;
    QStandardItemModel m_filePathModel;

    QDialogButtonBox* m_buttons;

    QStringList           m_foundFiles;
    std::vector<FileType> m_incomingFileTypes;
    QStringList           m_fileExtensions;
    FileType              m_fileType;

    bool m_isCancelPressed;
    bool m_blockUpdateOfOtherItems;
};

//==================================================================================================
/// @brief Data used to configure or execute recursive file search dialog result command workflows.
//==================================================================================================
struct RicRecursiveFileSearchDialogResult
{
    bool                                   ok;
    QStringList                            files;
    QString                                rootDir;
    QString                                pathFilter;
    QString                                fileNameFilter;
    RicRecursiveFileSearchDialog::FileType fileType;

    RiaDefines::EnsembleGroupingMode groupingMode;
};
