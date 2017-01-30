/*
 * Light node header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_LIGHT_NODE_H__
#define __FORK_LIGHT_NODE_H__


#include "Scene/Node/DynamicSceneNode.h"
#include "Scene/LightSource/LightSource.h"


namespace Fork
{

namespace Scene
{


//! Light source scene node class.
class FORK_EXPORT LightNode : public DynamicSceneNode
{
    
    public:
        
        LightNode() = default;
        LightNode(const LightNode&) = delete;
        LightNode& operator = (const LightNode&) = delete;
        virtual ~LightNode();

        /* === Functions === */

        virtual void Visit(SceneVisitor* visitor);

        /* === Members === */

        /**
        Light source information for the scene node.
        \see LightSource
        */
        LightSourcePtr lightSource;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================