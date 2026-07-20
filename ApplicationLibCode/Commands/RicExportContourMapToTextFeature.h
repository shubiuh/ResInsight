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
/// @brief Declares export contour map to text command support.
#pragma once

#include "CommandFileInterface/Core/RicfCommandObject.h"

#include "cafCmdFeature.h"
#include "cafPdmField.h"

#include <utility>

class RimContourMapProjection;
class RimEclipseContourMapView;
class RimGeoMechContourMapView;

class QTextStream;

//==================================================================================================
/// @brief Command feature for export contour map to text.
//==================================================================================================
class RicExportContourMapToTextFeature : public caf::CmdFeature, public RicfCommandObject
{
    RICF_HEADER_INIT;

public:
    /// Constructs the command object.
    RicExportContourMapToTextFeature();
    /// Executes command support.
    caf::PdmScriptResponse execute() override;

    /// @return The matching contour map view.
    static std::pair<RimEclipseContourMapView*, RimGeoMechContourMapView*> findContourMapView();

protected:
    /// @return Whether the command is available for the current selection.
    bool isCommandEnabled() const override;
    /// Executes the command for the current selection.
    void onActionTriggered( bool isChecked ) override;
    /// Configures the command action's text, icon, and state.
    void setupActionLook( QAction* actionToSetup ) override;

    /// Writes meta data to stream.
    static void writeMetaDataToStream( QTextStream&                   stream,
                                       const RimContourMapProjection* contourMapProjection,
                                       const QString&                 caseName,
                                       bool                           exportLocalCoordinates );

    /// Writes contour map to stream.
    static void writeContourMapToStream( QTextStream&                   stream,
                                         const RimContourMapProjection* contourMapProjection,
                                         bool                           exportLocalCoordinates,
                                         const QString&                 undefinedValueLabel,
                                         bool                           excludeUndefinedValues );

private:
    caf::PdmField<QString> m_exportFileName;
    caf::PdmField<bool>    m_exportLocalCoordinates;
    caf::PdmField<QString> m_undefinedValueLabel;
    caf::PdmField<bool>    m_excludeUndefinedValues;
    caf::PdmField<int>     m_viewId;
};
