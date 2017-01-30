/*
 * Physics system header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PHYSICS_SYSTEM_H__
#define __FORK_PHYSICS_SYSTEM_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Math/Geometry/Ray.h"
#include "Math/Geometry/Line.h"
#include "Platform/Core/DynamicLibrary.h"
#include "Physics/World.h"

#include <vector>


namespace Fork
{

namespace Physics
{


DECL_SHR_PTR(PhysicsSystem);

//! Physics system interface.
class FORK_EXPORT PhysicsSystem
{
    
    public:
        
        /* === Static functions === */

        /**
        Creates a physics system from the specified library.
        \param[in] library Specifies the library from which the render system is to be created (e.g. "ForkPhysicsNw" or "ForkPhysicsPx").
        \throws PhysicsSystemException If an error occured during creation.
        \see Platform::DynamicLibrary
        */
        static PhysicsSystemPtr Create(const Platform::DynamicLibraryPtr& library);

        //! Returns the active physics system.
        static PhysicsSystem* Active();
        /**
        Returns the active physics system.
        \param[in] isRequired Specifies whether the physics system is required or not.
        \return Raw-pointer to the active physics system or null if no physics system has been created yet.
        \throws PhysicsSystemException If 'isRequired' is true but the return value is null.
        */
        static PhysicsSystem* Active(bool isRequired);

        /* === Interface === */

        virtual ~PhysicsSystem();

        /**
        Creates a new physics world. This is similar to a scene graph.
        Each physics world will be simulated independently of each other.
        */
        virtual WorldPtr CreateWorld() = 0;
        //! Releases the specified physics world.
        virtual void ReleaseWorld(const WorldPtr& world);

        //! Returns the physics world list.
        inline const std::vector<WorldPtr>& GetWorlds() const
        {
            return worlds_;
        }

        //! Returns the standard rigid body event handler.
        inline const RigidBody::StandardEventHandlerPtr& GetStdRigidBodyEventHandler() const
        {
            return standardRigidBodyEventHandler_;
        }
        //! Returns the picking rigid body event handler.
        inline const RigidBody::PickingEventHandlerPtr& GetPickingRigidBodyEventHandler() const
        {
            return pickingRigidBodyEventHandler_;
        }

    protected:
        
        /* === Functions === */

        PhysicsSystem();

        /* === Members === */

        std::vector<WorldPtr> worlds_;

    private:
        
        RigidBody::StandardEventHandlerPtr standardRigidBodyEventHandler_;
        RigidBody::PickingEventHandlerPtr pickingRigidBodyEventHandler_;

        static PhysicsSystem* activePhysicsSystem_;

};


} // /namespace Physics

} // /namespace Fork


#endif



// ========================