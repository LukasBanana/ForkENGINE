/*
 * Newton physics body header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NW_PHYSICS_BODY_H__
#define __FORK_NW_PHYSICS_BODY_H__


#include "Physics/Dynamic/Body.h"

#include <Newton.h>


namespace Fork
{

namespace Physics
{


//! Newton collision geometry implementation.
class NwBody : public virtual Body
{
    
    public:
        
        //! \throws NullPointerException If 'body'.
        NwBody(NewtonBody* body);
        virtual ~NwBody();

        void SetupTransform(const Scene::Transform& transform) override;
        Math::Matrix4f Transform() const override;

        void SetupMaterial(const MaterialPtr& material) override;

        inline NewtonBody* GetBody() const
        {
            return body_;
        }

    private:
        
        NewtonBody* body_ = nullptr;

};


} // /namespace Physics

} // /namespace Fork


#endif



// ========================