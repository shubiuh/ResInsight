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
/// @brief Declares resample command support.

#pragma once

#include "RiaDateTimeDefines.h"

#include "cafPdmPointer.h"

#include <QDialog>

class QLabel;
class QComboBox;
class QLineEdit;
class QTextEdit;
class QDialogButtonBox;
class QPushButton;
class QMainWindow;
class QListWidget;
class RicResampleDialogResult;
class DateTimePeriodInfo;

//==================================================================================================
/// @brief Dialog for configuring resample.
//==================================================================================================
class RicResampleDialog : public QDialog
{
    Q_OBJECT

public:
    /// Constructs the command object.
    RicResampleDialog( QWidget* parent );
    /// Destroys the command object.
    ~RicResampleDialog() override;

    /// @return The open.
    static RicResampleDialogResult openDialog( QWidget* parent = nullptr, const QString& caption = QString() );

private:
    /// Sets period options.
    void                       setPeriodOptions( const std::vector<RiaDefines::DateTimePeriod>& dateTimePeriods );
    /// @return The selected date time period.
    RiaDefines::DateTimePeriod selectedDateTimePeriod() const;

private slots:
    /// Performs the slot dialog ok clicked command operation.
    void slotDialogOkClicked();
    /// Performs the slot dialog cancel clicked command operation.
    void slotDialogCancelClicked();

private:
    QLabel*    m_label;
    QComboBox* m_timePeriodCombo;

    QDialogButtonBox* m_buttons;
};

//==================================================================================================
/// @brief Supports resample dialog result command workflows.
//==================================================================================================
class RicResampleDialogResult
{
public:
    RicResampleDialogResult( bool ok, RiaDefines::DateTimePeriod period )
        : ok( ok )
        , period( period )
    {
    }

    bool                       ok;
    RiaDefines::DateTimePeriod period;
};
