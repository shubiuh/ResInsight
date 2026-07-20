/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2026-     Equinor ASA
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

#pragma once

#include "RimEclipseCase.h"

//==================================================================================================
//
// PDM object representing a VTK grid case.
//
// Reads VTK unstructured grid files (.vtk, .vtu) and presents them as
// Eclipse-style grid cases in the ResInsight project tree.
//
//==================================================================================================
class RimVtkCase : public RimEclipseCase
{
    CAF_PDM_HEADER_INIT;

public:
    RimVtkCase();
    ~RimVtkCase() override;

    bool openEclipseGridFile() override;

    QString locationOnDisc() const override;

protected:
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;
};
