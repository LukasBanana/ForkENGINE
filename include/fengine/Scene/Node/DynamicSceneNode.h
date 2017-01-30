/*
 * Dynamic scene node header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_DYNAMIC_SCENE_NODE_H__
#define __FORK_DYNAMIC_SCENE_NODE_H__


#include "Scene/Node/SceneNode.h"


namespace Fork
{

namespace Scene
{


/**
Dynamic scene node base class. A dynamic scene node has a 3D transformation.
\see Scene::Transform3D
*/
class FORK_EXPORT DynamicSceneNode : public SceneNode
{
    
    public:
        
        DynamicSceneNode() = default;
        virtual ~DynamicSceneNode();

        DynamicSceneNode(const DynamicSceneNode&) = delete;
        DynamicSceneNode& operator = (const DynamicSceneNode&) = delete;

        const Math::Matrix4f& LocalTransform() const;

        void GlobalTransform(Math::Matrix4f& matrix) const;
        Math::Matrix4f GlobalTransform() const;

        bool HasTransform() const;

        /**
        Local dynamic scene node 3D transformation. To get a global transformation, use "GlobalTransform".
        \see Math::Transform3D
        */
        Transform transform;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================