/*
 * Newton physics system file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "NwPhysicsSystem.h"
#include "NwWorld.h"


namespace Fork
{

namespace Physics
{


WorldPtr NwPhysicsSystem::CreateWorld()
{
    auto world = std::make_shared<NwWorld>();
    worlds_.push_back(world);
    return world;
}


} // /namespace Physics

} // /namespace Fork



// ========================