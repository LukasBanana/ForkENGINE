/*
 * Scene visitor header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SCENE_VISITOR_H__
#define __FORK_SCENE_VISITOR_H__


#include "Scene/Node/SceneNodeDecl.h"
#include "Core/Export.h"


namespace Fork
{

namespace Scene
{


#define DEF_DUMMY_VISIT_PROC(n)     \
    virtual void Visit##n(n* node)  \
    {                               \
        /* Dummy */                 \
    }

/**
Declares the scene visitor interface for all scene nodes.
\see SceneVisitor
*/
#define DECL_SCENE_VISITOR_NODE_INTERFACE                       \
    void VisitSceneNode     ( SceneNode*    node ) override;    \
    void VisitGeometryNode  ( GeometryNode* node ) override;    \
    void VisitLightNode     ( LightNode*    node ) override;    \
    void VisitCameraNode    ( CameraNode*   node ) override;

/**
Declares the scene visitor interface for all geometry nodes.
\see SceneVisitor
*/
#define DECL_SCENE_VISITOR_GEOMETRY_INTERFACE                                           \
    void VisitLODGeometry               ( LODGeometry*              node ) override;    \
    void VisitBillboardGeometry         ( BillboardGeometry*        node ) override;    \
    void VisitTerrainGeometry           ( TerrainGeometry*          node ) override;    \
    void VisitMeshGeometry              ( MeshGeometry*             node ) override;    \
    void VisitCompositionGeometry       ( CompositionGeometry*      node ) override;    \
    void VisitTexturedGeometry          ( TexturedGeometry*         node ) override;    \
    void VisitSimple2DMeshGeometry      ( Simple2DMeshGeometry*     node ) override;    \
    void VisitSimple3DMeshGeometry      ( Simple3DMeshGeometry*     node ) override;    \
    void VisitTangentSpaceMeshGeometry  ( TangentSpaceMeshGeometry* node ) override;    \
    void VisitCommonMeshGeometry        ( CommonMeshGeometry*       node ) override;    \
    void VisitTerrainMeshGeometry       ( TerrainMeshGeometry*      node ) override;

/**
Declares the entire scene visitor interface.
\see SceneVisitor
*/
#define DECL_SCENE_VISITOR_INTERFACE        \
    DECL_SCENE_VISITOR_NODE_INTERFACE       \
    DECL_SCENE_VISITOR_GEOMETRY_INTERFACE


//! Scene visitor interface (Visitor Pattern).
class FORK_EXPORT SceneVisitor
{
    
    public:
        
        virtual ~SceneVisitor()
        {
        }

        /* === Interface === */

        DEF_DUMMY_VISIT_PROC( SceneNode                 )
        DEF_DUMMY_VISIT_PROC( GeometryNode              )
        DEF_DUMMY_VISIT_PROC( LightNode                 )
        DEF_DUMMY_VISIT_PROC( CameraNode                )

        DEF_DUMMY_VISIT_PROC( LODGeometry               )
        DEF_DUMMY_VISIT_PROC( BillboardGeometry         )
        DEF_DUMMY_VISIT_PROC( TerrainGeometry           )
        DEF_DUMMY_VISIT_PROC( MeshGeometry              )
        DEF_DUMMY_VISIT_PROC( CompositionGeometry       )
        DEF_DUMMY_VISIT_PROC( TexturedGeometry          )

        DEF_DUMMY_VISIT_PROC( Simple2DMeshGeometry      )
        DEF_DUMMY_VISIT_PROC( Simple3DMeshGeometry      )
        DEF_DUMMY_VISIT_PROC( TangentSpaceMeshGeometry  )
        DEF_DUMMY_VISIT_PROC( CommonMeshGeometry        )
        DEF_DUMMY_VISIT_PROC( TerrainMeshGeometry       )

        //TODO -> VisitPointLightSource etc.

};

#undef DEF_DUMMY_VISIT_PROC


} // /namespace Scene

} // /namespace Fork


#endif



// ========================