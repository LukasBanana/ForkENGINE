/*
 * Physics system file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Physics/PhysicsSystem.h"
#include "Physics/PhysicsSystemException.h"
#include "Platform/Core/OS.h"

#include <algorithm>


namespace Fork
{

namespace Physics
{


PhysicsSystem* PhysicsSystem::activePhysicsSystem_ = nullptr;

PhysicsSystem::PhysicsSystem()
{
    PhysicsSystem::activePhysicsSystem_ = this;

    /* Create standard rigid body event handlers */
    standardRigidBodyEventHandler_ = std::make_shared<RigidBody::StandardEventHandler>();
    RigidBody::SetDefaultEventHandler(standardRigidBodyEventHandler_);

    pickingRigidBodyEventHandler_ = std::make_shared<RigidBody::PickingEventHandler>();
}
PhysicsSystem::~PhysicsSystem()
{
}

PhysicsSystemPtr PhysicsSystem::Create(const Platform::DynamicLibraryPtr& library)
{
    const std::string procName = "AllocPhysicsSystem";

    /* Load physics system allocation procedure from library */
    DEFINE_PROC_INTERFACE(void*, PFNALLOCPHYSICSSYSTEMPROC, (void));
    auto AllocPhysicsSystem = reinterpret_cast<PFNALLOCPHYSICSSYSTEMPROC>(library->LoadProc(procName));

    if (!AllocPhysicsSystem)
    {
        throw PhysicsSystemException(
            "Procedure \"" + procName + "\" not found in physics system library \"" + ToStr(library->GetName()) + "\""
        );
    }

    /* Allocate physics system via library function */
    auto physicsSystem = reinterpret_cast<PhysicsSystem*>(AllocPhysicsSystem());

    if (!physicsSystem)
    {
        throw PhysicsSystemException(
            "Allocating physics system from library \"" + ToStr(library->GetName()) + "\" failed"
        );
    }

    return std::shared_ptr<PhysicsSystem>(physicsSystem);
}

PhysicsSystem* PhysicsSystem::Active()
{
    return activePhysicsSystem_;
}

PhysicsSystem* PhysicsSystem::Active(bool isRequired)
{
    if (!activePhysicsSystem_ && isRequired)
        throw PhysicsSystemException("PhysicsSystem is required but has not been created yet");
    return activePhysicsSystem_;
}

void PhysicsSystem::ReleaseWorld(const WorldPtr& world)
{
    std::remove(worlds_.begin(), worlds_.end(), world);
}


} // /namespace Physics

} // /namespace Fork



// ========================