/*
 * Newton physics collider header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_NW_PHYSICS_COLLIDER_H__
#define __FORK_NW_PHYSICS_COLLIDER_H__


#include "Physics/Collider.h"
#include "Math/Core/Size3.h"
#include "Math/Core/Matrix4.h"

#include <Newton.h>
#include <vector>


namespace Fork
{

namespace Physics
{


DECL_SHR_PTR(NwCollider);

//! Newton collision geometry implementation.
class NwCollider : public Collider
{
    
    public:
        
        //! \throws NullPointerException If 'collision' is null.
        NwCollider(NewtonCollision* collision);
        ~NwCollider();

        static ColliderPtr CreateNull           (const NewtonWorld* world);
        static ColliderPtr CreateSphere         (const NewtonWorld* world, float radius, const Math::Matrix4f* offsetMatrix);
        static ColliderPtr CreateBox            (const NewtonWorld* world, const Math::Size3f& size, const Math::Matrix4f* offsetMatrix);
        static ColliderPtr CreateCone           (const NewtonWorld* world, float radius, float height, const Math::Matrix4f* offsetMatrix);
        static ColliderPtr CreateCapsule        (const NewtonWorld* world, float radius, float height, const Math::Matrix4f* offsetMatrix);
        static ColliderPtr CreateCylinder       (const NewtonWorld* world, float radius, float height, const Math::Matrix4f* offsetMatrix);
        static ColliderPtr CreateChamferCylinder(const NewtonWorld* world, float radius, float height, const Math::Matrix4f* offsetMatrix);

        static ColliderPtr CreateConvexHull(
            const NewtonWorld* world,
            const void* vertexData, unsigned int numVertices, unsigned int vertexStride,
            const Math::Matrix4f* offsetMatrix, float tolerance
        );

        static ColliderPtr CreateMesh(
            const NewtonWorld* world,
            const void* vertexData, unsigned int numVertices, unsigned int vertexStride,
            const Math::Matrix4f* offsetMatrix, bool optimize
        );

        //! \throws InvalidArgumentException If 'indexStride' is not 1, 2 or 4.
        static ColliderPtr CreateMesh(
            const NewtonWorld* world,
            const void* vertexData, unsigned int numVertices, unsigned int vertexStride,
            const void* indexData, unsigned int numIndices, unsigned int indexStride,
            const Math::Matrix4f* offsetMatrix, bool optimize
        );

        bool RayCast(const Math::Line3f& line, Math::Intersection3f& intersection) const;

        void SetupScale(const Math::Vector3f& scale);
        Math::Vector3f Scale() const;

        inline NewtonCollision* GetCollision() const
        {
            return collision_;
        }

    private:
        
        NewtonCollision* collision_ = nullptr;

};


} // /namespace Physics

} // /namespace Fork


#endif



// ========================