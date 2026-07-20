/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2018-     Equinor ASA
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
/// @brief Declares create multiple fractures command support.

#pragma once

#include "RicCreateMultipleFracturesOptionItemUi.h"

#include <QPointer>

#include "cafPdmChildArrayField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmProxyValueField.h"

class RimEclipseCase;
class RimWellPath;
class RigMainGrid;

/// @brief Utilities for caf command workflows.
namespace caf
{
class PdmUiPropertyViewDialog;
}

//==================================================================================================
/// @brief Supports location for new fracture command workflows.
//==================================================================================================
class LocationForNewFracture
{
public:
    LocationForNewFracture( RimFractureTemplate* fractureTemplate, RimWellPath* wellPath, double measuredDepth )
        : fractureTemplate( fractureTemplate )
        , wellPath( wellPath )
        , measuredDepth( measuredDepth )
    {
    }

    bool operator<( const LocationForNewFracture& loc ) const { return measuredDepth < loc.measuredDepth; }

    RimFractureTemplate* fractureTemplate;
    RimWellPath*         wellPath;
    double               measuredDepth;
};

//==================================================================================================
/// @brief UI model for configuring create multiple fractions.
//==================================================================================================
class RiuCreateMultipleFractionsUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    static const QString ADD_FRACTURES_BUTTON_TEXT;
    static const QString REPLACE_FRACTURES_BUTTON_TEXT;

    /// Constructs the command object.
    RiuCreateMultipleFractionsUi();

    /// Sets parent.
    void setParentDialog( QPointer<caf::PdmUiPropertyViewDialog> dialog );
    /// Sets values.
    void setValues( RimEclipseCase* eclipseCase, double minimumDistanceFromWellTip, int maxFracturesPerWell );
    /// Performs the reset values command operation.
    void resetValues();

    /// @return The options.
    std::vector<RicCreateMultipleFracturesOptionItemUi*> options() const;

    /// Inserts option item.
    void insertOptionItem( RicCreateMultipleFracturesOptionItemUi* insertAfterThisObject,
                           RicCreateMultipleFracturesOptionItemUi* objectToInsert );

    /// Removes or clears option item.
    void deleteOptionItem( RicCreateMultipleFracturesOptionItemUi* optionsItem );

    /// Removes or clears options.
    void clearOptions();

    /// Adds well path.
    void addWellPath( RimWellPath* wellPath );

    /// Removes or clears well paths.
    void clearWellPaths();

    /// @return The locations for new fractures.
    std::vector<LocationForNewFracture> locationsForNewFractures() const;

    /// Updates buttons enable state.
    void updateButtonsEnableState();

private:
    /// @return The selectable values for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;
    /// Performs the define custom context menu command operation.
    void defineCustomContextMenu( const caf::PdmFieldHandle* fieldNeedingMenu, QMenu* menu, QWidget* fieldEditorWidget ) override;
    /// Performs the define editor attribute command operation.
    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;

    /// @return The summary text.
    QString summaryText() const;

private:
    caf::PdmPtrField<RimEclipseCase*>                                m_sourceCase;
    caf::PdmField<double>                                            m_minDistanceFromWellTd;
    caf::PdmField<int>                                               m_maxFracturesPerWell;
    caf::PdmChildArrayField<RicCreateMultipleFracturesOptionItemUi*> m_options;

    caf::PdmProxyValueField<QString> m_fractureCreationSummary;

    std::vector<RimWellPath*> m_wellPaths;

    QPointer<caf::PdmUiPropertyViewDialog> m_dialog;
};
