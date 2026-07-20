/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) 2026-     Equinor ASA
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

#include "RifReaderVtkGrid.h"

#include "RiaLogging.h"

#include "RigActiveCellInfo.h"
#include "RigCaseCellResultsData.h"
#include "RigCell.h"
#include "RigEclipseCaseData.h"
#include "RigEclipseResultAddress.h"
#include "RigMainGrid.h"

#include "cvfBoundingBox.h"
#include "cvfVector3.h"

#include <array>
#include <cmath>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <pugixml.hpp>

//--------------------------------------------------------------------------------------------------
/// Internal helpers
//--------------------------------------------------------------------------------------------------
namespace internal
{
    // VTK cell type constants
    static constexpr int VTK_VERTEX          = 1;
    static constexpr int VTK_LINE            = 3;
    static constexpr int VTK_TRIANGLE        = 5;
    static constexpr int VTK_QUAD            = 9;
    static constexpr int VTK_TETRA           = 10;
    static constexpr int VTK_HEXAHEDRON      = 12;
    static constexpr int VTK_WEDGE           = 13;
    static constexpr int VTK_PYRAMID         = 14;
    static constexpr int VTK_QUADRATIC_EDGE  = 21;
    static constexpr int VTK_QUADRATIC_TRI   = 22;
    static constexpr int VTK_QUADRATIC_QUAD  = 23;
    static constexpr int VTK_QUADRATIC_TETRA = 24;
    static constexpr int VTK_QUADRATIC_HEX   = 25;

    // Number of corners per VTK cell type for linear types
    static int vtkCellCornerCount( int cellType )
    {
        switch ( cellType )
        {
            case VTK_VERTEX:
                return 1;
            case VTK_LINE:
                return 2;
            case VTK_TRIANGLE:
                return 3;
            case VTK_QUAD:
                return 4;
            case VTK_TETRA:
                return 4;
            case VTK_HEXAHEDRON:
                return 8;
            case VTK_WEDGE:
                return 6;
            case VTK_PYRAMID:
                return 5;
            case VTK_QUADRATIC_TRI:
                return 6;
            case VTK_QUADRATIC_QUAD:
                return 8;
            case VTK_QUADRATIC_TETRA:
                return 10;
            case VTK_QUADRATIC_HEX:
                return 20;
            default:
                return 0;
        }
    }

    //--------------------------------------------------------------------------------------------------
    // Simple tokenizer for legacy VTK ASCII format
    //--------------------------------------------------------------------------------------------------
    static std::vector<std::string> tokenize( const std::string& str )
    {
        std::vector<std::string> tokens;
        std::istringstream       iss( str );
        std::string              token;
        while ( iss >> token )
        {
            tokens.push_back( token );
        }
        return tokens;
    }

    //--------------------------------------------------------------------------------------------------
    // Data parsed from a VTK file
    //--------------------------------------------------------------------------------------------------
    struct VtkGridData
    {
        std::vector<cvf::Vec3d>              points;
        std::vector<int>                     cellTypes;      // Per cell VTK type
        std::vector<int>                     cellLocations;  // Offset into connectivity for each cell
        std::vector<int>                     connectivity;   // Flat node index array
        std::map<std::string, std::vector<float>> pointData;  // Point data arrays
        std::map<std::string, std::vector<float>> cellData;   // Cell data arrays
    };

    //--------------------------------------------------------------------------------------------------
    /// Parse a legacy .vtk file (ASCII unstructured grid)
    //--------------------------------------------------------------------------------------------------
    static bool parseLegacyVtk( const std::string& filePath, VtkGridData& gridData, QString* errorMessages )
    {
        std::ifstream file( filePath );
        if ( !file.is_open() )
        {
            if ( errorMessages ) *errorMessages = QString( "Could not open file: %1" ).arg( QString::fromStdString( filePath ) );
            return false;
        }

        std::string line;
        bool        foundUnstructuredGrid = false;

        // Read header
        std::getline( file, line ); // # vtk DataFile Version
        std::getline( file, line ); // Title
        std::getline( file, line ); // ASCII or BINARY

        if ( line.find( "BINARY" ) != std::string::npos )
        {
            if ( errorMessages ) *errorMessages = "Binary VTK files are not supported yet. Please save as ASCII.";
            return false;
        }

        while ( std::getline( file, line ) )
        {
            // Trim whitespace
            line.erase( 0, line.find_first_not_of( " \t\r\n" ) );
            if ( line.empty() ) continue;

            auto tokens = tokenize( line );

            if ( tokens.size() < 2 ) continue;

            if ( tokens[0] == "DATASET" )
            {
                if ( tokens[1] == "UNSTRUCTURED_GRID" )
                {
                    foundUnstructuredGrid = true;
                }
                else
                {
                    if ( errorMessages )
                        *errorMessages = QString( "Unsupported VTK dataset type: %1. Only UNSTRUCTURED_GRID is supported." )
                                             .arg( QString::fromStdString( tokens[1] ) );
                    return false;
                }
            }
            else if ( tokens[0] == "POINTS" && foundUnstructuredGrid )
            {
                int numPoints = std::stoi( tokens[1] );
                gridData.points.reserve( numPoints );
                for ( int i = 0; i < numPoints; i++ )
                {
                    double x, y, z;
                    file >> x >> y >> z;
                    // VTK uses z-up, ResInsight uses z-positive-downward
                    gridData.points.emplace_back( x, y, -z );
                }
            }
            else if ( tokens[0] == "CELLS" && foundUnstructuredGrid )
            {
                int numCells    = std::stoi( tokens[1] );
                int totalValues = std::stoi( tokens[2] );
                gridData.cellLocations.reserve( numCells );

                int connOffset = 0;
                for ( int i = 0; i < numCells; i++ )
                {
                    int numCellPoints;
                    file >> numCellPoints;

                    gridData.cellLocations.push_back( connOffset );
                    for ( int j = 0; j < numCellPoints; j++ )
                    {
                        int nodeIdx;
                        file >> nodeIdx;
                        gridData.connectivity.push_back( nodeIdx );
                    }
                    connOffset += numCellPoints;
                }
            }
            else if ( tokens[0] == "CELL_TYPES" && foundUnstructuredGrid )
            {
                int numTypes = std::stoi( tokens[1] );
                gridData.cellTypes.resize( numTypes );
                for ( int i = 0; i < numTypes; i++ )
                {
                    int type;
                    file >> type;
                    gridData.cellTypes[i] = type;
                }
            }
            else if ( tokens[0] == "POINT_DATA" && foundUnstructuredGrid )
            {
                int numData = std::stoi( tokens[1] );
                // Read scalar data following
                while ( std::getline( file, line ) )
                {
                    line.erase( 0, line.find_first_not_of( " \t\r\n" ) );
                    if ( line.empty() ) continue;
                    auto dataTokens = tokenize( line );
                    if ( dataTokens.size() >= 3 && dataTokens[0] == "SCALARS" )
                    {
                        std::string dataName = dataTokens[1];
                        std::getline( file, line ); // LOOKUP_TABLE
                        std::vector<float> values;
                        values.reserve( numData );
                        for ( int i = 0; i < numData; i++ )
                        {
                            float val;
                            file >> val;
                            values.push_back( val );
                        }
                        gridData.pointData[dataName] = std::move( values );
                    }
                    else if ( dataTokens.size() >= 3 && dataTokens[0] == "VECTORS" )
                    {
                        std::string dataName = dataTokens[1];
                        std::vector<float> values;
                        values.reserve( numData * 3 );
                        for ( int i = 0; i < numData; i++ )
                        {
                            float vx, vy, vz;
                            file >> vx >> vy >> vz;
                            values.push_back( vx );
                            values.push_back( vy );
                            values.push_back( vz );
                        }
                        gridData.pointData[dataName] = std::move( values );
                    }
                    else
                        break;
                }
            }
            else if ( tokens[0] == "CELL_DATA" && foundUnstructuredGrid )
            {
                int numData = std::stoi( tokens[1] );
                while ( std::getline( file, line ) )
                {
                    line.erase( 0, line.find_first_not_of( " \t\r\n" ) );
                    if ( line.empty() ) continue;
                    auto dataTokens = tokenize( line );
                    if ( dataTokens.size() >= 3 && dataTokens[0] == "SCALARS" )
                    {
                        std::string dataName = dataTokens[1];
                        std::getline( file, line ); // LOOKUP_TABLE
                        std::vector<float> values;
                        values.reserve( numData );
                        for ( int i = 0; i < numData; i++ )
                        {
                            float val;
                            file >> val;
                            values.push_back( val );
                        }
                        gridData.cellData[dataName] = std::move( values );
                    }
                    else
                        break;
                }
            }
        }

        if ( !foundUnstructuredGrid )
        {
            if ( errorMessages ) *errorMessages = "No UNSTRUCTURED_GRID dataset found in VTK file.";
            return false;
        }

        return true;
    }

    //--------------------------------------------------------------------------------------------------
    /// Parse an XML .vtu file
    //--------------------------------------------------------------------------------------------------
    static bool parseVtu( const std::string& filePath, VtkGridData& gridData, QString* errorMessages )
    {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file( filePath.c_str() );
        if ( !result )
        {
            if ( errorMessages ) *errorMessages = QString( "Failed to parse XML: %1" ).arg( result.description() );
            return false;
        }

        auto vtkFile = doc.child( "VTKFile" );
        if ( !vtkFile )
        {
            if ( errorMessages ) *errorMessages = "Missing VTKFile root element.";
            return false;
        }

        auto grid = vtkFile.child( "UnstructuredGrid" );
        if ( !grid )
        {
            if ( errorMessages ) *errorMessages = "Missing UnstructuredGrid element.";
            return false;
        }

        auto piece = grid.child( "Piece" );
        if ( !piece )
        {
            if ( errorMessages ) *errorMessages = "Missing Piece element.";
            return false;
        }

        // Read points
        auto pointsElem = piece.child( "Points" );
        if ( pointsElem )
        {
            for ( auto dataArray = pointsElem.child( "DataArray" ); dataArray; dataArray = dataArray.next_sibling( "DataArray" ) )
            {
                std::string name  = dataArray.attribute( "Name" ).as_string();
                int         nComp = dataArray.attribute( "NumberOfComponents" ).as_int( 3 );

                std::string format = dataArray.attribute( "format" ).as_string( "ascii" );
                if ( format == "ascii" )
                {
                    std::string            text = dataArray.text().as_string();
                    std::istringstream     iss( text );
                    std::vector<double>    raw;
                    double                 val;
                    while ( iss >> val )
                        raw.push_back( val );

                    if ( nComp == 3 )
                    {
                        for ( size_t i = 0; i + 2 < raw.size(); i += 3 )
                        {
                            gridData.points.emplace_back( raw[i], raw[i + 1], -raw[i + 2] );
                        }
                    }
                }
                else
                {
                    if ( errorMessages )
                        *errorMessages = QString( "Non-ASCII VTU format '%1' is not supported yet." ).arg( QString::fromStdString( format ) );
                    return false;
                }
            }
        }

        // Read cells
        auto cellsElem = piece.child( "Cells" );
        if ( cellsElem )
        {
            std::vector<int>   connectivity;
            std::vector<int>   offsets;
            std::vector<int>   cellTypes;
            std::vector<int>   faces; // optional

            for ( auto dataArray = cellsElem.child( "DataArray" ); dataArray; dataArray = dataArray.next_sibling( "DataArray" ) )
            {
                std::string name   = dataArray.attribute( "Name" ).as_string();
                std::string format = dataArray.attribute( "format" ).as_string( "ascii" );

                std::string        text = dataArray.text().as_string();
                std::istringstream iss( text );
                int                val;

                if ( name == "connectivity" )
                {
                    while ( iss >> val )
                        connectivity.push_back( val );
                }
                else if ( name == "offsets" )
                {
                    while ( iss >> val )
                        offsets.push_back( val );
                }
                else if ( name == "types" )
                {
                    while ( iss >> val )
                        cellTypes.push_back( val );
                }
                else if ( name == "faces" )
                {
                    while ( iss >> val )
                        faces.push_back( val );
                }
            }

            // Convert VTU format (offsets-based) to our flat format
            int numCells = static_cast<int>( offsets.size() );
            gridData.cellTypes.resize( numCells );

            for ( int i = 0; i < numCells; i++ )
            {
                int startOffset = ( i == 0 ) ? 0 : offsets[i - 1];
                int endOffset   = offsets[i];

                gridData.cellLocations.push_back( static_cast<int>( gridData.connectivity.size() ) );

                for ( int j = startOffset; j < endOffset; j++ )
                {
                    gridData.connectivity.push_back( connectivity[j] );
                }

                gridData.cellTypes[i] = cellTypes[i];
            }
        }

        // Read PointData
        auto pointDataElem = piece.child( "PointData" );
        if ( pointDataElem )
        {
            for ( auto dataArray = pointDataElem.child( "DataArray" ); dataArray; dataArray = dataArray.next_sibling( "DataArray" ) )
            {
                std::string name   = dataArray.attribute( "Name" ).as_string();
                std::string format = dataArray.attribute( "format" ).as_string( "ascii" );
                int         nComp  = dataArray.attribute( "NumberOfComponents" ).as_int( 1 );

                std::string        text = dataArray.text().as_string();
                std::istringstream iss( text );
                std::vector<float> values;
                float              val;
                while ( iss >> val )
                    values.push_back( val );
                gridData.pointData[name] = std::move( values );
            }
        }

        // Read CellData
        auto cellDataElem = piece.child( "CellData" );
        if ( cellDataElem )
        {
            for ( auto dataArray = cellDataElem.child( "DataArray" ); dataArray; dataArray = dataArray.next_sibling( "DataArray" ) )
            {
                std::string name   = dataArray.attribute( "Name" ).as_string();
                std::string format = dataArray.attribute( "format" ).as_string( "ascii" );
                int         nComp  = dataArray.attribute( "NumberOfComponents" ).as_int( 1 );

                std::string        text = dataArray.text().as_string();
                std::istringstream iss( text );
                std::vector<float> values;
                float              val;
                while ( iss >> val )
                    values.push_back( val );
                gridData.cellData[name] = std::move( values );
            }
        }

        return true;
    }

    //--------------------------------------------------------------------------------------------------
    /// Convert VtkGridData into RigEclipseCaseData
    /// Creates a synthetic corner-point grid from the VTK unstructured grid.
    /// Each VTK cell becomes one Eclipse cell with its bounding box as geometry.
    //--------------------------------------------------------------------------------------------------
    static bool populateEclipseCaseData( const VtkGridData& gridData, RigEclipseCaseData* eclipseCase, QString* errorMessages )
    {
        if ( gridData.points.empty() )
        {
            if ( errorMessages ) *errorMessages = "No points found in VTK file.";
            return false;
        }

        if ( gridData.cellTypes.empty() )
        {
            if ( errorMessages ) *errorMessages = "No cells found in VTK file.";
            return false;
        }

        // Count only volumetric cells (3D cells)
        int numVolumetricCells = 0;
        std::vector<int> volumetricCellIndices;
        for ( size_t i = 0; i < gridData.cellTypes.size(); i++ )
        {
            int cornerCount = vtkCellCornerCount( gridData.cellTypes[i] );
            if ( cornerCount == 4 || cornerCount == 5 || cornerCount == 6 || cornerCount == 8 )
            {
                volumetricCellIndices.push_back( static_cast<int>( i ) );
                numVolumetricCells++;
            }
        }

        if ( numVolumetricCells == 0 )
        {
            if ( errorMessages ) *errorMessages = "No volumetric cells found in VTK file.";
            return false;
        }

    // Compute the overall bounding box of all VTK cell centroids to determine
    // a reasonable 3D grid layout. We try to find unique positions along each axis.
    struct CellInfo
    {
        cvf::Vec3d centroid;
        cvf::Vec3d bbMin;
        cvf::Vec3d bbMax;
    };

    std::vector<CellInfo> cellInfos( numVolumetricCells );
    for ( int i = 0; i < numVolumetricCells; i++ )
    {
        int vtkCellIdx = volumetricCellIndices[i];
        int location   = gridData.cellLocations[vtkCellIdx];
        int numCorners = ( static_cast<size_t>( vtkCellIdx + 1 ) < gridData.cellLocations.size() )
                             ? ( gridData.cellLocations[vtkCellIdx + 1] - location )
                             : ( static_cast<int>( gridData.connectivity.size() ) - location );

        cvf::BoundingBox bb;
        for ( int j = 0; j < numCorners; j++ )
        {
            int        nodeIdx = gridData.connectivity[location + j];
            cvf::Vec3d pos     = gridData.points[nodeIdx];
            bb.add( pos );
        }

        cellInfos[i].centroid = bb.center();
        cellInfos[i].bbMin    = bb.min();
        cellInfos[i].bbMax    = bb.max();
    }

    // Determine grid dimensions by finding unique centroid positions along each axis.
    // Use a tolerance-based approach since VTK coordinates may have floating-point variations.
    const double posTol = 1e-4;

    auto countUnique = [posTol]( const std::vector<double>& values ) -> size_t
    {
        if ( values.empty() ) return 1;
        std::vector<double> sorted = values;
        std::sort( sorted.begin(), sorted.end() );
        size_t count = 1;
        for ( size_t i = 1; i < sorted.size(); i++ )
        {
            if ( std::fabs( sorted[i] - sorted[i - 1] ) > posTol ) count++;
        }
        return count;
    };

    std::vector<double> xVals, yVals, zVals;
    xVals.reserve( numVolumetricCells );
    yVals.reserve( numVolumetricCells );
    zVals.reserve( numVolumetricCells );

    for ( int i = 0; i < numVolumetricCells; i++ )
    {
        xVals.push_back( cellInfos[i].centroid.x() );
        yVals.push_back( cellInfos[i].centroid.y() );
        zVals.push_back( cellInfos[i].centroid.z() );
    }

    // Compute nx, ny, nz from unique centroid positions, with sensible defaults
    size_t nx = countUnique( xVals );
    size_t ny = countUnique( yVals );
    size_t nz = countUnique( zVals );

    // Ensure the total cell count matches
    // If the cell count doesn't match nx*ny*nz (e.g. irregular mesh), fall back to a 1D layout
    if ( nx * ny * nz != static_cast<size_t>( numVolumetricCells ) )
    {
        // Try to find better dimensions by factoring the cell count
        size_t n = static_cast<size_t>( numVolumetricCells );
        nx = static_cast<size_t>( std::cbrt( (double)n ) + 0.5 );
        if ( nx < 1 ) nx = 1;
        ny = nx;
        nz = n / ( nx * ny );
        if ( nz * nx * ny < n ) nz++;
        // Fallback: use the unique-count approach anyway for best visual layout
        if ( nx > n ) nx = n;
        if ( ny > n / nx ) ny = n / nx;
        if ( nz < 1 ) nz = 1;
    }

    RigMainGrid* mainGrid = eclipseCase->mainGrid();

    // Set cell counts on the main grid
    mainGrid->setCellCounts( cvf::Vec3st( nx, ny, nz ) );

    // Set up active cell info
    RigActiveCellInfo* activeCellInfo = eclipseCase->activeCellInfo( RiaDefines::PorosityModelType::MATRIX_MODEL );
    activeCellInfo->setGridCount( 1 );
    activeCellInfo->setReservoirCellCount( nx * ny * nz );

    // Allocate cells in the main grid's cell storage
    size_t totalCellCount = nx * ny * nz;
    mainGrid->allocateCells( totalCellCount );

    // Build a shared node grid: each VTK cell's corners become permanent nodes.
    // To share nodes between adjacent cells, we use a map from position to node index.
    struct Vec3Compare
    {
        bool operator()( const cvf::Vec3d& a, const cvf::Vec3d& b ) const
        {
            double eps = 1e-6;
            if ( std::fabs( a.x() - b.x() ) > eps ) return a.x() < b.x();
            if ( std::fabs( a.y() - b.y() ) > eps ) return a.y() < b.y();
            return a.z() < b.z() - eps;
        }
    };
    std::map<cvf::Vec3d, size_t, Vec3Compare> nodeMap;

    // Sort cells so they map to IJK order: all cells are placed at their centroid positions
    // within the grid. We sort by I (fastest), then J, then K (slowest).
    // To handle cases where the cell count doesn't perfectly match the grid, we clamp.
    std::vector<int> cellOrder( numVolumetricCells );
    for ( int i = 0; i < numVolumetricCells; i++ )
        cellOrder[i] = i;

    std::sort( cellOrder.begin(), cellOrder.end(), [&cellInfos]( int a, int b ) {
        const auto& ca = cellInfos[a].centroid;
        const auto& cb = cellInfos[b].centroid;
        if ( std::fabs( ca.z() - cb.z() ) > 1e-6 ) return ca.z() < cb.z();
        if ( std::fabs( ca.y() - cb.y() ) > 1e-6 ) return ca.y() < cb.y();
        return ca.x() < cb.x();
    } );

    // Create cells in IJK order
    size_t cellIdx         = 0;
    int    cellsToProcess  = std::min( numVolumetricCells, static_cast<int>( totalCellCount ) );
    int    cellsRemaining  = cellsToProcess;

    for ( size_t k = 0; k < nz && cellsRemaining > 0; k++ )
    {
        for ( size_t j = 0; j < ny && cellsRemaining > 0; j++ )
        {
            for ( size_t i = 0; i < nx && cellsRemaining > 0; i++, cellsRemaining--, cellIdx++ )
            {
                int vtkCellIdx = volumetricCellIndices[cellOrder[cellIdx]];

                int location = gridData.cellLocations[vtkCellIdx];
                int numCorners;
                if ( static_cast<size_t>( vtkCellIdx + 1 ) < gridData.cellLocations.size() )
                {
                    numCorners = gridData.cellLocations[vtkCellIdx + 1] - location;
                }
                else
                {
                    numCorners = static_cast<int>( gridData.connectivity.size() ) - location;
                }

                const auto& info = cellInfos[cellOrder[cellIdx]];
                cvf::Vec3d  cellMin = info.bbMin;
                cvf::Vec3d  cellMax = info.bbMax;

                // Use the actual VTK cell bounding box corners directly
                // Map VTK corner ordering to ResInsight hex corner ordering:
                // 0=min-min-min, 1=max-min-min, 2=max-max-min, 3=min-max-min,
                // 4=min-min-max, 5=max-min-max, 6=max-max-max, 7=min-max-max
                std::array<cvf::Vec3d, 8> corners;
                corners[0] = cvf::Vec3d( cellMin.x(), cellMin.y(), cellMin.z() );
                corners[1] = cvf::Vec3d( cellMax.x(), cellMin.y(), cellMin.z() );
                corners[2] = cvf::Vec3d( cellMax.x(), cellMax.y(), cellMin.z() );
                corners[3] = cvf::Vec3d( cellMin.x(), cellMax.y(), cellMin.z() );
                corners[4] = cvf::Vec3d( cellMin.x(), cellMin.y(), cellMax.z() );
                corners[5] = cvf::Vec3d( cellMax.x(), cellMin.y(), cellMax.z() );
                corners[6] = cvf::Vec3d( cellMax.x(), cellMax.y(), cellMax.z() );
                corners[7] = cvf::Vec3d( cellMin.x(), cellMax.y(), cellMax.z() );

                // Look up or create shared nodes
                size_t nodeStartIdx = mainGrid->nodes().size();
                for ( int c = 0; c < 8; c++ )
                {
                    auto it = nodeMap.find( corners[c] );
                    if ( it != nodeMap.end() )
                    {
                        // Use existing shared node
                        // Need to re-index: we store the mapping for later use
                    }
                    else
                    {
                        nodeMap[corners[c]] = mainGrid->nodes().size();
                        mainGrid->nodes().push_back( corners[c] );
                    }
                }

                // Set up the cell
                RigCell& cell = mainGrid->cell( cellIdx );
                cell.setGridLocalCellIndex( cellIdx );

                // Set corner indices using shared nodes
                for ( int c = 0; c < 8; c++ )
                {
                    cell.cornerIndices()[c] = nodeMap[corners[c]];
                }

                // Mark as active
                activeCellInfo->setCellResultIndex( ReservoirCellIndex( cellIdx ), ActiveCellIndex( cellIdx ) );
            }
        }
    }

    activeCellInfo->setGridActiveCellCounts( 0, totalCellCount );
    activeCellInfo->computeDerivedData();

    // Store cell data as results if available
    if ( !gridData.cellData.empty() )
    {
        RigCaseCellResultsData* resultsData = eclipseCase->results( RiaDefines::PorosityModelType::MATRIX_MODEL );

        for ( const auto& [dataName, values] : gridData.cellData )
        {
            if ( static_cast<int>( values.size() ) >= numVolumetricCells )
            {
                std::vector<double> doubleValues( values.begin(), values.begin() + numVolumetricCells );
                resultsData->addStaticScalarResult( RiaDefines::ResultCatType::INPUT_PROPERTY,
                                                    QString::fromStdString( dataName ),
                                                    true,
                                                    numVolumetricCells );
                auto* resultData = resultsData->modifiableCellScalarResult( RigEclipseResultAddress( RiaDefines::ResultCatType::INPUT_PROPERTY,
                                                                                                      QString::fromStdString( dataName ) ),
                                                                            0 );
                if ( resultData )
                {
                    *resultData = std::move( doubleValues );
                }
            }
        }
    }

    // Store point data as results (averaged to cells) if available
    if ( !gridData.pointData.empty() )
    {
        for ( const auto& [dataName, values] : gridData.pointData )
        {
            std::vector<double> cellValues( numVolumetricCells, 0.0 );

            for ( int i = 0; i < numVolumetricCells; i++ )
            {
                int vtkCellIdx = volumetricCellIndices[i];
                int location   = gridData.cellLocations[vtkCellIdx];
                int numCorners = ( static_cast<size_t>( vtkCellIdx + 1 ) < gridData.cellLocations.size() )
                                     ? ( gridData.cellLocations[vtkCellIdx + 1] - location )
                                     : ( static_cast<int>( gridData.connectivity.size() ) - location );

                double sum = 0.0;
                for ( int j = 0; j < numCorners; j++ )
                {
                    int nodeIdx = gridData.connectivity[location + j];
                    if ( static_cast<size_t>( nodeIdx ) < values.size() )
                    {
                        sum += values[nodeIdx];
                    }
                }
                cellValues[i] = sum / numCorners;
            }

            RigCaseCellResultsData* resultsData = eclipseCase->results( RiaDefines::PorosityModelType::MATRIX_MODEL );
            resultsData->addStaticScalarResult( RiaDefines::ResultCatType::INPUT_PROPERTY,
                                                QString::fromStdString( dataName ) + "_point_avg",
                                                true,
                                                numVolumetricCells );
            auto* resultData = resultsData->modifiableCellScalarResult(
                RigEclipseResultAddress( RiaDefines::ResultCatType::INPUT_PROPERTY,
                                         QString::fromStdString( dataName ) + "_point_avg" ),
                0 );
            if ( resultData )
            {
                *resultData = std::move( cellValues );
            }
        }
    }

    // Set case info
    mainGrid->setFlipAxis( false, false );
    mainGrid->initAllSubGridsParentGridPointer();

        return true;
    }

} // namespace internal

//--------------------------------------------------------------------------------------------------
///
//--------------------------------------------------------------------------------------------------
bool RifReaderVtkGrid::openFile( const QString& fileName, RigEclipseCaseData* eclipseCaseData, QString* errorMessages )
{
    std::string filePath = fileName.toStdString();

    internal::VtkGridData gridData;

    bool parsed = false;

    // Detect file type by extension
    if ( fileName.endsWith( ".vtu", Qt::CaseInsensitive ) )
    {
        parsed = internal::parseVtu( filePath, gridData, errorMessages );
    }
    else if ( fileName.endsWith( ".vtk", Qt::CaseInsensitive ) )
    {
        parsed = internal::parseLegacyVtk( filePath, gridData, errorMessages );
    }
    else
    {
        // Try XML first, fall back to legacy
        parsed = internal::parseVtu( filePath, gridData, errorMessages );
        if ( !parsed )
        {
            parsed = internal::parseLegacyVtk( filePath, gridData, errorMessages );
        }
    }

    if ( !parsed ) return false;

    RiaLogging::info( std::format( "VTK file parsed: {} points, {} cells", gridData.points.size(), gridData.cellTypes.size() ) );

    return internal::populateEclipseCaseData( gridData, eclipseCaseData, errorMessages );
}
