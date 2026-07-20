/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2016 Statoil ASA
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
/// @brief Declares summary plot editor command support.

#pragma once

#include "RiuSummaryVectorSelectionWidgetCreator.h"

#include "cafPdmChildArrayField.h"
#include "cafPdmChildField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPointer.h"
#include "cafPdmProxyValueField.h"
#include "cafPdmPtrField.h"

#include <memory>

#define OBSERVED_DATA_AVALUE_POSTFIX "_OBSDATA"

/// @brief Utilities for caf command workflows.
namespace caf
{
class PdmObject;
};

class RimSummaryCase;
class RimSummaryCurve;
class RimSummaryMultiPlot;
class RimSummaryPlot;
class RiaSummaryCurveDefinition;
class RimEnsembleCurveSet;
class RimCalculatedSummaryCase;

//==================================================================================================
/// @brief UI model for configuring summary plot editor.
//==================================================================================================
class RicSummaryPlotEditorUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    static const QString CONFIGURATION_NAME;

public:
    /// Constructs the command object.
    RicSummaryPlotEditorUi();
    /// Destroys the command object.
    ~RicSummaryPlotEditorUi() override;

    /// @return The preview plot.
    RimSummaryPlot* previewPlot() const;
    /// Updates from summary plot.
    void            updateFromSummaryPlot( RimSummaryPlot*                     targetPlot,
                                           const std::vector<caf::PdmObject*>& defaultSources = std::vector<caf::PdmObject*>() );

    /// Updates from summary multi plot.
    void updateFromSummaryMultiPlot( RimSummaryMultiPlot*                summaryMultiPlot,
                                     const std::vector<caf::PdmObject*>& defaultSources = std::vector<caf::PdmObject*>() );

    /// Adds ress selection widget.
    QWidget* addressSelectionWidget( QWidget* parent );

    /// @return Whether close button pressed.
    bool isCloseButtonPressed() const;
    /// Removes or clears close button.
    void clearCloseButton();
    /// Updates curve names.
    void updateCurveNames();

private:
    /// Responds to a value changed through the command UI.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;
    /// @return The selectable values for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;
    /// Defines the field ordering used by the command UI.
    void                          defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;
    /// Performs the define editor attribute command operation.
    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;

    /// Performs the sync preview curves from UI selection command operation.
    void syncPreviewCurvesFromUiSelection();
    /// Updates preview curves from curve definitions.
    void updatePreviewCurvesFromCurveDefinitions( const std::set<RiaSummaryCurveDefinition>& allCurveDefsToDisplay,
                                                  const std::set<RiaSummaryCurveDefinition>& curveDefsToAdd,
                                                  const std::set<RimSummaryCurve*>&          curvesToDelete,
                                                  const std::set<RimEnsembleCurveSet*>&      curveSetsToDelete );

    /// Performs the populate curve creator command operation.
    void        populateCurveCreator( const RimSummaryPlot& sourceSummaryPlot );
    /// Updates target plot.
    void        updateTargetPlot();
    /// Copies curve and add to plot.
    static void copyCurveAndAddToPlot( const RimSummaryCurve* curve, RimSummaryPlot* plot, bool forceVisible = false );
    /// Sets default curve selection.
    void        setDefaultCurveSelection( const std::vector<caf::PdmObject*>& defaultCases );

    /// Performs the reset all fields command operation.
    void resetAllFields();
    /// Applies appearance to all preview curves.
    void applyAppearanceToAllPreviewCurves();
    /// Creates new plot.
    void createNewPlot();
    /// @return Whether observed data.
    bool isObservedData( RimSummaryCase* sumCase ) const;

    /// Selects ion editor field changed.
    void selectionEditorFieldChanged();
    /// Sets initial curve visibility.
    void setInitialCurveVisibility( const RimSummaryPlot* targetPlot );

    /// Performs the on ok button clicked command operation.
    void onOkButtonClicked();
    /// Performs the on apply button clicked command operation.
    void onApplyButtonClicked();

private:
    caf::PdmPtrField<RimSummaryPlot*> m_targetPlot;

    std::unique_ptr<RimSummaryPlot> m_previewPlot;

    bool m_closeButtonPressed;

    std::unique_ptr<RiuSummaryVectorSelectionWidgetCreator> m_summaryCurveSelectionEditor;

    caf::PdmPointer<RimSummaryMultiPlot> m_plotContainer;
};
