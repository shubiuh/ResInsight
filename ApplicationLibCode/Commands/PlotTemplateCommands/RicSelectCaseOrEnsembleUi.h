/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2022     Equinor ASA
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
/// @brief Declares select case or ensemble command support.

#pragma once

#include "cafPdmObject.h"
#include "cafPdmPtrField.h"

#include <QList>
#include <QString>
#include <vector>

class RimSummaryCase;
class RimSummaryEnsemble;

//==================================================================================================
/// @brief UI model for configuring select case or ensemble.
//==================================================================================================
class RicSelectCaseOrEnsembleUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Constructs the command object.
    RicSelectCaseOrEnsembleUi();

    /// Sets ensemble selection mode.
    void setEnsembleSelectionMode( bool selectEnsemble );

    /// @return The selected summary case.
    RimSummaryCase*     selectedSummaryCase() const;
    /// @return The selected ensemble.
    RimSummaryEnsemble* selectedEnsemble() const;

protected:
    /// Defines the field ordering used by the command UI.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;

    /// @return The selectable values for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;

private:
    caf::PdmPtrField<RimSummaryCase*>     m_selectedSummaryCase;
    caf::PdmPtrField<RimSummaryEnsemble*> m_selectedEnsemble;

    bool m_useEnsembleMode;
};
