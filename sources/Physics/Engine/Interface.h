/*
 * Physics engine library interface header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PHYSICS_ENGINE_LIB_INTERFACE_H__
#define __FORK_PHYSICS_ENGINE_LIB_INTERFACE_H__


#include "Core/Export.h"


extern "C"
{

/**
Returns a raw-pointer to the allocated physics system.
This should be wrapped into a smart pointer:
\code
auto physicsSystem = std::shared_ptr<PhysicsSystem>(AllocPhysicsSystem());
\endcode
*/
FORK_EXPORT void* AllocPhysicsSystem();

}


#endif



// ========================