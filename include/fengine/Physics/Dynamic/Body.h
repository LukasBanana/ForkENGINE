/*
 * Physics body header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PHYSICS_BODY_H__
#define __FORK_PHYSICS_BODY_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Math/Core/Matrix4.h"
#include "Physics/Collider.h"
#include "Physics/Material.h"
#include "Scene/Node/SceneNode.h"


namespace Fork
{

namespace Physics
{


DECL_SHR_PTR(Body);

//! Physics body interface.
class FORK_EXPORT Body
{
    
    public:
        
        /* === Enumerations === */

        //! Physics body types.
        enum class Types
        {
            StaticBody,
            RigidBody,
            SoftBody,
        };

        /* === Interface === */

        virtual ~Body()
        {
        }

        //! Returns the type of this physics body.
        virtual Types Type() const = 0;

        //! Sets the global transformation of this physics body.
        virtual void SetupTransform(const Scene::Transform& transform) = 0;
        //! Returns the global transformation matrix of this physics body.
        virtual Math::Matrix4f Transform() const = 0;

        //! Sets the physics material of this physics body.
        virtual void SetupMaterial(const MaterialPtr& material) = 0;

        /* === Inline functions === */

        //! Returns a raw-pointer to the collision geoemtry of this physics body.
        inline Collider* GetCollider() const
        {
            return collider_.get();
        }
        //! Returns a raw-pointer to the physics material of this physics body.
        inline Material* GetMaterial() const
        {
            return material_.get();
        }

        /* === Members === */

        //! User defined raw-pointer. By default null.
        void* userData = nullptr;

        /**
        Reference to the transformation which will be controlled by this physics body.
        If this is null, no transformation is controlled. By default null.
        */
        Scene::Transform* transform = nullptr;

    protected:
        
        Body() = default;
        Body(const ColliderPtr& collider) :
            collider_{ collider }
        {
        }

    protected:
        
        inline Body* ThisBodyPtr()
        {
            return this;
        }

        MaterialPtr material_; //!< Physics material of this body.

    private:

        ColliderPtr collider_; //!< Collider of this body.
        
};


} // /namespace Physics

} // /namespace Fork


#endif



// ========================