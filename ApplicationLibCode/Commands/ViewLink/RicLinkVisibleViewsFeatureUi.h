/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2015-     Statoil ASA
//  Copyright (C) 2015-     Ceetron Solutions AS
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
/// @brief Declares link visible views feature command support.

#pragma once

#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPtrField.h"

#include <vector>

class Rim3dView;

//==================================================================================================
/// @brief UI model for configuring link visible views feature.
//==================================================================================================
class RicLinkVisibleViewsFeatureUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Constructs the command object.
    RicLinkVisibleViewsFeatureUi();

    /// Sets views.
    void       setViews( const std::vector<Rim3dView*>& allViews );
    /// @return The master view.
    Rim3dView* masterView();

protected:
    /// @return The selectable values for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;

private:
    caf::PdmPtrField<Rim3dView*> m_masterView;

    std::vector<Rim3dView*> m_allViews;
};
