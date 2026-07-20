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
/// @brief Eclipse file-name parsing and related-file discovery.

#pragma once

#include <QString>

#include <vector>

//--------------------------------------------------------------------------------------------------
/// @brief Utilities for parsing Eclipse file names and discovering related files.
///
/// Given a grid, data, or summary file name, RiaEclipseFileNameTools can locate companion files
/// such as the corresponding .GRID, .EGRID, .DATA, .SMSPEC, or .ESMRY in the same directory. The
/// lookup is based on the absolute directory and complete base name of the input; it does not scan
/// other directories or attempt fuzzy matching. Returned paths are empty when a candidate is absent.
//--------------------------------------------------------------------------------------------------
class RiaEclipseFileNameTools
{
public:
    /// File extensions understood by the companion-file lookup.
    enum class EclipseFileType
    {
        ECLIPSE_DATA,      ///< Eclipse input deck (.DATA).
        ECLIPSE_GRID,      ///< Legacy grid geometry (.GRID).
        ECLIPSE_EGRID,     ///< Unified grid geometry (.EGRID).
        ECLIPSE_INIT,      ///< Initialization properties (.INIT).
        ECLIPSE_UNRST,     ///< Unified restart results (.UNRST).
        ECLIPSE_SMSPEC,    ///< Summary specification (.SMSPEC).
        ECLIPSE_UNSMRY,    ///< Unified summary vectors (.UNSMRY).
        ECLIPSE_ESMRY,     ///< Enhanced summary vectors (.ESMRY).
        RESINSIGHT_PROJECT, ///< ResInsight project file (.rsp).
        UNKNOWN            ///< Unrecognized or unsupported extension.
    };

public:
    /// Builds a companion-file base path from @p fileName.
    /// @param fileName Any file whose directory and complete base name identify the case.
    explicit RiaEclipseFileNameTools( const QString& fileName );

    /// Finds summary entry points, preferring SMSPEC when both formats exist.
    /// @return Two entries in SMSPEC/ESMRY order; missing or suppressed candidates are empty strings.
    std::vector<QString> findSummaryFileCandidates();

    /// Finds the related grid, preferring EGRID over legacy GRID.
    /// @return Absolute grid path, or an empty string when neither candidate exists.
    QString              findRelatedGridFile();

    /// @return Absolute path to the related DATA deck, or an empty string when absent.
    QString              findRelatedDataFile();

private:
    /// Constructs and verifies the companion path for @p fileType.
    QString relatedFilePath( EclipseFileType fileType ) const;

    /// @return true when @p fileName has the registered extension for @p fileType, ignoring case.
    static bool hasMatchingSuffix( const QString& fileName, EclipseFileType fileType );

private:
    QString m_baseName; ///< Absolute path without the input file's complete extension.
};
