/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2021-     Equinor ASA
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
/// @brief Declares create ensemble surface command support.

#pragma once

#include "cafPdmField.h"
#include "cafPdmObject.h"

#include <QString>
#include <QStringList>

class RigEclipseCaseData;

//==================================================================================================
/// @brief UI model for configuring create ensemble surface.
//==================================================================================================
class RicCreateEnsembleSurfaceUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Constructs the command object.
    RicCreateEnsembleSurfaceUi();
    /// Destroys the command object.
    ~RicCreateEnsembleSurfaceUi() override;
    /// @return The tab names.
    const QStringList& tabNames() const;

    /// Sets layers min max.
    void setLayersMinMax( int minLayer, int maxLayer );

    /// @return The layers.
    std::vector<int> layers() const;

    /// @return The auto create ensemble surfaces.
    bool autoCreateEnsembleSurfaces() const;

protected:
    /// Performs the define editor attribute command operation.
    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;
    /// Defines the field ordering used by the command UI.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;

    /// @return The selectable values for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;

    caf::PdmField<std::vector<int>> m_layers;
    caf::PdmField<bool>             m_autoCreateEnsembleSurfaces;
    caf::PdmField<int>              m_minLayer;
    caf::PdmField<int>              m_maxLayer;

    QStringList m_tabNames;
};
