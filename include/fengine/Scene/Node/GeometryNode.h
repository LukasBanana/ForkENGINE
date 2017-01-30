/*
 * Geometry node header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GEOMETRY_NODE_H__
#define __FORK_GEOMETRY_NODE_H__


#include "Scene/Node/DynamicSceneNode.h"
#include "Scene/Geometry/Node/Geometry.h"
#include "Video/RenderSystem/Shader/ShaderComposition.h"


namespace Fork
{

namespace Scene
{


//! Geometry scene node class.
class FORK_EXPORT GeometryNode : public DynamicSceneNode
{
    
    public:
        
        GeometryNode() = default;
        virtual ~GeometryNode();

        GeometryNode(const GeometryNode&) = delete;
        GeometryNode& operator = (const GeometryNode&) = delete;

        /* === Functions === */

        virtual void Visit(SceneVisitor* visitor);

        /* === Members === */

        /**
        Main geometry for this scene node.
        \see Geometry
        \see CompositionGeometry
        \see LODGeometry
        \see BillboardGeometry
        \see TerrainGeometry
        \see MeshGeometry
        \see TexturedGeometry
        */
        GeometryPtr geometry;

        /**
        Custom shader composition.
        \todo Replace by "TexturedGeometry::shaderComposition"
        */
        Video::ShaderCompositionPtr shaderComposition;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================