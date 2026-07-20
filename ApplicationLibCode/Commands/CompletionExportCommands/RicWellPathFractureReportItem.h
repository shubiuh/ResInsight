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
/// @brief Declares well path fracture report item command support.

#pragma once

#include "RiaDefines.h"
#include "RiaTextStringTools.h"

#include <QString>

//==================================================================================================
/// @brief Supports well path fracture report item command workflows.
//==================================================================================================
class RicWellPathFractureReportItem
{
public:
    /// Constructs the command object.
    RicWellPathFractureReportItem( const QString& wellPathNameForExport, const QString& fractureName, const QString& fractureTemplateName );

    /// Sets data.
    void setData( double trans, size_t connCount, double area );
    /// Sets width and conductivity.
    void setWidthAndConductivity( double width, double conductivity );
    /// Sets height and half length.
    void setHeightAndHalfLength( double height, double halfLength );
    /// Sets area weighted permeability.
    void setAreaWeightedPermeability( double permeability );
    /// Sets unit system.
    void setUnitSystem( RiaDefines::EclipseUnitSystem unitSystem );
    /// Sets pressure depletion parameters.
    void setPressureDepletionParameters( bool           performPressureDepletionScaling,
                                         const QString& timeStepString,
                                         const QString& wbhpString,
                                         double         userWBHP,
                                         double         actualWBHP,
                                         double         minPressureDrop,
                                         double         maxPressureDrop );

    /// @return The well path name for export.
    QString wellPathNameForExport() const;
    /// @return The fracture name.
    QString fractureName() const;
    /// @return The fracture template name.
    QString fractureTemplateName() const;

    /// @return The unit system.
    RiaDefines::EclipseUnitSystem unitSystem() const;

    /// @return The transmissibility.
    double transmissibility() const;
    /// @return The connection count.
    size_t connectionCount() const;
    /// @return The fcd.
    double fcd() const;
    /// @return The area.
    double area() const;

    /// @return The kfwf.
    double kfwf() const;
    /// @return The kf.
    double kf() const;
    /// @return The wf.
    double wf() const;

    /// @return The xf.
    double xf() const;
    /// @return The h.
    double h() const;
    /// @return The km.
    double km() const;
    /// @return The kmxf.
    double kmxf() const;

    /// @return The perform pressure depletion scaling.
    bool    performPressureDepletionScaling() const;
    /// @return The pressure depletion time step string.
    QString pressureDepletionTimeStepString() const;
    /// @return The pressure depletion WBHP string.
    QString pressureDepletionWBHPString() const;
    /// @return The pressure depletion user WBHP.
    double  pressureDepletionUserWBHP() const;
    /// @return The pressure depletion actual WBHP.
    double  pressureDepletionActualWBHP() const;
    /// @return The pressure depletion min pressure drop.
    double  pressureDepletionMinPressureDrop() const;
    /// @return The pressure depletion max pressure drop.
    double  pressureDepletionMaxPressureDrop() const;

    /// Provides the corresponding operator for command data.
    auto operator<=>( const RicWellPathFractureReportItem& rhs ) const = default;

private:
    RiaDefines::EclipseUnitSystem m_unitSystem;
    QString                       m_wellPathNameForExport;
    QString                       m_wellPathFracture;
    QString                       m_wellPathFractureTemplate;

    double m_transmissibility;
    size_t m_connectionCount;
    double m_area;

    double m_kfwf;
    double m_kf;
    double m_wf;
    double m_xf;
    double m_h;
    double m_km;

    bool    m_performPressureDepletionScaling;
    QString m_pressureDepletionTimeStepString;
    QString m_pressureDepletionWBHPString;
    double  m_pressureDepletionUserWBHP;
    double  m_pressureDepletionActualWBHP;
    double  m_pressureDepletionMinPressureDrop;
    double  m_pressureDepletionMaxPressureDrop;
};
