/////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Oil-field-level owner of cases, wells, observations, templates, and views.
//
//  Copyright (C) 2011-     Statoil ASA
//  Copyright (C) 2013-     Ceetron Solutions AS
//  Copyright (C) 2011-2012 Ceetron AS
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

#include "cafPdmChildField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"
#include "cafPdmPointer.h"

class RimEclipseCaseCollection;
class RimFormationNamesCollection;
class RimFractureTemplateCollection;
class RimCompletionTemplateCollection;
class RimValveTemplateCollection;
class RimGeoMechModels;
class RimObservedDataCollection;
class RimSummaryCase;
class RimSummaryCaseMainCollection;
class RimWellPathCollection;
class RimAnnotationCollection;
class RimMeasurement;
class RimSeismicDataCollection;
class RimSeismicViewCollection;
class RimSurfaceCollection;
class RimEnsembleWellLogsCollection;
class RimPolygonCollection;
class RimEclipseViewCollection;
class RimEclipseContourMapViewCollection;
class RimVfpDataCollection;
class RimCloudDataSourceCollection;

//==================================================================================================
/// @brief Groups the domain collections that together represent one oil field in a project.
///
/// RimOilField is the principal child beneath RimProject. Its PDM child fields own reservoir and
/// geo-mechanical cases, wells, summary data, annotations, surfaces, seismic data, templates, and
/// related views. The object is primarily an ownership and navigation boundary; subsystem behavior
/// remains implemented by the individual collections.
//==================================================================================================
class RimOilField : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Creates all mandatory child collections for an empty oil field.
    RimOilField();
    /// Destroys the complete oil-field child graph through PDM ownership.
    ~RimOilField() override;

    /// @return Mutable completion fracture-template collection.
    RimFractureTemplateCollection*       fractureDefinitionCollection();
    /// @return Const completion fracture-template collection.
    const RimFractureTemplateCollection* fractureDefinitionCollection() const;

    /// @return Mutable completion valve-template collection.
    RimValveTemplateCollection*       valveTemplateCollection();
    /// @return Const completion valve-template collection.
    const RimValveTemplateCollection* valveTemplateCollection() const;

    /// Owned Eclipse reservoir cases, ensembles, and identical-grid groups.
    caf::PdmChildField<RimEclipseCaseCollection*>           analysisModels;
    /// Owned geo-mechanical case collection.
    caf::PdmChildField<RimGeoMechModels*>                   geoMechModels;
    /// Owned imported and modeled well paths.
    caf::PdmChildField<RimWellPathCollection*>              wellPathCollection;
    /// Owned reusable completion templates.
    caf::PdmChildField<RimCompletionTemplateCollection*>    completionTemplateCollection;
    /// Owned summary cases and ensembles.
    caf::PdmChildField<RimSummaryCaseMainCollection*>       summaryCaseMainCollection;
    /// Owned observed summary and well-log datasets.
    caf::PdmChildField<RimObservedDataCollection*>          observedDataCollection;
    /// Owned formation-name definitions available to cases.
    caf::PdmChildField<RimFormationNamesCollection*>        formationNamesCollection;
    /// Owned text, polyline, reach-circle, and other annotations.
    caf::PdmChildField<RimAnnotationCollection*>            annotationCollection;
    /// Owned transient interactive measurement state.
    caf::PdmChildField<RimMeasurement*>                     measurement;
    /// Owned imported and generated surfaces.
    caf::PdmChildField<RimSurfaceCollection*>               surfaceCollection;
    /// Owned seismic datasets.
    caf::PdmChildField<RimSeismicDataCollection*>           seismicDataCollection;
    /// Owned seismic section views.
    caf::PdmChildField<RimSeismicViewCollection*>           seismicViewCollection;
    /// Owned Eclipse 3D views not nested directly under cases.
    caf::PdmChildField<RimEclipseViewCollection*>           eclipseViewCollection;
    /// Owned ensemble well-log datasets and plots.
    caf::PdmChildField<RimEnsembleWellLogsCollection*>      ensembleWellLogsCollection;
    /// Owned polygon geometry used by filters and modeling tools.
    caf::PdmChildField<RimPolygonCollection*>               polygonCollection;
    /// Owned Eclipse contour-map views.
    caf::PdmChildField<RimEclipseContourMapViewCollection*> eclipseContourMapCollection;
    /// Owned vertical-flow-performance tables and models.
    caf::PdmChildField<RimVfpDataCollection*>               vfpDataCollection;
    /// Owned imported cloud summary data-source definitions.
    caf::PdmChildField<RimCloudDataSourceCollection*>       cloudDataCollection;

protected:
    /// Migrates obsolete fracture-template storage and restores missing mandatory children.
    void initAfterRead() override;

private:
    /// Legacy fracture collection retained only for project-file migration.
    caf::PdmChildField<RimFractureTemplateCollection*> m_fractureTemplateCollection_OBSOLETE;
};
