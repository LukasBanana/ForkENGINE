/*
 * Newton physics material file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "NwMaterial.h"


namespace Fork
{

namespace Physics
{


NwMaterial::NwMaterial(const NewtonWorld* world)
{
    /* Generate ID number for material group */
    groupID_ = NewtonMaterialCreateGroupID(world);
}


} // /namespace Physics

} // /namespace Fork



// ========================