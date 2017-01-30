/*
 * Newton physics static body header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NW_PHYSICS_STATIC_BODY_H__
#define __FORK_NW_PHYSICS_STATIC_BODY_H__


#include "Physics/Dynamic/StaticBody.h"
#include "NwBody.h"
#include "../NwCollider.h"


namespace Fork
{

namespace Physics
{


//! Newton static body implementation.
class NwStaticBody : public StaticBody, public NwBody
{
    
    public:
        
        NwStaticBody(
            const NewtonWorld* world,
            const NwColliderPtr& collider,
            const Math::Matrix4f& initTransform
        );

    private:
        
        NewtonBody* CreateBody(
            const NewtonWorld* world, const NewtonCollision* collision, const Math::Matrix4f& initTransform
        );

};


} // /namespace Physics

} // /namespace Fork


#endif



// ========================