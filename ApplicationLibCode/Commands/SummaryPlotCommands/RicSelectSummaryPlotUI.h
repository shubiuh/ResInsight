/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017     Statoil ASA
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
/// @brief Declares select summary plot command support.

#pragma once

#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPtrField.h"

class RimSummaryPlot;

//==================================================================================================
/// @brief UI model for configuring select summary plot.
//==================================================================================================
class RicSelectSummaryPlotUI : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Constructs the command object.
    RicSelectSummaryPlotUI();

    /// Sets default summary plot.
    void setDefaultSummaryPlot( RimSummaryPlot* summaryPlot );
    /// Sets suggested plot name.
    void setSuggestedPlotName( const QString& name );

    /// @return The selected summary plot.
    RimSummaryPlot* selectedSummaryPlot() const;
    /// @return Whether create new plot checked.
    bool            isCreateNewPlotChecked() const;
    /// @return The new plot name.
    QString         newPlotName() const;

    /// @return The selectable values for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;

protected:
    /// Defines the field ordering used by the command UI.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;

private:
    caf::PdmPtrField<RimSummaryPlot*> m_selectedSummaryPlot;
    caf::PdmField<bool>               m_createNewPlot;
    caf::PdmField<QString>            m_newSummaryPlotName;
};
