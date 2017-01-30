/*
 * Physics static body header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PHYSICS_STATIC_BODY_H__
#define __FORK_PHYSICS_STATIC_BODY_H__


#include "Physics/Dynamic/Body.h"


namespace Fork
{

namespace Physics
{


DECL_SHR_PTR(StaticBody);

/**
Physics static body interface. Static bodies are physics objects,
which represent the static environment. These objects have no mass,
and thus can not suffer forces, momentums or impulses.
*/
class FORK_EXPORT StaticBody : public virtual Body
{
    
    public:
        
        /* === Interface === */

        virtual ~StaticBody()
        {
        }

        //! Returns Types::RigidBody.
        Types Type() const
        {
            return Types::StaticBody;
        }

    protected:
        
        StaticBody() = default;

};


} // /namespace Physics

} // /namespace Fork


#endif



// ========================