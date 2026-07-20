/////////////////////////////////////////////////////////////////////////////////
//
//  Copyright (C) Statoil ASA
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
/// Defines the canonical, sortable identity of an Eclipse summary vector.

#pragma once

#include <compare>
#include <cstdint>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "RifEclipseSummaryAddressDefines.h"
using namespace RifEclipseSummaryAddressDefines;

class QTextStream;
class QString;

//==================================================================================================
/// Canonical address for a simulator summary vector.
///
/// An address combines a vector keyword with the category-specific identifiers
/// needed to locate it, such as a well, group, region pair, segment, or grid cell.
/// Frequently repeated strings are stored as indices in a shared string pool to
/// keep large ensemble address sets compact. Declaration order of the data members
/// intentionally defines the default three-way comparison and therefore set order.
//==================================================================================================
class RifEclipseSummaryAddress
{
public:
    /// Creates an invalid address used as a not-found sentinel.
    RifEclipseSummaryAddress();

    /// Creates an address from all normalized components.
    RifEclipseSummaryAddress( SummaryCategory    category,
                              StatisticsType     statisticsType,
                              const std::string& vectorName,
                              int                regionNumber,
                              int                regionNumber2,
                              const std::string& groupName,
                              const std::string& networkName,
                              const std::string& wellName,
                              int                wellSegmentNumber,
                              const std::string& lgrName,
                              int                cellI,
                              int                cellJ,
                              int                cellK,
                              int                aquiferNumber,
                              int                completionNumber,
                              bool               isErrorResult,
                              int                id );

    /// Creates a category-specific address from an identifier map.
    RifEclipseSummaryAddress( SummaryCategory category, std::map<SummaryIdentifierType, std::string>& identifiers );

    /// @name Parsing and specialized factories
    /// These helpers establish the correct category and identifier layout, avoiding
    /// partially initialized addresses at call sites.
    /// @{

    /// Parses the canonical colon-separated Eclipse text representation.
    static RifEclipseSummaryAddress fromEclipseTextAddress( const std::string& textAddress );
    /// Parses a text address while retaining tokens produced by malformed input for diagnostics.
    static RifEclipseSummaryAddress fromEclipseTextAddressParseErrorTokens( const std::string& textAddress );

    /// Creates a field-level vector address.
    static RifEclipseSummaryAddress fieldAddress( const std::string& vectorName, int calculationId = -1 );
    /// Creates an aquifer-level vector address.
    static RifEclipseSummaryAddress aquiferAddress( const std::string& vectorName, int aquiferNumber, int calculationId = -1 );
    /// Creates a network-level vector address.
    static RifEclipseSummaryAddress networkAddress( const std::string& vectorName, const std::string& networkName, int calculationId = -1 );
    /// Creates a miscellaneous/global vector address.
    static RifEclipseSummaryAddress miscAddress( const std::string& vectorName, int calculationId = -1 );
    /// Creates a region-level vector address.
    static RifEclipseSummaryAddress regionAddress( const std::string& vectorName, int regionNumber, int calculationId = -1 );
    /// Creates a region-to-region flow vector address.
    static RifEclipseSummaryAddress
        regionToRegionAddress( const std::string& vectorName, int regionNumber, int region2Number, int calculationId = -1 );
    /// Creates a group-level vector address.
    static RifEclipseSummaryAddress groupAddress( const std::string& vectorName, const std::string& groupName, int calculationId = -1 );
    /// Creates a well-level vector address.
    static RifEclipseSummaryAddress wellAddress( const std::string& vectorName, const std::string& wellName, int calculationId = -1 );
    /// Creates a completion-number vector address.
    static RifEclipseSummaryAddress
        wellCompletionAddress( const std::string& vectorName, const std::string& wellName, int completionNumber, int calculationId = -1 );

    /// Creates a well-connection address from one-based Eclipse IJK coordinates.
    static RifEclipseSummaryAddress
        wellConnectionAddress( const std::string& vectorName, const std::string& wellName, int i, int j, int k, int calculationId = -1 );
    /// Creates a well vector associated with a local grid refinement.
    static RifEclipseSummaryAddress
        wellLgrAddress( const std::string& vectorName, const std::string& lgrName, const std::string& wellName, int calculationId = -1 );
    /// Creates a well-completion address inside a local grid refinement.
    static RifEclipseSummaryAddress wellCompletionLgrAddress( const std::string& vectorName,
                                                              const std::string& lgrName,
                                                              const std::string& wellName,
                                                              int                i,
                                                              int                j,
                                                              int                k,
                                                              int                calculationId = -1 );
    /// Creates a multisegment-well vector address.
    static RifEclipseSummaryAddress
        wellSegmentAddress( const std::string& vectorName, const std::string& wellName, int segmentNumber, int calculationId = -1 );
    /// Creates a main-grid block address from one-based Eclipse IJK coordinates.
    static RifEclipseSummaryAddress blockAddress( const std::string& vectorName, int i, int j, int k, int calculationId = -1 );
    /// Creates a local-grid block address.
    static RifEclipseSummaryAddress
        blockLgrAddress( const std::string& vectorName, const std::string& lgrName, int i, int j, int k, int calculationId = -1 );
    /// Creates an address for externally imported or calculated data.
    static RifEclipseSummaryAddress importedAddress( const std::string& vectorName, int calculationId = -1 );

    /// Creates the special address representing time as an x-axis vector.
    static RifEclipseSummaryAddress timeAddress();
    /// @}

    /// Joins canonical text representations of @p addressVector with @p jointString.
    static std::string generateStringFromAddresses( const std::vector<RifEclipseSummaryAddress>& addressVector,
                                                    const std::string                            jointString = "; " );

    /// Returns whether @p category requires a well-name identifier.
    static bool isDependentOnWellName( SummaryCategory category );

    /// @name Address components
    /// @{

    SummaryCategory category() const;        ///< @return Summary object category.
    std::string     vectorName() const;      ///< @return Keyword/vector name from the string pool.
    bool            isHistoryVector() const; ///< @return Whether the keyword denotes historical data.

    bool           isStatistics() const;                       ///< @return Whether this address represents an ensemble statistic.
    StatisticsType statisticsType() const;                     ///< @return Statistic kind.
    void           setStatisticsType( StatisticsType type );   ///< Assigns the statistic kind.
    int            percentile() const;                         ///< @return Percentile in the inclusive 0–100 range.
    void           setPercentile( int percentile );            ///< Assigns a percentile, subject to the supported range.

    int regionNumber() const;  ///< @return Primary region identifier.
    int regionNumber2() const; ///< @return Secondary region identifier for region-pair vectors.

    std::string groupName() const;           ///< @return Group identifier when applicable.
    std::string networkName() const;         ///< @return Network identifier when applicable.
    std::string wellName() const;            ///< @return Well identifier when applicable.
    int         wellCompletionNumber() const; ///< @return Completion number when applicable.
    int         wellSegmentNumber() const;    ///< @return Segment number when applicable.
    std::string lgrName() const;              ///< @return Local-grid-refinement name.
    int         cellI() const;                ///< @return One-based Eclipse I coordinate.
    int         cellJ() const;                ///< @return One-based Eclipse J coordinate.
    int         cellK() const;                ///< @return One-based Eclipse K coordinate.
    int         aquiferNumber() const;        ///< @return Aquifer identifier when applicable.
    int         id() const;                   ///< @return Calculation/import identifier, or `-1` for native data.
    std::string blockAsString() const;        ///< Formats the block IJK component.
    std::string connectionAsString() const;   ///< Formats the well-connection component.
    /// @}

    /// Serializes the address into its canonical Eclipse-style text form.
    std::string toEclipseTextAddress() const;

    /// @name UI representation and validation
    /// @{

    std::string uiText() const;     ///< @return Full human-readable address label.
    std::string itemUiText() const; ///< @return Category-specific object label without redundant vector context.
    std::string addressComponentUiText( SummaryIdentifierType itemTypeInput ) const; ///< Formats one identifier component.
    bool isUiTextMatchingFilterText( const QString& filterString ) const; ///< Applies the summary-browser text filter.

    bool isValid() const; ///< @return Whether all identifiers required by the category are populated.
    /// @}

    /// @name Component mutation
    /// Mutators preserve the compact pooled-string representation.
    /// @{
    void setVectorName( const std::string& vectorName );
    void setWellName( const std::string& wellName );
    void setGroupName( const std::string& groupName );
    void setNetworkName( const std::string& networkName );
    void setRegion( int region );
    void setRegion2( int region2 );
    void setAquiferNumber( int aquiferNumber );
    void setCellIjk( const std::string& uiText );
    void setWellSegmentNumber( int segment );
    void setWellCompletionNumber( int completionNumber );
    /// @}

    void setAsErrorResult();        ///< Marks the vector as an uncertainty/error result.
    bool isErrorResult() const;     ///< @return Whether the error-result flag is set.

    void setId( int id ); ///< Assigns the calculated/imported vector identifier.

    bool hasAccumulatedData() const; ///< @return Whether keyword semantics indicate a cumulative quantity.

    /// Removes history/statistics decorations to obtain the underlying keyword.
    static std::string baseVectorName( const std::string& vectorName );

    /// Compares all stored components in declaration order.
    auto operator<=>( const RifEclipseSummaryAddress& rhs ) const = default;

    bool isCalculated() const; ///< @return Whether the address refers to an imported/calculated vector.
    bool isTime() const;       ///< @return Whether this is the special time-axis address.

    std::string formatUiTextRegionToRegion() const; ///< Formats the two region identifiers for display.
    static std::pair<int, int> regionToRegionPairFromUiText( const std::string& s ); ///< Parses a displayed region pair.

    // Percentile constants
    static constexpr int MIN_PERCENTILE = 0;
    static constexpr int MAX_PERCENTILE = 100;

private:
    /// Builds an address from tokenized canonical text.
    static RifEclipseSummaryAddress fromTokens( const std::vector<std::string>& tokens );

    bool isValidEclipseCategory() const; ///< Validates category-specific Eclipse identifiers.
    static std::tuple<int, int, int> ijkTupleFromUiText( const std::string& s ); ///< Parses an IJK text component.
    void                             setCellIjk( std::tuple<int, int, int> ijk );
    void                             setCellIjk( int i, int j, int k );

    void setVectorNameIdx( const std::string& str ); ///< Interns the vector keyword.
    void setNameIdx( const std::string& str );       ///< Interns the category-dependent object name.
    void setLgrNameIdx( const std::string& str );    ///< Interns the local-grid name.

private:
    // The ordering the variables are defined in defines how the objects get sorted. Members defined first will be
    // evaluated first. This concept is used by <=> operator.

    SummaryCategory m_category;       ///< Determines how the generic name/number slots are interpreted.
    StatisticsType  m_statisticsType; ///< Ensemble statistic represented by the address.
    uint32_t m_vectorNameIdx;          ///< Shared-string-pool index for the vector keyword.
    uint32_t m_nameIdx;                ///< Shared-string-pool index for well/group/network name.
    uint32_t m_lgrNameIdx;             ///< Shared-string-pool index for the local-grid name.
    int m_number0;                     ///< Category-dependent first numeric identifier.
    int m_number1;                     ///< Category-dependent second numeric identifier.
    int m_number2;                     ///< Category-dependent third numeric identifier.
    bool m_isErrorResult;              ///< Distinguishes an error vector from its primary result.
    int  m_id;                         ///< Calculation/import identity; native results normally use `-1`.
    int  m_percentile;                 ///< Percentile value for percentile statistics.

#ifdef _DEBUG
    // Debug-only strings mirroring the uint32_t pool indices for easier inspection in debugger.
    // Wrapped in a struct with trivial comparison so the defaulted <=> is unaffected.
    struct DebugString
    {
        std::string value;
        auto        operator<=>( const DebugString& ) const { return std::strong_ordering::equal; }
        bool        operator==( const DebugString& ) const { return true; }
    };

    DebugString m_vectorNameDbg;
    DebugString m_nameDbg;
    DebugString m_lgrNameDbg;
#endif
};

/// Writes the canonical text form to a Qt stream.
QTextStream& operator<<( QTextStream& str, const RifEclipseSummaryAddress& sobj );
/// Reads a canonical text form from a Qt stream.
QTextStream& operator>>( QTextStream& str, RifEclipseSummaryAddress& sobj );
