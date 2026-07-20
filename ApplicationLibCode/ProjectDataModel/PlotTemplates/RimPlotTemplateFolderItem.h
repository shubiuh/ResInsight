////////////////////////////////////////////////////////////////////////////////
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
/// @brief Recursively discovered folder tree of summary plot templates.

#pragma once

#include "cafPdmChildArrayField.h"
#include "cafPdmField.h"
#include "cafPdmObject.h"

namespace caf
{
class PdmUiEditorAttribute;
}

class RimPlotTemplateFileItem;

//==================================================================================================
/// @brief Owns folder and file items discovered beneath configured template roots.
///
/// Scanning recognizes `.rpt` and `.erpt` files and is bounded by the preference recursion depth.
/// The same owned tree feeds both the project browser and hierarchical PDM option menus. Rescanning
/// replaces existing children so deleted files and preference changes cannot leave stale entries.
//==================================================================================================
class RimPlotTemplateFolderItem : public caf::PdmObject
{
    CAF_PDM_HEADER_INIT;

public:
    /// Initializes an empty folder node.
    RimPlotTemplateFolderItem();
    ~RimPlotTemplateFolderItem() override;

    /// Rebuilds root children from @p folderPaths using the configured maximum depth.
    void createRootFolderItemsFromFolderPaths( const QStringList& folderPaths );

    /// @return Directly owned template files.
    std::vector<RimPlotTemplateFileItem*>   fileItems() const;
    /// @return Directly owned subfolders.
    std::vector<RimPlotTemplateFolderItem*> subFolders() const;

    /// Appends a hierarchical menu representation of @p templateFolderItem to @p options.
    static void appendOptionItemsForPlotTemplates( QList<caf::PdmOptionItemInfo>& options, RimPlotTemplateFolderItem* templateFolderItem );

    /// Flattens every template beneath @p templateFolderItem into @p fileItems.
    static void allPlotTemplates( std::vector<RimPlotTemplateFileItem*>& fileItems, RimPlotTemplateFolderItem* templateFolderItem );
    /// Recursively refreshes default-template icon overlays.
    void        updateIconState() const;

private:
    /// Replaces children with files and readable subdirectories, bounded by @p levelsLeft.
    void searchForFileAndFolderNames( int levelsLeft );
    /// Stores @p path and derives this node's display name.
    void setFolderPath( const QString& path );
    /// Creates and scans one owned folder node for each path.
    void createSubFolderItemsFromFolderPaths( const QStringList& folderPaths, int levelsLeft );

    void defineEditorAttribute( const caf::PdmFieldHandle* field, QString uiConfigName, caf::PdmUiEditorAttribute* attribute ) override;
    void defineUiOrdering( QString uiConfigName, caf::PdmUiOrdering& uiOrdering ) override;

    /// Recursive worker that preserves folder nesting through option levels and headers.
    static void appendOptionItemsForPlotTemplatesRecursively( QList<caf::PdmOptionItemInfo>& options,
                                                              RimPlotTemplateFolderItem*     templateFolderItem,
                                                              int                            menuLevel );

private:
    /// Absolute path represented by this folder node; empty for the synthetic root.
    caf::PdmField<caf::FilePath>                        m_folderName;
    /// Owned templates found directly in this folder.
    caf::PdmChildArrayField<RimPlotTemplateFileItem*>   m_fileNames;
    /// Owned readable subfolders within the configured depth.
    caf::PdmChildArrayField<RimPlotTemplateFolderItem*> m_subFolders;
};
