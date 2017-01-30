/*
 * Scene node declaration header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SCENE_NODE_DECL_H__
#define __FORK_SCENE_NODE_DECL_H__


#include "Core/DeclPtr.h"


namespace Fork
{

namespace Scene
{


/* --- Delcare scene nodes --- */

DECL_SHR_PTR( SceneNode        );
DECL_SHR_PTR( StaticSceneNode  );
DECL_SHR_PTR( DynamicSceneNode );
DECL_SHR_PTR( GeometryNode     );
DECL_SHR_PTR( LightNode        );
DECL_SHR_PTR( CameraNode       );

/* --- Declare geometry classes --- */

DECL_SHR_PTR( BillboardGeometry   );
DECL_SHR_PTR( LODGeometry         );
DECL_SHR_PTR( MeshGeometry        );
DECL_SHR_PTR( CompositionGeometry );
DECL_SHR_PTR( TexturedGeometry    );
DECL_SHR_PTR( TerrainGeometry     );

/* --- Declare mesh geometry --- */

DECL_SHR_PTR( CommonMeshGeometry       );
DECL_SHR_PTR( Simple2DMeshGeometry     );
DECL_SHR_PTR( Simple3DMeshGeometry     );
DECL_SHR_PTR( TangentSpaceMeshGeometry );
DECL_SHR_PTR( TerrainMeshGeometry      );


} // /namespace Scene

} // /namespace Fork


#endif



// ========================