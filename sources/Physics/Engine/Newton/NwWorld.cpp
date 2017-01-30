/*
 * Newton physics world file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "NwWorld.h"
#include "NwMaterial.h"
#include "NwCollider.h"
#include "Dynamic/NwRigidBody.h"
#include "Dynamic/NwStaticBody.h"

#include "Core/Exception/PointerConversionException.h"


namespace Fork
{

namespace Physics
{


NwWorld::NwWorld()
{
    world_ = NewtonCreate();
}
NwWorld::~NwWorld()
{
    NewtonDestroy(world_);
}

/* --- Material --- */

MaterialPtr NwWorld::CreateMaterial()
{
    auto material = std::make_shared<NwMaterial>(world_);
    materials_.push_back(material);
    return material;
}

void NwWorld::SetupMaterialBehavior(
    Material* materialA, Material* materialB, const Material::Behavior& behavior)
{
    /* Get newton materials */
    auto materialANw = dynamic_cast<NwMaterial*>(materialA);
    auto materialBNw = dynamic_cast<NwMaterial*>(materialB);

    if (!materialANw || !materialBNw)
        throw PointerConversionException(__FUNCTION__, "Material", "NwMaterial");

    auto groupID0 = materialANw->GetGroupID();
    auto groupID1 = materialBNw->GetGroupID();

    /* Setup material configuration */
    NewtonMaterialSetDefaultCollidable(
        world_, groupID0, groupID1,
        behavior.collidable ? 1 : 0
    );
    NewtonMaterialSetDefaultFriction(
        world_, groupID0, groupID1,
        behavior.staticFriction, behavior.kineticFriction
    );
    NewtonMaterialSetDefaultElasticity(
        world_, groupID0, groupID1,
        behavior.elasticity
    );
    NewtonMaterialSetDefaultSoftness(
        world_, groupID0, groupID1,
        behavior.softness
    );
}

/* --- Collider --- */

ColliderPtr NwWorld::CreateNullCollider()
{
    return NwCollider::CreateNull(world_);
}

ColliderPtr NwWorld::CreateSphereCollider(float radius, const Math::Matrix4f* offsetMatrix)
{
    return NwCollider::CreateSphere(world_, radius, offsetMatrix);
}

ColliderPtr NwWorld::CreateBoxCollider(const Math::Size3f& size, const Math::Matrix4f* offsetMatrix)
{
    return NwCollider::CreateBox(world_, size, offsetMatrix);
}

ColliderPtr NwWorld::CreateConeCollider(float radius, float height, const Math::Matrix4f* offsetMatrix)
{
    return NwCollider::CreateCone(world_, radius, height, offsetMatrix);
}

ColliderPtr NwWorld::CreateCapsuleCollider(float radius, float height, const Math::Matrix4f* offsetMatrix)
{
    return NwCollider::CreateCapsule(world_, radius, height, offsetMatrix);
}

ColliderPtr NwWorld::CreateCylinderCollider(float radius, float height, const Math::Matrix4f* offsetMatrix)
{
    return NwCollider::CreateCylinder(world_, radius, height, offsetMatrix);
}

ColliderPtr NwWorld::CreateChamferCylinderCollider(float radius, float height, const Math::Matrix4f* offsetMatrix)
{
    return NwCollider::CreateChamferCylinder(world_, radius, height, offsetMatrix);
}

ColliderPtr NwWorld::CreateConvexHullCollider(
    const void* vertexData, unsigned int numVertices, unsigned int vertexStride,
    const Math::Matrix4f* offsetMatrix, float tolerance)
{
    return NwCollider::CreateConvexHull(world_, vertexData, numVertices, vertexStride, offsetMatrix, tolerance);
}

ColliderPtr NwWorld::CreateMeshCollider(
    const void* vertexData, unsigned int numVertices, unsigned int vertexStride,
    const Math::Matrix4f* offsetMatrix, bool optimize)
{
    return NwCollider::CreateMesh(world_, vertexData, numVertices, vertexStride, offsetMatrix, optimize);
}

ColliderPtr NwWorld::CreateMeshCollider(
    const void* vertexData, unsigned int numVertices, unsigned int vertexStride,
    const void* indexData, unsigned int numIndices, unsigned int indexStride,
    const Math::Matrix4f* offsetMatrix, bool optimize)
{
    return NwCollider::CreateMesh(
        world_,
        vertexData, numVertices, vertexStride,
        indexData, numIndices, indexStride,
        offsetMatrix, optimize
    );
}

/* --- Bodies --- */

RigidBodyPtr NwWorld::CreateRigidBody(
    const ColliderPtr& collider, const Math::Matrix4f& initTransform)
{
    /* Get newton geometry */
    auto colliderNw = std::dynamic_pointer_cast<NwCollider>(collider);
    if (!colliderNw)
        throw PointerConversionException(__FUNCTION__, "Collider", "NwCollider");

    /* Create rigid body */
    return std::make_shared<NwRigidBody>(world_, colliderNw, initTransform);
}

StaticBodyPtr NwWorld::CreateStaticBody(
    const ColliderPtr& collider, const Math::Matrix4f& initTransform)
{
    /* Get newton geometry */
    auto colliderNw = std::dynamic_pointer_cast<NwCollider>(collider);
    if (!colliderNw)
        throw PointerConversionException(__FUNCTION__, "Collider", "NwCollider");

    /* Create static body */
    return std::make_shared<NwStaticBody>(world_, colliderNw, initTransform);
}

/* --- Physics simulation --- */

void NwWorld::Simulate(float timeStep)
{
    NewtonUpdate(world_, timeStep);
}

/* --- Intersection tests --- */

struct WorldRayCastUserDataWrapper
{
    void*                       userData        = nullptr;
    World::RayCastEventHandler* eventHandler    = nullptr;
};

static unsigned NwWorldRayPrefilterCallback(
    const NewtonBody* const body, const NewtonCollision* const collision, void* const userData)
{
    /* Get physics system objects */
    auto bodyRef        = reinterpret_cast<const Body*>(NewtonBodyGetUserData(body));
    auto geometryRef    = reinterpret_cast<const Collider*>(NewtonCollisionGetUserData(collision));

    /* Call event handler */
    auto userDataWrapper = reinterpret_cast<WorldRayCastUserDataWrapper*>(userData);

    if (!userDataWrapper->eventHandler->OnContactAABB(bodyRef, geometryRef, userDataWrapper->userData))
        return 0;

    return 1;
}

static dFloat NwWorldRayFilterCallback(
    const NewtonBody* const body, const NewtonCollision* const collision,
    const dFloat* const hitContact, const dFloat* const hitNormal,
    dLong collisionID, void* const userData, dFloat intersectParam)
{
    /* Get physics system objects */
    auto bodyRef        = reinterpret_cast<const Body*>(NewtonBodyGetUserData(body));
    auto geometryRef    = reinterpret_cast<const Collider*>(NewtonCollisionGetUserData(collision));

    /* Setup intersection */
    Math::Intersection3f intersection
    {
        { hitContact[0], hitContact[1], hitContact[2] },
        { hitNormal[0], hitNormal[1], hitNormal[2] }
    };

    /* Call event handler */
    auto userDataWrapper = reinterpret_cast<WorldRayCastUserDataWrapper*>(userData);

    auto proceeding = userDataWrapper->eventHandler->OnContact(
        bodyRef, geometryRef, intersection, userDataWrapper->userData
    );

    /* Check ray cast procedding */
    switch (proceeding)
    {
        case World::RayCastProceedings::Continue:
            return 1.0f;
        case World::RayCastProceedings::Nearest:
            return intersectParam;
        case World::RayCastProceedings::Cancel:
        default:
            break;
    }

    return 0.0f;
}

void NwWorld::RayCast(
    const Math::Line3f& line, RayCastEventHandler* eventHandler, void* userData) const
{
    if (eventHandler)
    {
        WorldRayCastUserDataWrapper userDataWrapper;
        {
            userDataWrapper.userData = userData;
            userDataWrapper.eventHandler = eventHandler;
        }
        NewtonWorldRayCast(
            world_, line.start.Ptr(), line.end.Ptr(),
            NwWorldRayFilterCallback, &userDataWrapper, NwWorldRayPrefilterCallback, 0
        );
    }
}


} // /namespace Physics

} // /namespace Fork



// ========================