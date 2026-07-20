/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2020-     Equinor ASA
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
/// @brief Declares new correlation plot command support.

#pragma once

#include "cafCmdFeature.h"

#include <ctime>

class RimSummaryEnsemble;

/// @brief Supports ensemble plot params command workflows.
class EnsemblePlotParams
{
public:
    RimSummaryEnsemble* ensemble;
    QStringList         includedQuantityNames;
    QString             mainQuantityName;
    QString             ensembleParameter;
    std::time_t         timeStep;

    /// Constructs the command object.
    EnsemblePlotParams();
    /// Constructs the command object.
    EnsemblePlotParams( const EnsemblePlotParams& rhs ) = default;

    /// Constructs the command object.
    EnsemblePlotParams( RimSummaryEnsemble* ensemble,
                        const QStringList&  includedQuantityNames,
                        const QString&      mainQuantityName,
                        const std::time_t&  timeStep );
    /// Destroys the command object.
    ~EnsemblePlotParams() = default;
};

Q_DECLARE_METATYPE( EnsemblePlotParams );

//==================================================================================================
/// @brief Command feature for new correlation plot.
//==================================================================================================
class RicNewCorrelationPlotFeature : public caf::CmdFeature
{
    CAF_CMD_HEADER_INIT;

protected:
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;
};
