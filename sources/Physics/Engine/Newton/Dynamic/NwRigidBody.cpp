/*
 * Newton physics rigid body file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "NwRigidBody.h"
#include "NwRigidBodyCallbacks.h"
#include "Core/Exception/NullPointerException.h"


namespace Fork
{

namespace Physics
{


NwRigidBody::NwRigidBody(
    const NewtonWorld* world, const NwColliderPtr& collider, const Math::Matrix4f& initTransform) :
        Body{ collider },
        NwBody
        {
            CreateBody(world, collider->GetCollision(), initTransform)
        }
{
    /*
    Setup rigid body callbacks
    -> to apply force/torque and set transformation
    */
    NewtonBodySetForceAndTorqueCallback(GetBody(), NwApplyForceAndTorqueCallback);
    NewtonBodySetTransformCallback(GetBody(), NwSetTransformCallback);
}

void NwRigidBody::SetupMass(float mass)
{
    /* Compute geometry inertia matrix */
    auto collision = NewtonBodyGetCollision(GetBody());

    Math::Vector3f inertia, origin;
    NewtonConvexCollisionCalculateInertialMatrix(collision, inertia.Ptr(), origin.Ptr());

    /* Setup body mass */
    NewtonBodySetMassMatrix(
        GetBody(), mass,
        inertia.x * mass,
        inertia.y * mass,
        inertia.z * mass
    );
}

float NwRigidBody::Mass() const
{
    Math::Vector3f inertia;
    float mass = 0.0f;
    
    NewtonBodyGetMassMatrix(GetBody(), &mass, &inertia.x, &inertia.y, &inertia.z);

    return mass;
}

void NwRigidBody::SetupMassCenter(const Math::Vector3f& relativeCenter)
{
    NewtonBodySetCentreOfMass(GetBody(), relativeCenter.Ptr());
}

Math::Vector3f NwRigidBody::MassCenter() const
{
    Math::Vector3f relativeCenter;
    NewtonBodyGetCentreOfMass(GetBody(), relativeCenter.Ptr());
    return relativeCenter;
}

void NwRigidBody::SetupVelocity(const Math::Vector3f& globalDirection)
{
    NewtonBodySetVelocity(GetBody(), globalDirection.Ptr());
}

Math::Vector3f NwRigidBody::Velocity() const
{
    Math::Vector3f globalDirection;
    NewtonBodyGetVelocity(GetBody(), globalDirection.Ptr());
    return globalDirection;
}

void NwRigidBody::SetupForce(const Math::Vector3f& force)
{
    NewtonBodySetForce(GetBody(), force.Ptr());
}

void NwRigidBody::AddForce(const Math::Vector3f& force)
{
    NewtonBodyAddForce(GetBody(), force.Ptr());
}

void NwRigidBody::SetupTorque(const Math::Vector3f& torque)
{
    NewtonBodySetTorque(GetBody(), torque.Ptr());
}

void NwRigidBody::AddTorque(const Math::Vector3f& torque)
{
    NewtonBodyAddTorque(GetBody(), torque.Ptr());
}

void NwRigidBody::AddImpulse(const Math::Vector3f& direction, const Math::Vector3f& pivotPoint)
{
    NewtonBodyAddImpulse(GetBody(), direction.Ptr(), pivotPoint.Ptr());
}


/*
 * ======= Private: =======
 */

NewtonBody* NwRigidBody::CreateBody(
    const NewtonWorld* world, const NewtonCollision* collision, const Math::Matrix4f& initTransform)
{
    ASSERT_POINTER(world);
    ASSERT_POINTER(collision);
    return NewtonCreateDynamicBody(world, collision, initTransform.Ptr());
}


} // /namespace Physics

} // /namespace Fork



// ========================