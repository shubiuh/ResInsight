/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2025-     Equinor ASA
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
/// @brief Implements configuration for simulation input.

#include "RigSimulationInputSettings.h"

#include "RigNoRefinement.h"

#include "opm/input/eclipse/Deck/DeckRecord.hpp"

//--------------------------------------------------------------------------------------------------
/// Creates a RigSimulationInputSettings instance.
//--------------------------------------------------------------------------------------------------
RigSimulationInputSettings::RigSimulationInputSettings()
    : m_min( caf::VecIjk0::ZERO )
    , m_max( caf::VecIjk0::ZERO )
    , m_refinement( std::make_unique<RigNoRefinement>( cvf::Vec3st( 0, 0, 0 ) ) )
    , m_boundaryCondition( RiaModelExportDefines::BoundaryCondition::OPERNUM_OPERATER )
    , m_porvMultiplier( 1.0e6 )
{
}

//--------------------------------------------------------------------------------------------------
/// Returns the min.
//--------------------------------------------------------------------------------------------------
caf::VecIjk0 RigSimulationInputSettings::min() const
{
    return m_min;
}

//--------------------------------------------------------------------------------------------------
/// Returns the max.
//--------------------------------------------------------------------------------------------------
caf::VecIjk0 RigSimulationInputSettings::max() const
{
    return m_max;
}

//--------------------------------------------------------------------------------------------------
/// Sets min.
//--------------------------------------------------------------------------------------------------
void RigSimulationInputSettings::setMin( const caf::VecIjk0& min )
{
    m_min = min;
}

//--------------------------------------------------------------------------------------------------
/// Sets max.
//--------------------------------------------------------------------------------------------------
void RigSimulationInputSettings::setMax( const caf::VecIjk0& max )
{
    m_max = max;
}

//--------------------------------------------------------------------------------------------------
/// Returns the validate box.
//--------------------------------------------------------------------------------------------------
std::expected<void, QString> RigSimulationInputSettings::validateBox() const
{
    if ( m_min.i() > m_max.i() || m_min.j() > m_max.j() || m_min.k() > m_max.k() )
    {
        return std::unexpected(
            "Invalid cell box: Min should be less than or equal to Max in all dimensions. Please check export settings." );
    }
    return {};
}

//--------------------------------------------------------------------------------------------------
/// Returns the refinement.
//--------------------------------------------------------------------------------------------------
const RigRefinement& RigSimulationInputSettings::refinement() const
{
    return *m_refinement;
}

//--------------------------------------------------------------------------------------------------
/// Sets refinement.
//--------------------------------------------------------------------------------------------------
void RigSimulationInputSettings::setRefinement( std::unique_ptr<RigRefinement> refinement )
{
    m_refinement = std::move( refinement );
}

//--------------------------------------------------------------------------------------------------
/// Returns whether refinement is available.
//--------------------------------------------------------------------------------------------------
bool RigSimulationInputSettings::hasRefinement() const
{
    return m_refinement && m_refinement->hasRefinement();
}

//--------------------------------------------------------------------------------------------------
/// Sets keywords to remove.
//--------------------------------------------------------------------------------------------------
void RigSimulationInputSettings::setKeywordsToRemove( const std::vector<std::string>& keywords )
{
    m_keywordsToRemove = keywords;
}

//--------------------------------------------------------------------------------------------------
/// Returns the keywords to remove.
//--------------------------------------------------------------------------------------------------
const std::vector<std::string>& RigSimulationInputSettings::keywordsToRemove() const
{
    return m_keywordsToRemove;
}

//--------------------------------------------------------------------------------------------------
/// Returns the bcprop keywords.
//--------------------------------------------------------------------------------------------------
std::vector<Opm::DeckRecord> RigSimulationInputSettings::bcpropKeywords() const
{
    return m_bcpropKeywords;
}

//--------------------------------------------------------------------------------------------------
/// Sets bcprop keywords.
//--------------------------------------------------------------------------------------------------
void RigSimulationInputSettings::setBcpropKeywords( const std::vector<Opm::DeckRecord>& keywords )
{
    m_bcpropKeywords = keywords;
}

//--------------------------------------------------------------------------------------------------
/// Returns the boundary condition.
//--------------------------------------------------------------------------------------------------
RiaModelExportDefines::BoundaryCondition RigSimulationInputSettings::boundaryCondition() const
{
    return m_boundaryCondition;
}

//--------------------------------------------------------------------------------------------------
/// Sets boundary condition.
//--------------------------------------------------------------------------------------------------
void RigSimulationInputSettings::setBoundaryCondition( RiaModelExportDefines::BoundaryCondition value )
{
    m_boundaryCondition = value;
}

//--------------------------------------------------------------------------------------------------
/// Returns the porv multiplier.
//--------------------------------------------------------------------------------------------------
double RigSimulationInputSettings::porvMultiplier() const
{
    return m_porvMultiplier;
}

//--------------------------------------------------------------------------------------------------
/// Sets porv multiplier.
//--------------------------------------------------------------------------------------------------
void RigSimulationInputSettings::setPorvMultiplier( double value )
{
    m_porvMultiplier = value;
}

//--------------------------------------------------------------------------------------------------
/// Returns the input deck file name.
//--------------------------------------------------------------------------------------------------
QString RigSimulationInputSettings::inputDeckFileName() const
{
    return m_inputDeckFileName;
}

//--------------------------------------------------------------------------------------------------
/// Sets input deck file name.
//--------------------------------------------------------------------------------------------------
void RigSimulationInputSettings::setInputDeckFileName( const QString& fileName )
{
    m_inputDeckFileName = fileName;
}

//--------------------------------------------------------------------------------------------------
/// Returns the output deck file name.
//--------------------------------------------------------------------------------------------------
QString RigSimulationInputSettings::outputDeckFileName() const
{
    return m_outputDeckFileName;
}

//--------------------------------------------------------------------------------------------------
/// Sets output deck file name.
//--------------------------------------------------------------------------------------------------
void RigSimulationInputSettings::setOutputDeckFileName( const QString& fileName )
{
    m_outputDeckFileName = fileName;
}

//--------------------------------------------------------------------------------------------------
/// Returns the padding settings.
//--------------------------------------------------------------------------------------------------
const RigModelPaddingSettings& RigSimulationInputSettings::paddingSettings() const
{
    return m_paddingSettings;
}

//--------------------------------------------------------------------------------------------------
/// Sets padding settings.
//--------------------------------------------------------------------------------------------------
void RigSimulationInputSettings::setPaddingSettings( const RigModelPaddingSettings& settings )
{
    m_paddingSettings = settings;
}
