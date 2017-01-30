/*
 * Newton physics rigid body callbacks file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "NwRigidBodyCallbacks.h"
#include "NwRigidBody.h"
#include "Core/CastToConcreteObject.h"
#include "../NwCore.h"


namespace Fork
{

namespace Physics
{


//!TODO! -> avoid dynamic_cast !!!

void NwApplyForceAndTorqueCallback(
    const NewtonBody* const body, dFloat timeStep, int threadIndex)
{
    auto bodyRef = reinterpret_cast<Body*>(NewtonBodyGetUserData(body));
    //auto rigidBodyRef = CAST_TO_NW_OBJECT(RigidBody, bodyRef);
    auto rigidBodyRef = dynamic_cast<NwRigidBody*>(bodyRef);
    if (rigidBodyRef)
        rigidBodyRef->PostApplyForceAndTorque();
}

void NwSetTransformCallback(
    const NewtonBody* const body, const dFloat* const matrix, int threadIndex)
{
    auto bodyRef = reinterpret_cast<Body*>(NewtonBodyGetUserData(body));
    //auto rigidBodyRef = CAST_TO_NW_OBJECT(RigidBody, bodyRef);
    auto rigidBodyRef = dynamic_cast<NwRigidBody*>(bodyRef);
    if (rigidBodyRef)
        rigidBodyRef->PostUpdateTransform(*reinterpret_cast<const Math::Matrix4f*>(matrix));
}


} // /namespace Physics

} // /namespace Fork



// ========================