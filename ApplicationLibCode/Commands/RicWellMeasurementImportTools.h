/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2020-     Equinor ASA
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
/// @brief Declares well measurement import command support.

#pragma once

#include <QStringList>

#include <vector>

class RimWellPathCollection;
class RimWellMeasurementFilePath;

//--------------------------------------------------------------------------------------------------
/// @brief Utility functions for well measurement import.
//--------------------------------------------------------------------------------------------------
class RicWellMeasurementImportTools
{
public:
    /// Imports well measurements from files.
    static void importWellMeasurementsFromFiles( const std::vector<RimWellMeasurementFilePath*>& filePaths );
    /// Removes or clears well measurements from files.
    static void removeWellMeasurementsFromFiles( const std::vector<RimWellMeasurementFilePath*>& filePaths );
    /// Removes or clears all empty measurement curves.
    static void deleteAllEmptyMeasurementCurves();

    /// Imports well measurements from files.
    static void importWellMeasurementsFromFiles( const QStringList& filePaths, RimWellPathCollection* wellPathCollection );

    /// @return The selected well path collection.
    static RimWellPathCollection* selectedWellPathCollection();

private:
    // Hidden to avoid instantiation
    RicWellMeasurementImportTools();
};
