/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2019-     Equinor ASA
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
/// @brief Implements Eclipse file-name parsing and related-file discovery.

#include "RiaEclipseFileNameTools.h"

#include "cafAppEnum.h"

#include <QFileInfo>

namespace caf
{
template <>
void caf::AppEnum<RiaEclipseFileNameTools::EclipseFileType>::setUp()
{
    addItem( RiaEclipseFileNameTools::EclipseFileType::ECLIPSE_DATA, "DATA", "Data Deck" );
    addItem( RiaEclipseFileNameTools::EclipseFileType::ECLIPSE_GRID, "GRID", "Grid" );
    addItem( RiaEclipseFileNameTools::EclipseFileType::ECLIPSE_EGRID, "EGRID", "Grid" );
    addItem( RiaEclipseFileNameTools::EclipseFileType::ECLIPSE_INIT, "INIT", "Init file" );
    addItem( RiaEclipseFileNameTools::EclipseFileType::ECLIPSE_UNRST, "UNRST", "Unified Restart" );
    addItem( RiaEclipseFileNameTools::EclipseFileType::ECLIPSE_SMSPEC, "SMSPEC", "Summary Specification" );
    addItem( RiaEclipseFileNameTools::EclipseFileType::ECLIPSE_UNSMRY, "UNSMR", "Summary Vectors" );
    addItem( RiaEclipseFileNameTools::EclipseFileType::ECLIPSE_ESMRY, "ESMRY", "ESRMY Summary Vectors" );

    addItem( RiaEclipseFileNameTools::EclipseFileType::RESINSIGHT_PROJECT, "rsp", "ResInsight Project" );
}

} // End namespace caf

//--------------------------------------------------------------------------------------------------
/// Normalizes the lookup to an absolute directory plus the input file's complete base name.
//--------------------------------------------------------------------------------------------------
RiaEclipseFileNameTools::RiaEclipseFileNameTools( const QString& inputFilePath )
{
    QFileInfo fi( inputFilePath );

    m_baseName = fi.absolutePath() + "/" + fi.baseName();
}

//--------------------------------------------------------------------------------------------------
/// EGRID is preferred because it is the unified grid representation used by modern Eclipse cases.
//--------------------------------------------------------------------------------------------------
QString RiaEclipseFileNameTools::findRelatedGridFile()
{
    QString candidate = relatedFilePath( EclipseFileType::ECLIPSE_EGRID );
    if ( !candidate.isEmpty() )
    {
        return candidate;
    }

    return relatedFilePath( EclipseFileType::ECLIPSE_GRID );
}

//--------------------------------------------------------------------------------------------------
/// SMSPEC takes precedence because it is the canonical summary entry point when both files exist.
//--------------------------------------------------------------------------------------------------
std::vector<QString> RiaEclipseFileNameTools::findSummaryFileCandidates()
{
    auto smryCandidate  = relatedFilePath( EclipseFileType::ECLIPSE_SMSPEC );
    auto esmryCandidate = relatedFilePath( EclipseFileType::ECLIPSE_ESMRY );

    if ( !smryCandidate.isEmpty() && !esmryCandidate.isEmpty() )
    {
        // If both files exist, we prefer the SMSPEC file
        esmryCandidate = "";
    }

    return { smryCandidate, esmryCandidate };
}

//--------------------------------------------------------------------------------------------------
/// DATA discovery uses the same base-name convention as grid and summary discovery.
//--------------------------------------------------------------------------------------------------
QString RiaEclipseFileNameTools::findRelatedDataFile()
{
    return relatedFilePath( EclipseFileType::ECLIPSE_DATA );
}

//--------------------------------------------------------------------------------------------------
/// AppEnum provides the canonical extension text so lookup and UI registration cannot diverge.
//--------------------------------------------------------------------------------------------------
QString RiaEclipseFileNameTools::relatedFilePath( EclipseFileType fileType ) const
{
    const QString extension        = caf::AppEnum<EclipseFileType>::text( fileType );
    const QString completeFilePath = m_baseName + "." + extension;

    QFileInfo fi( completeFilePath );
    if ( fi.exists() )
    {
        return fi.absoluteFilePath();
    }

    return "";
}

//--------------------------------------------------------------------------------------------------
/// completeSuffix() handles names containing multiple dots while comparison remains case-insensitive.
//--------------------------------------------------------------------------------------------------
bool RiaEclipseFileNameTools::hasMatchingSuffix( const QString& fileName, EclipseFileType fileType )
{
    QFileInfo fi( fileName );

    QString suffix = fi.completeSuffix();

    return suffix.compare( caf::AppEnum<EclipseFileType>::text( fileType ), Qt::CaseInsensitive ) == 0;
}
