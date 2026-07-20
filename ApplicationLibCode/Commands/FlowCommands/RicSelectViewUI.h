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
/// @brief Declares select view command support.

#pragma once

#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPtrField.h"

class RimEclipseView;
class RimEclipseResultCase;

//==================================================================================================
/// @brief UI model for configuring select view.
//==================================================================================================
class RicSelectViewUI : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Constructs the command object.
    RicSelectViewUI();

    /// Sets view.
    void setView( RimEclipseView* currentView );
    /// Sets case.
    void setCase( RimEclipseResultCase* currentCase );
    /// Sets new view name.
    void setNewViewName( const QString& name );

    /// @return The selected view.
    RimEclipseView* selectedView() const;
    /// Creates new view.
    bool            createNewView() const;
    /// @return The new view name.
    QString         newViewName() const;

    /// @return The selectable values for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;

protected:
    /// Defines the field ordering used by the command UI.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;

private:
    caf::PdmPtrField<RimEclipseView*> m_selectedView;
    caf::PdmField<bool>               m_createNewView;
    caf::PdmField<QString>            m_newViewName;

    RimEclipseView*       m_currentView;
    RimEclipseResultCase* m_currentCase;
};
