/*
 * Newton physics material header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NW_PHYSICS_MATERIAL_H__
#define __FORK_NW_PHYSICS_MATERIAL_H__


#include "Physics/Material.h"

#include <Newton.h>


namespace Fork
{

namespace Physics
{


//! Newton physics material.
class NwMaterial : public Material
{
    
    public:
        
        NwMaterial(const NewtonWorld* world);

        inline int GetGroupID() const
        {
            return groupID_;
        }

    private:
        
        int groupID_ = 0;

};


} // /namespace Physics

} // /namespace Fork


#endif



// ========================