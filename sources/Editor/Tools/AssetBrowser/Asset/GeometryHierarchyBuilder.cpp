/*
 * Editor geometry hierarchy builder file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GeometryHierarchyBuilder.h"
#include "../../../Core/UIExtensions/UITreeView.h"
#include "Core/Exception/NullPointerException.h"
#include "../../../../Scene/Geometry/ImportGeometries.h"
#include "Core/StringModifier.h"


namespace Fork
{

namespace Editor
{


void GeometryHierarchyBuilder::BuildHierarchy(UITreeView* treeView, Scene::Geometry* geometry)
{   
    ASSERT_POINTER(geometry);
    ASSERT_POINTER(treeView);
    
    /* Initialize tree view builder */
    treeViewBuilder_ = std::make_unique<TreeViewBuilder>(treeView);
    {
        geometry->Visit(this);
        treeView->ExpandAll();
    }
    treeViewBuilder_.reset();
}

void GeometryHierarchyBuilder::BuildHierarchy(UITreeView* treeView, Scene::Geometry* geometry, const wxTreeItemId& entryPoint)
{
    ASSERT_POINTER(geometry);
    ASSERT_POINTER(treeView);
    
    /* Initialize tree view builder */
    treeViewBuilder_ = std::make_unique<TreeViewBuilder>(treeView, entryPoint);
    {
        geometry->Visit(this);
    }
    treeViewBuilder_.reset();
}


/*
 * ======= Private: =======
 */

void GeometryHierarchyBuilder::VisitLODGeometry(Scene::LODGeometry* node)
{
    treeViewBuilder_->AddItem(
        "LOD Geometry (" + ToStr(node->lodGeometries.size()) + " Sub Geometries)"
    );
    treeViewBuilder_->PushParent();

    for (const auto& subGeom : node->lodGeometries)
        subGeom->Visit(this);

    treeViewBuilder_->PopParent();
}

void GeometryHierarchyBuilder::VisitBillboardGeometry(Scene::BillboardGeometry* node)
{
    treeViewBuilder_->AddItem("Billboard Geometry");
}

void GeometryHierarchyBuilder::VisitMeshGeometry(Scene::MeshGeometry* node)
{
    treeViewBuilder_->AddItem(
        "Mesh Geometry (" +
        ToStr(node->NumVertices()) + " Vertices, " +
        ToStr(node->NumIndices()) + " Indices)"
    );
}

void GeometryHierarchyBuilder::VisitCompositionGeometry(Scene::CompositionGeometry* node)
{
    treeViewBuilder_->AddItem(
        "Composition Geometry (" + ToStr(node->subGeometries.size()) + " Sub Geometries)"
    );

    treeViewBuilder_->PushParent();
    {
        for (const auto& subGeom : node->subGeometries)
            subGeom->Visit(this);
    }
    treeViewBuilder_->PopParent();
}

void GeometryHierarchyBuilder::VisitTexturedGeometry(Scene::TexturedGeometry* node)
{
    treeViewBuilder_->AddItem(
        "Textured Geometry (" + ToStr(node->textures.size()) + " Textures)"
    );

    treeViewBuilder_->PushParent();
    {
        node->actualGeometry->Visit(this);
    }
    treeViewBuilder_->PopParent();
}

void GeometryHierarchyBuilder::VisitSimple2DMeshGeometry(Scene::Simple2DMeshGeometry* node)
{
    treeViewBuilder_->AddItem(
        "Simple 2D Mesh Geometry (" +
        ToStr(node->NumVertices()) + " Vertices, " +
        ToStr(node->NumIndices()) + " Indices)"
    );
}

void GeometryHierarchyBuilder::VisitSimple3DMeshGeometry(Scene::Simple3DMeshGeometry* node)
{
    treeViewBuilder_->AddItem(
        "Simple 3D Mesh Geometry (" +
        ToStr(node->NumVertices()) + " Vertices, " +
        ToStr(node->NumIndices()) + " Indices)"
    );
}

void GeometryHierarchyBuilder::VisitTangentSpaceMeshGeometry(Scene::TangentSpaceMeshGeometry* node)
{
    treeViewBuilder_->AddItem(
        "Tangent-Space Mesh Geometry (" +
        ToStr(node->NumVertices()) + " Vertices, " +
        ToStr(node->NumIndices()) + " Indices)"
    );
}

void GeometryHierarchyBuilder::VisitCommonMeshGeometry(Scene::CommonMeshGeometry* node)
{
    treeViewBuilder_->AddItem(
        "Common Mesh Geometry (" +
        ToStr(node->NumVertices()) + " Vertices, " +
        ToStr(node->NumIndices()) + " Indices)"
    );
}


} // /namespace Editor

} // /namespace Fork



// ========================