/*
 * Newton physics rigid body header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NW_PHYSICS_RIGID_BODY_H__
#define __FORK_NW_PHYSICS_RIGID_BODY_H__


#include "Physics/Dynamic/RigidBody.h"
#include "NwBody.h"
#include "../NwCollider.h"


namespace Fork
{

namespace Physics
{


//! Newton rigid body implementation.
class NwRigidBody : public RigidBody, public NwBody
{
    
    public:
        
        //! \throws NullPointerException If 'world' or 'geometry' is null.
        NwRigidBody(
            const NewtonWorld* world,
            const NwColliderPtr& collider,
            const Math::Matrix4f& initTransform
        );

        void SetupMass(float mass) override;
        float Mass() const override;

        void SetupMassCenter(const Math::Vector3f& relativeCenter) override;
        Math::Vector3f MassCenter() const override;

        void SetupVelocity(const Math::Vector3f& globalDirection) override;
        Math::Vector3f Velocity() const override;

        void SetupForce(const Math::Vector3f& force) override;
        void AddForce(const Math::Vector3f& force) override;

        void SetupTorque(const Math::Vector3f& torque) override;
        void AddTorque(const Math::Vector3f& torque) override;

        void AddImpulse(const Math::Vector3f& direction, const Math::Vector3f& pivotPoint) override;

    private:
        
        NewtonBody* CreateBody(
            const NewtonWorld* world, const NewtonCollision* collision, const Math::Matrix4f& initTransform
        );

};


} // /namespace Physics

} // /namespace Fork


#endif



// ========================