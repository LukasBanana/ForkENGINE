/*
 * Physics collider header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PHYSICS_COLLIDER_H__
#define __FORK_PHYSICS_COLLIDER_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Math/Core/Matrix4.h"
#include "Math/Geometry/Line.h"
#include "Math/Collision/Intersection.h"


namespace Fork
{

namespace Physics
{


DECL_SHR_PTR(Collider);

//! Physics collider interface.
class FORK_EXPORT Collider
{
    
    public:
        
        /* === Enumerations === */

        //! Physics collider shapes.
        enum class Shapes
        {
            Null,               //!< Null collider. This can be used for non-collidable rigid bodies (e.g. rag-doll necks).
            Sphere,             //!< Sphere collider.
            Box,                //!< Box collider.
            Cone,               //!< Cone collider.
            Capsule,            //!< Capsule collider.
            Cylinder,           //!< Cylinder collider.
            ChamferCylinder,    //!< Chamfer cylinder collider.
            ConvexHull,         //!< Abitrary convex hull collider. A convex hull can be build out of vertex data.
            Mesh,               //!< Abitrary mesh collider. Mesh collisions can only be used for static bodies.
        };

        /* === Interface === */

        virtual ~Collider();

        /**
        Makes a ray cast intersection test with this collider.
        \param[in] invLine Specifies the inverse line for the ray cast. This is actually the "ray" but with a line,
        the length can be set. Because a collider never has any transformation data, i.e. the position, rotation
        and scale in global space, one has to transform the line by the inverse matrix of the collider transformation,
        which is stored in another class, e.g. "Physics::RigidBody" or "Scene::DynamicSceneNode".
        That's why this is called "invLine" - for "inverse line".
        \code
        auto invObjectMatrix = sceneNode.transform.GetMatrix().Inverse();
        auto invLine = Math::Transform(invObjectMatrix, line);
        collider->RayCast(invLine, intersection);
        \endcode
        \param[out] intersection Specifices the resulting intersection. An intersection contains a point and a normal vector.
        \return True if the ray hit the collider.
        \note The intersection point and normal vector are only in object space (and not in world space).
        So you have to transform it by the scene node transformation matrix, to convert it to world space.
        */
        virtual bool RayCast(const Math::Line3f& invLine, Math::Intersection3f& intersection) const = 0;

        /**
        Makes a ray cast intersection test with this collider.
        \param[in] worldMatrix Specifies the matrix (in world space) whose
        inverse will be used to transform the specified line to the inverse-line.
        \note This is just a comfort function. For better performance, use the other "RayCast" function!
        \see RayCast(const Math::Line3f&, Math::Intersection3f&)
        */
        bool RayCast(
            const Math::Matrix4f& worldMatrix, const Math::Line3f& line, Math::Intersection3f& intersection
        ) const;

        //! Sets the new collider scale. By default { 1, 1, 1 }.
        virtual void SetupScale(const Math::Vector3f& scale) = 0;
        //! Returns the collider scale.
        virtual Math::Vector3f Scale() const = 0;

        //! User defined raw-pointer. By default null.
        void* userData = nullptr;

    protected:
        
        Collider() = default;

        inline Collider* ThisColliderPtr()
        {
            return this;
        }

};


} // /namespace Physics

} // /namespace Fork


#endif



// ========================