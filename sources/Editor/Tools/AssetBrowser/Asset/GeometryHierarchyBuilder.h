/*
 * Editor geometry hierarchy builder header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_GEOMETRY_HIERARCHY_BUILDER_H__
#define __FORK_EDITOR_GEOMETRY_HIERARCHY_BUILDER_H__


#include "Scene/Node/SceneVisitor.h"
#include "../../../Core/UIExtensions/TreeViewBuilder.h"

#include <memory>


namespace Fork
{

namespace Scene { class Geometry; }

namespace Editor
{


class UITreeView;

//! Hierarchy builder for scene geometries.
class GeometryHierarchyBuilder : private Scene::SceneVisitor
{
    
    public:
        
        /**
        Builds a geometry hierarchy of the specified geometry into the specified tree view.
        \throws NullPointerException If 'treeView' is null or 'geometry' is null.
        */
        void BuildHierarchy(UITreeView* treeView, Scene::Geometry* geometry);
        /**
        Builds a geometry hierarchy of the specified geometry into the specified tree view.
        \throws NullPointerException If 'treeView' is null or 'geometry' is null.
        \throws InvalidArgumentException If 'entryPoint' is invalid.
        */
        void BuildHierarchy(UITreeView* treeView, Scene::Geometry* geometry, const wxTreeItemId& entryPoint);

    private:

        void VisitLODGeometry               ( Scene::LODGeometry*               node );
        void VisitBillboardGeometry         ( Scene::BillboardGeometry*         node );
        void VisitMeshGeometry              ( Scene::MeshGeometry*              node );
        void VisitCompositionGeometry       ( Scene::CompositionGeometry*       node );
        void VisitTexturedGeometry          ( Scene::TexturedGeometry*          node );
        void VisitSimple2DMeshGeometry      ( Scene::Simple2DMeshGeometry*      node );
        void VisitSimple3DMeshGeometry      ( Scene::Simple3DMeshGeometry*      node );
        void VisitTangentSpaceMeshGeometry  ( Scene::TangentSpaceMeshGeometry*  node );
        void VisitCommonMeshGeometry        ( Scene::CommonMeshGeometry*        node );

        std::unique_ptr<TreeViewBuilder> treeViewBuilder_;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================