/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2025   Equinor ASA
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
/// @brief Declares export sector model command support.

#pragma once

#include "cafFilePath.h"
#include "cafPdmChildField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPtrArrayField.h"
#include "cafPdmPtrField.h"
#include "cafVecIjk.h"

#include "RiaModelExportDefines.h"
#include "RigRefinement.h"

#include <QString>
#include <QStringList>

#include <map>
#include <memory>

class RigModelPaddingSettings;
class RimKeywordBcprop;
class RimEclipseCase;
class RimEclipseView;
class RicRefinementSettings;
class RimRefinementRegion;

//==================================================================================================
/// @brief UI model for configuring export sector model.
//==================================================================================================
class RicExportSectorModelUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

    using GridBoxSelectionEnum  = caf::AppEnum<RiaModelExportDefines::GridBoxSelection>;
    using BoundaryConditionEnum = caf::AppEnum<RiaModelExportDefines::BoundaryCondition>;

public:
    /// Constructs the command object.
    RicExportSectorModelUi();
    /// Destroys the command object.
    ~RicExportSectorModelUi() override;

    /// @return The page names.
    const QStringList& pageNames() const;
    /// @return The page sub titles.
    const QStringList& pageSubTitles() const;

    /// Sets Eclipse view.
    void setEclipseView( RimEclipseView* view );

    // data access
    caf::VecIjk0 min() const;
    /// @return The max.
    caf::VecIjk0 max() const;
    /// Sets min.
    void         setMin( const caf::VecIjk0& min );
    /// Sets max.
    void         setMax( const caf::VecIjk0& max );

    /// @return The refinement settings.
    RicRefinementSettings* refinementSettings() const;

    /// @return The effective refinement.
    std::unique_ptr<RigRefinement> effectiveRefinement() const;
    /// @return Whether non uniform refinement.
    bool                           hasNonUniformRefinement() const;

    /// @return The keywords to remove.
    std::vector<QString> keywordsToRemove() const;

    /// @return The bcprop keywords.
    std::vector<RimKeywordBcprop*>           bcpropKeywords() const;
    /// @return The boundary condition.
    RiaModelExportDefines::BoundaryCondition boundaryCondition() const;
    /// @return The porv multiplier.
    double                                   porvMultiplier() const;
    /// Exports deck filename.
    QString                                  exportDeckFilename() const;
    /// @return The input deck filename.
    QString                                  inputDeckFilename() const;

    /// @return The grid box selection.
    RiaModelExportDefines::GridBoxSelection gridBoxSelection() const;
    /// @return The well padding.
    int                                     wellPadding() const;

    /// @return Whether create simulation job.
    bool    shouldCreateSimulationJob() const;
    /// @return The start simulation job after export.
    bool    startSimulationJobAfterExport() const;
    /// @return The new simulation job folder.
    QString newSimulationJobFolder() const;
    /// @return The new simulation job name.
    QString newSimulationJobName() const;

    // Model padding settings accessor
    RigModelPaddingSettings paddingSettings() const;

protected:
    /// Defines the field ordering used by the command UI.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;
    /// Performs the define editor attribute command operation.
    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;
    /// Responds to a value changed through the command UI.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;
    /// @return The selectable values for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;
    /// @return The validate.
    std::map<QString, QString>    validate( const QString& configName ) const override;

    /// Sets default keywords to remove.
    void setDefaultKeywordsToRemove();

private:
    enum WizardPageEnum : unsigned int
    {
        ExportSettings     = 0,
        GridBoxSelection   = 1,
        GridRefinement     = 2,
        BoundaryConditions = 3,
        KeywordAdjustments = 4,
        ModelPadding       = 5,
        SimulationJob      = 6,
        TotalPages         = 7
    };

    /// Applies boundary defaults.
    void           applyBoundaryDefaults();
    /// @return The default folder.
    static QString defaultFolder();

private:
    caf::PdmField<caf::FilePath> m_exportFolder;
    caf::PdmField<QString>       m_exportDeckName;
    caf::PdmField<caf::FilePath> m_inputDeckName;

    caf::PdmField<int>                  m_visibleWellsPadding;
    caf::PdmField<GridBoxSelectionEnum> m_gridBoxSelection;

    caf::PdmField<int> m_minI;
    caf::PdmField<int> m_maxI;
    caf::PdmField<int> m_minJ;
    caf::PdmField<int> m_maxJ;
    caf::PdmField<int> m_minK;
    caf::PdmField<int> m_maxK;

    caf::PdmChildField<RicRefinementSettings*>  m_refinementSettings;
    caf::PdmPtrArrayField<RimRefinementRegion*> m_selectedRegions;

    caf::PdmField<BoundaryConditionEnum>       m_boundaryCondition;
    caf::PdmChildArrayField<RimKeywordBcprop*> m_bcpropKeywords;
    caf::PdmField<double>                      m_porvMultiplier;

    caf::PdmField<std::vector<QString>> m_keywordsToRemove;

    // Model padding fields
    caf::PdmField<bool>   m_enablePadding;
    caf::PdmField<int>    m_paddingNzUpper;
    caf::PdmField<double> m_paddingTopUpper;
    caf::PdmField<double> m_paddingUpperPorosity;
    caf::PdmField<int>    m_paddingUpperEquilnum;
    caf::PdmField<int>    m_paddingNzLower;
    caf::PdmField<double> m_paddingBottomLower;
    caf::PdmField<double> m_paddingMinThickness;
    caf::PdmField<bool>   m_paddingFillGaps;
    caf::PdmField<bool>   m_paddingMonotonicZcorn;
    caf::PdmField<bool>   m_paddingVerticalPillars;

    caf::PdmField<bool>          m_createSimulationJob;
    caf::PdmField<caf::FilePath> m_simulationJobFolder;
    caf::PdmField<QString>       m_simulationJobName;
    caf::PdmField<bool>          m_startSimulationJobAfterExport;

    caf::PdmPtrField<RimEclipseCase*> m_eclipseCase;
    caf::PdmPtrField<RimEclipseView*> m_eclipseView;

    QStringList  m_pageNames;
    QStringList  m_pageSubtitles;
    caf::VecIjk0 m_visibleMin;
    caf::VecIjk0 m_visibleMax;
    int          m_totalCells;
};
