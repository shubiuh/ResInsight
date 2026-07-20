/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Abstract persistent base for grid-backed simulation and geo-mechanical cases.
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

#pragma once

#include "RimCaseDisplayNameTools.h"

#include "cafPdmChildArrayField.h"
#include "cafPdmChildField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPtrField.h"

#include "cvfVector3.h"

#include <vector>

class Rim3dView;
class RimGridView;
class RimFormationNames;
class RimTimeStepFilter;
class Rim2dIntersectionView;
class RimExtrudedCurveIntersection;
class Rim2dIntersectionViewCollection;

namespace cvf
{
class BoundingBox;
}

/// @brief Defines identity, naming, time steps, spatial bounds, formations, and views for a grid case.
///
/// Concrete Eclipse and geo-mechanical cases implement data-source-specific time steps, bounding
/// boxes, formation updates, and cell scale. RimCase owns shared filters and intersection views and
/// provides stable case identifiers assigned by RimProject. View pointers returned from queries are
/// borrowed descendants or associated special views; the case does not transfer ownership.
class RimCase : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

    using DisplayNameEnum = caf::AppEnum<RimCaseDisplayNameTools::DisplayName>;

public:
    /// Initializes shared case fields, time-step filtering, and intersection-view storage.
    RimCase();
    /// Guards view queries during teardown and releases owned child objects.
    ~RimCase() override;

    /// @return Stable project-local case identifier.
    int  caseId() const;
    /// Sets the project-local case identifier to @p id.
    void setCaseId( int id );

    /// @return User-editable case description.
    QString caseUserDescription() const;
    /// Updates the user description and derived tree-item name.
    void    setCaseUserDescription( const QString& description );

    /// Stores the primary grid/data filename used by the case.
    void    setGridFileName( const QString& fileName );
    /// @return Stored primary grid/data filename.
    QString gridFileName() const;

    /// @return Strategy currently used to generate the displayed case name.
    DisplayNameEnum displayNameType() const;
    /// Changes the display-name strategy and refreshes the tree label.
    void            setDisplayNameType( RimCaseDisplayNameTools::DisplayName displayNameType );
    /// Stores a custom case label and selects custom-name display when applicable.
    void            setCustomCaseName( const QString& caseName );
    /// Recomputes the automatically shortened unique name.
    void            updateAutoShortName();
    /// Enables or disables name fields according to the selected strategy.
    void            updateOptionSensitivity();

    /// @return All regular and specialized 3D views associated with this case.
    std::vector<Rim3dView*>   views() const;
    /// @return Associated views that derive from RimGridView.
    std::vector<RimGridView*> gridViews() const;

    /// @name Case time-step interface
    /// @{
    virtual std::vector<QDateTime> timeStepDates() const              = 0;
    virtual QStringList            timeStepStrings() const            = 0;
    virtual QString                timeStepName( int frameIdx ) const = 0;
    /// @}

    /// @name Spatial extent interface
    /// Bounding boxes use the native case coordinate system before display offset is applied.
    /// @{
    virtual cvf::BoundingBox reservoirBoundingBox()         = 0;
    virtual cvf::BoundingBox activeCellsBoundingBox() const = 0;
    virtual cvf::BoundingBox allCellsBoundingBox() const    = 0;
    /// @}

    /// @return Translation applied when placing case geometry in display coordinates.
    virtual cvf::Vec3d displayModelOffset() const;

    /// Associates a non-owned formation-name definition with this case.
    void               setFormationNames( RimFormationNames* formationNames );
    /// @return Active formation-name definition, or `nullptr` when none is selected.
    RimFormationNames* activeFormationNames() const;
    /// Reloads or recomputes formation-name data in the concrete case backend.
    virtual void       updateFormationNamesData() = 0;

    /// @return Representative cell dimension used for tolerances and display scaling.
    virtual double characteristicCellSize() const = 0;

    /// Maps a filtered UI time-step position to the case's native time-step index.
    size_t uiToNativeTimeStepIndex( size_t uiTimeStepIndex );

    /// @return Owned collection of 2D intersection views for this case.
    Rim2dIntersectionViewCollection* intersectionViewCollection();

    /// Emitted when shared case settings change and dependents should refresh.
    caf::Signal<> settingsChanged;

protected:
    /// Supplies formation-name and display-name choices for property editors.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;
    /// Propagates naming, formation, and time-step-filter changes from the UI.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;

    /// @return Data-source-specific views that are not direct case descendants.
    virtual std::vector<Rim3dView*> allSpecialViews() const = 0;
    /// Restores transient naming state and mandatory children after deserialization.
    void                            initAfterRead() override;
    /// Recomputes the tree label from the selected display-name strategy.
    void                            updateTreeItemName();

    /// @return Base name used when composing automatic case labels.
    virtual QString caseName() const;

    /// Finds a project-unique shortened name no longer than @p shortNameLengthLimit when possible.
    static QString uniqueShortNameCase( RimCase* rimCase, int shortNameLengthLimit );

private:
    /// Uses the case description as the generic PDM object label field.
    caf::PdmFieldHandle* userDescriptionField() override;

protected:
    /// Stable project-local identifier.
    caf::PdmField<int>                                   m_caseId;
    /// User-editable case description or custom name.
    caf::PdmField<QString>                               m_caseUserDescription;
    /// Primary imported data-file path.
    caf::PdmField<caf::FilePath>                         m_caseFileName;
    /// Strategy controlling how the case appears in trees and selectors.
    caf::PdmField<DisplayNameEnum>                       m_displayNameOption;
    /// Owned filter mapping visible UI steps to native case time steps.
    caf::PdmChildField<RimTimeStepFilter*>               m_timeStepFilter;
    /// Owned 2D intersection views associated with this case.
    caf::PdmChildField<Rim2dIntersectionViewCollection*> m_2dIntersectionViewCollection;
    /// Non-owning selected formation-name definition.
    caf::PdmPtrField<RimFormationNames*>                 m_activeFormationNames;

private:
    /// Suppresses callbacks into partially destroyed views during case teardown.
    bool m_isInActiveDestruction;
};
