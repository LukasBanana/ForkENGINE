/*
 * Newton physics static body file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "NwStaticBody.h"
#include "Core/Exception/NullPointerException.h"


namespace Fork
{

namespace Physics
{


NwStaticBody::NwStaticBody(
    const NewtonWorld* world, const NwColliderPtr& collider, const Math::Matrix4f& initTransform) :
        Body{ collider },
        NwBody
        {
            CreateBody(world, collider->GetCollision(), initTransform)
        }
{
}


/*
 * ======= Private: =======
 */

NewtonBody* NwStaticBody::CreateBody(
    const NewtonWorld* world, const NewtonCollision* collision, const Math::Matrix4f& initTransform)
{
    ASSERT_POINTER(world);
    ASSERT_POINTER(collision);
    return NewtonCreateDynamicBody(world, collision, initTransform.Ptr());
}


} // /namespace Physics

} // /namespace Fork



// ========================