/*
 * Newton physics system header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NW_PHYSICS_SYSTEM_H__
#define __FORK_NW_PHYSICS_SYSTEM_H__


#include "Physics/PhysicsSystem.h"


namespace Fork
{

namespace Physics
{


//! Newton physics system implementation.
class NwPhysicsSystem : public PhysicsSystem
{
    
    public:
        
        WorldPtr CreateWorld() override;

};


} // /namespace Physics

} // /namespace Fork


#endif



// ========================