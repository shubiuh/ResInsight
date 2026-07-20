/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2016-     Statoil ASA
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
/// @brief Declares export to LAS file resample command support.

#pragma once

#include "cafAppEnum.h"
#include "cafPdmChildArrayField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"

//==================================================================================================
/// @brief Supports export to LAS file obj command workflows.
//==================================================================================================
class RicExportToLasFileObj : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Constructs the command object.
    RicExportToLasFileObj();

    caf::PdmField<QString> tvdrkbOffset;
};

//==================================================================================================
/// @brief UI model for configuring export to LAS file resample.
//==================================================================================================
class RicExportToLasFileResampleUi : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    enum class CurveUnitConversion
    {
        EXPORT_NORMALIZED,
        EXPORT_WITH_STANDARD_UNITS
    };
    using CurveUnitConversionEnum = caf::AppEnum<CurveUnitConversion>;

public:
    /// Constructs the command object.
    RicExportToLasFileResampleUi();

    caf::PdmField<QString> exportFolder;
    caf::PdmField<QString> filePrefix;
    caf::PdmField<bool>    capitalizeFileName;

    caf::PdmField<bool>   activateResample;
    caf::PdmField<double> resampleInterval;

    caf::PdmField<bool>                    exportTvdrkb;
    caf::PdmField<CurveUnitConversionEnum> curveUnitConversion;

    /// Performs the tvdrkb diff for well paths command operation.
    void tvdrkbDiffForWellPaths( std::vector<double>* rkbDiffs );
    /// Sets rkb diffs.
    void setRkbDiffs( const std::vector<QString>& wellNames, const std::vector<double>& rkbDiffs );
    /// Sets unit conversion option enabled.
    void setUnitConversionOptionEnabled( bool enabled );

    /// Responds to a value changed through the command UI.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;
    /// Performs the define editor attribute command operation.
    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;

protected:
    /// Defines the field ordering used by the command UI.
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;

private:
    /// Updates field visibility.
    void updateFieldVisibility();

private:
    caf::PdmChildArrayField<RicExportToLasFileObj*> m_tvdrkbOffsets;

    bool m_enableCurveUnitConversion;
};
