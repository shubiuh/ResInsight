/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2024     Equinor ASA
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
/// @brief Owned collection of Eclipse views with optional case-choice filtering.

#pragma once

#include "cafPdmField.h"

#include "cafPdmChildArrayField.h"
#include "cafPdmObject.h"

#include <QString>

#include <functional>

class RimEclipseView;
class RimEclipseCase;

/// @brief Owns Eclipse views and propagates a runtime case-provider callback to each view.
///
/// The provider is intentionally not serialized. Specialized collections use it to restrict case
/// choices, while ordinary case-owned collections leave it empty and expose all applicable cases.
class RimEclipseViewCollection : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes an empty owned view array.
    RimEclipseViewCollection();
    ~RimEclipseViewCollection() override;

    /// @return Whether the collection contains no views.
    bool isEmpty();

    /// Creates, configures, and owns a new view for @p eclipseCase.
    RimEclipseView* addView( RimEclipseCase* eclipseCase );
    /// Takes ownership of @p view and applies the current provider.
    void            addView( RimEclipseView* view );

    /// Detaches @p view without deleting it.
    void removeView( RimEclipseView* view );

    /// @return Owned views in stored order.
    std::vector<RimEclipseView*> views() const;

    /// Replaces the runtime case provider and propagates it to existing views.
    void setEclipseCaseProvider( std::function<std::vector<RimEclipseCase*>()> provider = nullptr );

private:
    /// Cleans references and presentation after an owned view is deleted.
    void onChildDeleted( caf::PdmChildArrayFieldHandle* childArray, std::vector<caf::PdmObjectHandle*>& referringObjects ) override;

    /// Installs the collection's current provider callback on @p view.
    void applyCallbackToView( RimEclipseView* view );

private:
    /// Owned Eclipse views.
    caf::PdmChildArrayField<RimEclipseView*>      m_views;
    /// Runtime callback supplying cases eligible for contained views.
    std::function<std::vector<RimEclipseCase*>()> m_eclipseCaseProvider;
};
