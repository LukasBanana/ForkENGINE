/*
 * Newton physics world header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NW_PHYSICS_WORLD_H__
#define __FORK_NW_PHYSICS_WORLD_H__


#include "Physics/World.h"

#include <Newton.h>


namespace Fork
{

namespace Physics
{


//! Newton physics world
class NwWorld : public World
{
    
    public:
        
        NwWorld();
        ~NwWorld();

        /* --- Material --- */

        MaterialPtr CreateMaterial() override;

        void SetupMaterialBehavior(
            Material* materialA, Material* materialB, const Material::Behavior& behavior
        ) override;

        /* --- Collider --- */

        ColliderPtr CreateNullCollider              () override;
        ColliderPtr CreateSphereCollider            (float radius, const Math::Matrix4f* offsetMatrix = nullptr) override;
        ColliderPtr CreateBoxCollider               (const Math::Size3f& size, const Math::Matrix4f* offsetMatrix = nullptr) override;
        ColliderPtr CreateConeCollider              (float radius, float height, const Math::Matrix4f* offsetMatrix = nullptr) override;
        ColliderPtr CreateCapsuleCollider           (float radius, float height, const Math::Matrix4f* offsetMatrix = nullptr) override;
        ColliderPtr CreateCylinderCollider          (float radius, float height, const Math::Matrix4f* offsetMatrix = nullptr) override;
        ColliderPtr CreateChamferCylinderCollider   (float radius, float height, const Math::Matrix4f* offsetMatrix = nullptr) override;

        ColliderPtr CreateConvexHullCollider(
            const void* vertexData, unsigned int numVertices, unsigned int vertexStride,
            const Math::Matrix4f* offsetMatrix = nullptr, float tolerance = 0.002f
        ) override;

        ColliderPtr CreateMeshCollider(
            const void* vertexData, unsigned int numVertices, unsigned int vertexStride,
            const Math::Matrix4f* offsetMatrix = nullptr, bool optimize = true
        ) override;

        ColliderPtr CreateMeshCollider(
            const void* vertexData, unsigned int numVertices, unsigned int vertexStride,
            const void* indexData, unsigned int numIndices, unsigned int indexStride,
            const Math::Matrix4f* offsetMatrix = nullptr, bool optimize = true
        ) override;

        /* --- Bodies --- */

        /**
        Creates a new dynamic rigid body.
        \throws PointerConversionException If 'geoemtry' is not a physics geometry created by this physics world.
        \see RigidBody
        */
        RigidBodyPtr CreateRigidBody(
            const ColliderPtr& collider, const Math::Matrix4f& initTransform = {}
        ) override;

        /**
        Creates a new static body.
        \throws PointerConversionException If 'geoemtry' is not a physics geometry created by this physics world.
        \see StaticBody
        */
        StaticBodyPtr CreateStaticBody(
            const ColliderPtr& collider, const Math::Matrix4f& initTransform = {}
        ) override;

        /* --- Physics simulation --- */

        void Simulate(float timeStep) override;

        /* --- Intersection tests --- */

        void RayCast(
            const Math::Line3f& line, RayCastEventHandler* eventHandler, void* userData = nullptr
        ) const override;

    private:
        
        NewtonWorld* world_ = nullptr;

};


} // /namespace Physics

} // /namespace Fork


#endif



// ========================