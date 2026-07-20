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

#include "RimVtkCase.h"

#include "RiaDefines.h"
#include "RiaLogging.h"
#include "RiaPreferencesGrid.h"

#include "RifReaderVtkGrid.h"

#include "RigActiveCellInfo.h"
#include "RigCaseCellResultsData.h"
#include "RigEclipseCaseData.h"
#include "RigMainGrid.h"

#include "RimEclipseView.h"
#include "RimReservoirCellResultsStorage.h"

#include "cafPdmObjectScriptingCapability.h"
#include "cafPdmUiOrdering.h"

#include <QFileInfo>

CAF_PDM_SOURCE_INIT( RimVtkCase, "VtkCase", "RimVtkCase" );

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RimVtkCase::RimVtkCase()
    : RimEclipseCase()
{
    CAF_PDM_InitScriptableObject( "VTK Grid Case", ":/EclipseInput48x48.png" );
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
RimVtkCase::~RimVtkCase()
{
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RimVtkCase::openEclipseGridFile()
{
    if ( eclipseCaseData() )
    {
        // Early exit if reservoir data is already created
        return true;
    }

    setReservoirData( new RigEclipseCaseData( this ) );

    QString fileName = gridFileName();

    if ( eclipseCaseData()->mainGrid()->cellCount() == 0 )
    {
        QString errorMessages;
        if ( RifReaderVtkGrid::openFile( fileName, eclipseCaseData(), &errorMessages ) )
        {
            QFileInfo gridFileInfo( fileName );
            QString   caseName = gridFileInfo.completeBaseName();
            setCaseUserDescription( caseName );
            eclipseCaseData()->mainGrid()->setFlipAxis( m_flipXAxis, m_flipYAxis );
            computeCachedData();
        }
        else
        {
            RiaLogging::error( errorMessages.toStdString() );
            return false;
        }
    }

    results( RiaDefines::PorosityModelType::MATRIX_MODEL )->createPlaceholderResultEntries();

    if ( RiaPreferencesGrid::current()->autoComputeDepthRelatedProperties() )
    {
        eclipseCaseData()->computeDepthRelatedResults();
    }

    results( RiaDefines::PorosityModelType::MATRIX_MODEL )->computeCellVolumes();

    return true;
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
QString RimVtkCase::locationOnDisc() const
{
    return gridFileName();
}

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
void RimVtkCase::defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering )
{
    RimEclipseCase::defineUiOrdering( uiConfigName, uiOrdering );
}
