/////////////////////////////////////////////////////////////////////////////////

/// @file
/// Declares the common interface and import policy shared by reservoir grid readers.
//
//  Copyright (C) 2011-     Statoil ASA
//  Copyright (C) 2013-     Ceetron Solutions AS
//  Copyright (C) 2011-2012 Ceetron AS
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

#include "RiaDefines.h"
#include "RiaPorosityModel.h"

#include "RifReaderSettings.h"

#include "cvfCollection.h"
#include "cvfObject.h"

#include "cafPdmPointer.h"

#include <QString>
#include <QStringList>

#include <memory>
#include <set>
#include <vector>

class RigEclipseCaseData;
class RigFault;

//==================================================================================================
//
/// Base class for readers that populate a runtime Eclipse reservoir case.
///
/// Besides the format-specific grid and result API, this class centralizes reader
/// preferences, time-step filtering, and deck-based fault discovery. Concrete
/// readers therefore implement storage access while inheriting consistent import
/// behavior from the application preferences.
//==================================================================================================
class RifReaderInterface : public cvf::Object
{
public:
    /// Initializes settings from the current grid-reader preferences.
    RifReaderInterface();
    ~RifReaderInterface() override {}

    /// @name Effective import settings
    /// @{
    bool          isFaultImportEnabled() const;
    bool          isImportOfCompleteMswDataEnabled() const;
    bool          isNNCsEnabled() const;
    bool          includeInactiveCellsInFaultGeometry() const;
    bool          loadWellDataEnabled() const;
    const QString faultIncludeFileAbsolutePathPrefix() const;
    bool          onlyLoadActiveCells() const;
    bool          invalidateLongThinCells() const;
    /// @}

    /// Replaces the reader's complete import policy.
    void setReaderSettings( RifReaderSettings readerSettings );

    /// Opens @p fileName and populates grid metadata in @p eclipseCase.
    virtual bool open( const QString& fileName, RigEclipseCaseData* eclipseCase ) = 0;

    /// Loads a static result into @p values for the requested porosity model.
    virtual bool staticResult( const QString& result, RiaDefines::PorosityModelType matrixOrFracture, std::vector<double>* values ) = 0;
    /// Loads one dynamic result time step into @p values.
    virtual bool dynamicResult( const QString&                result,
                                RiaDefines::PorosityModelType matrixOrFracture,
                                size_t                        stepIndex,
                                std::vector<double>*          values )                                                                       = 0;

    /// Stores explicit fault include files, bypassing automatic deck discovery.
    void setFilenamesWithFaults( const std::vector<QString>& filenames ) { m_filenamesWithFaults = filenames; }
    /// @return Fault include files discovered or explicitly assigned for this case.
    std::vector<QString> filenamesWithFaults() { return m_filenamesWithFaults; }

    /// Selects file time-step indices and defines the exposed-to-file index mapping.
    void setTimeStepFilter( const std::vector<size_t>& fileTimeStepIndices );

    /// @return Fluid phases advertised by the concrete reader, if known.
    virtual std::set<RiaDefines::PhaseType> availablePhases() const;

    /// Notifies readers whose internal layout depends on the number of grids.
    virtual void updateFromGridCount( size_t gridCount ) {};

protected:
    /// Tests a file time-step index against the configured filter.
    bool              isTimeStepIncludedByFilter( size_t timeStepIndex ) const;
    /// Maps an exposed time-step index back to its original file index.
    size_t            timeStepIndexOnFile( size_t timeStepIndex ) const;
    /// Imports faults from explicit includes, the DATA grid section, or a PFLOTRAN input file.
    void              importFaults( const QStringList& fileSet, cvf::Collection<RigFault>* faults );
    /// @return A copy of the effective settings for use by subclasses.
    RifReaderSettings readerSettings() const;

private:
    std::vector<QString> m_filenamesWithFaults; ///< Explicit or discovered files containing fault definitions.

    std::vector<size_t> m_fileTimeStepIndices; ///< Exposed-index to file-index mapping; empty means identity.

    RifReaderSettings m_readerSettings; ///< Effective import policy for this reader instance.
};
