/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2021 Equinor ASA
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
/// @brief Declares create multiple well path laterals command support.

#pragma once

#include "RimMultipleLocations.h"

#include "cafPdmChildArrayField.h"
#include "cafPdmChildField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmProxyValueField.h"
#include "cafPdmPtrField.h"

#include <QPointer>

class RimModeledWellPath;
class RimWellPath;

/// @brief Utilities for caf command workflows.
namespace caf
{
class PdmUiPropertyViewDialog;
}

//==================================================================================================
/// @brief UI model for configuring create multiple well path laterals.
//==================================================================================================
class RicCreateMultipleWellPathLateralsUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Constructs the command object.
    RicCreateMultipleWellPathLateralsUi();

    /// Sets top level well path.
    void setTopLevelWellPath( RimWellPath* wellPath );
    /// Sets default values.
    void setDefaultValues( double start, double end );

    /// @return The source lateral.
    RimModeledWellPath*   sourceLateral() const;
    /// @return The location config.
    RimMultipleLocations* locationConfig() const;

private:
    /// Defines the field ordering used by the command UI.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;

    /// @return The selectable values for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;

private:
    caf::PdmPtrField<RimModeledWellPath*> m_sourceLateral;
    caf::PdmPtrField<RimWellPath*>        m_topLevelWellPath;

    caf::PdmChildField<RimMultipleLocations*> m_locations;
};
