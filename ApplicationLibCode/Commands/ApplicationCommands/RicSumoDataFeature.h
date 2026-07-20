/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2024     Equinor ASA
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
/// @brief Declares sumo data command support.

#pragma once

#include "cafCmdFeature.h"

#include "Cloud/RiaSumoConnector.h"

#include <QDialog>
#include <QLabel>
#include <QPointer>
#include <QPushButton>
#include <QVBoxLayout>

/// @brief Dialog for configuring simple.
class SimpleDialog : public QDialog
{
    Q_OBJECT

public:
    /// Constructs the command object.
    SimpleDialog( QWidget* parent = nullptr );
    /// Destroys the command object.
    ~SimpleDialog() override;

    /// Creates connection.
    void createConnection();

private:
    /// Performs the on ok clicked command operation.
    void onOkClicked();
    /// Performs the on cancel clicked command operation.
    void onCancelClicked();
    /// Performs the on token ready command operation.
    void onTokenReady( const QString& token );
    /// Performs the on auth clicked command operation.
    void onAuthClicked();
    /// Performs the on assets clicked command operation.
    void onAssetsClicked();
    /// Performs the on cases clicked command operation.
    void onCasesClicked();
    /// Performs the on vector names clicked command operation.
    void onVectorNamesClicked();
    /// Performs the on find blob ID clicked command operation.
    void onFindBlobIdClicked();
    /// Performs the on parquet clicked command operation.
    void onParquetClicked();
    /// Performs the on show content parquet clicked command operation.
    void onShowContentParquetClicked();
    /// Performs the on realizations clicked command operation.
    void onRealizationsClicked();

    /// @return Whether token valid.
    bool isTokenValid();

private:
    QLabel*      label;
    QPushButton* okButton;
    QPushButton* cancelButton;
    QPushButton* authButton;
    QPushButton* assetsButton;
    QPushButton* casesButton;
    QPushButton* vectorNamesButton;
    QPushButton* blobIdButton;
    QPushButton* parquetDownloadButton;
    QPushButton* showContentParquetButton;
    QPushButton* realizationIdsButton;

    QPointer<RiaSumoConnector> m_sumoConnector;
};

//==================================================================================================
/// @brief Command feature for sumo data.
//==================================================================================================
class RicSumoDataFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

protected:
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

private:
    SimpleDialog* m_dialog = nullptr;
};
