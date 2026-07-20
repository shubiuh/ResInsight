/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2017-     Statoil ASA
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
/// @brief Declares export Eclipse sector model command support.

#pragma once

#include "RiaModelExportDefines.h"

#include "cafAppEnum.h"
#include "cafPdmChildArrayField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafVecIjk.h"

#include "cvfVector3.h"

#include <QString>
#include <QStringList>

#include <set>
#include <vector>

class RigEclipseCaseData;
class RigSimWellData;
class RimEclipseView;

//==================================================================================================
/// @brief UI model for configuring export Eclipse sector model.
//==================================================================================================
class RicExportEclipseSectorModelUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

    enum ResultExportOptions
    {
        EXPORT_NO_RESULTS,
        EXPORT_TO_GRID_FILE,
        EXPORT_TO_SINGLE_SEPARATE_FILE,
        EXPORT_TO_SEPARATE_FILE_PER_RESULT
    };
    using ResultExportOptionsEnum = caf::AppEnum<ResultExportOptions>;

    using GridBoxSelectionEnum  = caf::AppEnum<RiaModelExportDefines::GridBoxSelection>;
    using BoundaryConditionEnum = caf::AppEnum<RiaModelExportDefines::BoundaryCondition>;

public:
    /// Constructs the command object.
    RicExportEclipseSectorModelUi();
    /// Destroys the command object.
    ~RicExportEclipseSectorModelUi() override;
    /// @return The tab names.
    const QStringList& tabNames() const;

    /// Sets case data.
    void setCaseData( RigEclipseCaseData* caseData    = nullptr,
                      RimEclipseView*     eclipseView = nullptr,
                      const caf::VecIjk0& visibleMin  = caf::VecIjk0::ZERO,
                      const caf::VecIjk0& visibleMax  = caf::VecIjk0::ZERO );

    /// @return The min.
    caf::VecIjk0 min() const;
    /// @return The max.
    caf::VecIjk0 max() const;
    /// Sets min.
    void         setMin( const caf::VecIjk0& min );
    /// Sets max.
    void         setMax( const caf::VecIjk0& max );
    /// Applies boundary defaults.
    void         applyBoundaryDefaults();
    /// Removes or clears invalid keywords.
    void         removeInvalidKeywords();
    /// @return The refinement.
    cvf::Vec3st  refinement() const;

    /// Exports faults filename.
    QString exportFaultsFilename() const;
    /// Exports grid filename.
    QString exportGridFilename() const;
    /// Exports parameters filename.
    QString exportParametersFilename() const;
    /// Writes echo keywords.
    bool    writeEchoKeywords() const;

    caf::PdmField<bool> exportGrid;
    caf::PdmField<bool> exportInLocalCoordinates;
    caf::PdmField<bool> makeInvisibleCellsInactive;

    caf::PdmField<ResultExportOptionsEnum> exportFaults;

    caf::PdmField<ResultExportOptionsEnum> exportParameters;

    caf::PdmField<std::vector<QString>> selectedKeywords;

    caf::PdmField<GridBoxSelectionEnum> exportGridBox;

    caf::PdmField<int> m_visibleWellsPadding;

    caf::PdmField<int> refinementCountI;
    caf::PdmField<int> refinementCountJ;
    caf::PdmField<int> refinementCountK;

protected:
    caf::PdmField<int> minI;
    caf::PdmField<int> maxI;
    caf::PdmField<int> minJ;
    caf::PdmField<int> maxJ;
    caf::PdmField<int> minK;
    caf::PdmField<int> maxK;

    /// Performs the define editor attribute command operation.
    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;
    /// Defines the field ordering used by the command UI.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;
    /// Responds to a value changed through the command UI.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;
    /// @return The selectable values for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;

    /// @return The main keywords.
    static std::set<QString> mainKeywords();
    /// @return The default folder.
    QString                  defaultFolder() const;
    /// @return The default grid file name.
    QString                  defaultGridFileName() const;
    /// @return The default results file name.
    QString                  defaultResultsFileName() const;
    /// @return The default faults file name.
    QString                  defaultFaultsFileName() const;

private:
    caf::PdmField<caf::FilePath> m_exportFolder;
    caf::PdmField<QString>       m_exportFaultsFilename;
    caf::PdmField<QString>       m_exportParametersFilename;
    caf::PdmField<QString>       m_exportGridFilename;
    caf::PdmField<bool>          m_writeEchoInGrdeclFiles;

    RigEclipseCaseData* m_caseData;
    RimEclipseView*     m_eclipseView;
    caf::VecIjk0        m_visibleMin;
    caf::VecIjk0        m_visibleMax;
    QStringList         m_tabNames;
};
