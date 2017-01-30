/*
 * Scene header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SCENE_H__
#define __FORK_SCENE_H__


/* --- Core header file --- */

#include "Core/StaticConfig.h"
#include "Scene/SceneHelper.h"


/* --- Node header files --- */

#include "Scene/FileHandler/ModelFileHandler.h"


/* --- Node header files --- */

#include "Scene/Node/SceneVisitor.h"
#include "Scene/Node/CameraNode.h"
#include "Scene/Node/LightNode.h"
#include "Scene/Node/GeometryNode.h"


/* --- Light source header files --- */

#include "Scene/LightSource/DirectionalLightSource.h"
#include "Scene/LightSource/PointLightSource.h"
#include "Scene/LightSource/SpotLightSource.h"


/* --- Geometry header files --- */

#include "Scene/Geometry/Node/CompositionGeometry.h"
#include "Scene/Geometry/Node/TexturedGeometry.h"
#include "Scene/Geometry/Node/LODGeometry.h"
#include "Scene/Geometry/Node/MeshGeometry.h"
#include "Scene/Geometry/Node/BillboardGeometry.h"
#include "Scene/Geometry/Node/CommonMeshGeometry.h"
#include "Scene/Geometry/Node/Simple2DMeshGeometry.h"
#include "Scene/Geometry/Node/Simple3DMeshGeometry.h"
#include "Scene/Geometry/Node/TangentSpaceMeshGeometry.h"
#include "Scene/Geometry/Generator/GeometryGenerator.h"
#include "Scene/Geometry/Modifier/GeometryAutoUVMapModifier.h"
#include "Scene/Geometry/GeometryConverter.h"
#include "Scene/Geometry/MeshGeometryAtlas.h"
#include "Scene/Geometry/GeometrySearchFilter.h"
#include "Scene/Geometry/Skeleton.h"


/* --- Manager header files --- */

#include "Scene/Manager/SceneManager.h"
#include "Scene/Manager/FreeViewSceneNodeController.h"
#include "Scene/Manager/PresentSceneNodeController.h"


/* --- Renderer header files --- */

#include "Scene/Renderer/ForwardSceneRenderer.h"
#include "Scene/Renderer/SimpleSceneRenderer.h"
#include "Scene/Renderer/BoundingBoxSceneRenderer.h"
#include "Scene/Renderer/LogSceneRenderer.h"
#include "Scene/Renderer/CubeMapRenderer.h"


#endif



// ========================