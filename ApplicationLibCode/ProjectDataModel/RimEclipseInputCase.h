/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Eclipse case loaded from an input deck and its included property files.
//
//  Copyright (C) 2011-2012 Statoil ASA, Ceetron AS
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

#include "cafPdmChildField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmProxyValueField.h"

#include "cvfObject.h"

class RifReaderInterface;
class RimEclipseInputProperty;
class RimEclipseInputPropertyCollection;

/// @brief Loads grid geometry and static properties from Eclipse DATA/GRDECL input files.
///
/// Unlike RimEclipseResultCase, an input case has no simulator restart sequence. The additional-files
/// proxy exposes discovered INCLUDE files without persisting a duplicate list; the primary deck and
/// reader determine them when the case is opened.
class RimEclipseInputCase : public RimEclipseCase
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes the read-only additional-file proxy.
    RimEclipseInputCase();
    ~RimEclipseInputCase() override;

    /// Opens @p fileNames as one Eclipse input file set and installs the resulting grid data.
    bool openDataFileSet( const QStringList& fileNames );
    /// Imports additional ASCII properties into the already loaded input grid.
    bool importAsciiInputProperties( const QStringList& fileNames ) override;

    /// Opens the configured primary deck, or creates a requested built-in mock model.
    bool openEclipseGridFile() override;

    /// @return Directory containing the configured input deck.
    QString locationOnDisc() const override;

    /// Relocates referenced additional files beneath @p newFolder where possible.
    void updateAdditionalFileFolder( const QString& newFolder );

protected:
    /// Presents the primary input deck and discovered additional files.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;

private:
    /// Creates a reader populated with the named built-in synthetic grid.
    cvf::ref<RifReaderInterface> createMockModel( QString modelName );

private:
    /// Read-only runtime view of additional files discovered by the input reader.
    caf::PdmProxyValueField<std::vector<QString>> m_additionalFiles;
};
