/*
 * Static scene node header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_STATIC_SCENE_NODE_H__
#define __FORK_STATIC_SCENE_NODE_H__


#include "Scene/Node/SceneNode.h"


namespace Fork
{

namespace Scene
{


//! Static scene node base class. A static scene node has no transformation.
class FORK_EXPORT StaticSceneNode : public SceneNode
{
    
    public:
        
        virtual ~StaticSceneNode()
        {
        }

        StaticSceneNode(const StaticSceneNode&) = delete;
        StaticSceneNode& operator = (const StaticSceneNode&) = delete;

    protected:

        StaticSceneNode() = default;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================