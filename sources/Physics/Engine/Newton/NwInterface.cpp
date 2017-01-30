/*
 * Newton physics engine library interface file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "../Interface.h"
#include "NwPhysicsSystem.h"


extern "C"
{

/*
Allocate the "newton game dynamics" physics systems and return the raw pointer.
-> This is an exception in memory management of this engine.
   All other memory objects must be created with managed memory (i.e. std::shared_ptr)!
*/

FORK_EXPORT void* AllocPhysicsSystem()
{
    return new Fork::Physics::NwPhysicsSystem();
}

}



// ========================