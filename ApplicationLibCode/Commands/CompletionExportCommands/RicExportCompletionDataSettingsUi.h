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
/// @brief Declares export completion data settings command support.

#pragma once

#include "RicCaseAndFileExportSettingsUi.h"
#include "RicExportFractureCompletionsImpl.h"

#include "cafAppEnum.h"
#include "cafPdmField.h"

//==================================================================================================
/// @brief UI model for configuring export completion data settings.
//==================================================================================================
class RicExportCompletionDataSettingsUi : public RicCaseAndFileExportSettingsUi
{
    CAF_PDM_HEADER_INIT;

public:
    // Exported in .proto file. Do not change without changing .proto
    enum class ExportSplit
    {
        UNIFIED_FILE,
        SPLIT_ON_WELL,
    };
    using ExportSplitType = caf::AppEnum<ExportSplit>;

    // Exported in .proto file. Do not change without changing .proto
    enum class CompdatExport
    {
        TRANSMISSIBILITIES,
        WPIMULT_AND_DEFAULT_CONNECTION_FACTORS,
    };
    using CompdatExportType = caf::AppEnum<CompdatExport>;

    using TransScalingWBHPSource = caf::AppEnum<RicExportFractureCompletionsImpl::PressureDepletionWBHPSource>;

    /// Constructs the command object.
    RicExportCompletionDataSettingsUi();

    caf::PdmField<int> timeStep;

    caf::PdmField<ExportSplitType>   fileSplit;
    caf::PdmField<CompdatExportType> compdatExport;

    caf::PdmField<bool>                   performTransScaling;
    caf::PdmField<int>                    transScalingTimeStep;
    caf::PdmField<TransScalingWBHPSource> transScalingWBHPSource;
    caf::PdmField<double>                 transScalingWBHP;

    caf::PdmField<bool> includeMsw;
    caf::PdmField<bool> useLateralNTG;
    caf::PdmField<bool> includePerforations;
    caf::PdmField<bool> includeFishbones;
    caf::PdmField<bool> excludeMainBoreForFishbones;

    caf::PdmField<bool> includeFractures;

    /// Performs the enable include MSW command operation.
    void enableIncludeMsw();

    /// Shows fracture in.
    void showFractureInUi( bool enable );
    /// Shows perforations in.
    void showPerforationsInUi( bool enable );
    /// Shows fishbones in.
    void showFishbonesInUi( bool enable );

    /// Sets export data source as comment.
    void setExportDataSourceAsComment( bool enable );
    /// Exports data source as comment.
    bool exportDataSourceAsComment() const;

    /// Sets export welspec.
    void setExportWelspec( bool enable );
    /// Exports welspec.
    bool exportWelspec() const;

    /// Sets export completion welspec after main bore.
    void setExportCompletionWelspecAfterMainBore( bool enable );
    /// Exports completion welspec after main bore.
    bool exportCompletionWelspecAfterMainBore() const;

    /// Sets custom file name.
    void    setCustomFileName( const QString& fileName );
    /// @return The custom file name.
    QString customFileName() const;

    /// Responds to a value changed through the command UI.
    void fieldChangedByUi( const caf::PdmFieldHandle* changedField, const QVariant& oldValue, const QVariant& newValue ) override;

protected:
    /// @return The selectable values for the requested PDM field.
    QList<caf::PdmOptionItemInfo> calculateValueOptions( const caf::PdmFieldHandle* fieldNeedingOptions ) override;
    /// Defines the field ordering used by the command UI.
    void                          defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;

    /// @return The generate well production start strings.
    std::map<int, std::vector<std::pair<QString, QString>>> generateWellProductionStartStrings();

private:
    caf::PdmField<bool>    m_exportDataSourceAsComment;
    caf::PdmField<bool>    m_exportWelspec;
    caf::PdmField<bool>    m_completionWelspecAfterMainBore;
    caf::PdmField<bool>    m_useCustomFileName;
    caf::PdmField<QString> m_customFileName;

    bool m_fracturesEnabled;
    bool m_perforationsEnabled;
    bool m_fishbonesEnabled;
};
