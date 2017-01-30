/*
 * Physics world header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PHYSICS_WORLD_H__
#define __FORK_PHYSICS_WORLD_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Math/Geometry/Ray.h"
#include "Math/Geometry/Line.h"
#include "Math/Core/Size3.h"
#include "Math/Core/Matrix4.h"
#include "Physics/Material.h"
#include "Physics/Collider.h"
#include "Physics/Dynamic/RigidBody.h"
#include "Physics/Dynamic/StaticBody.h"

#include <vector>


namespace Fork
{

namespace Scene
{

// Forward declaration
class Geometry;

} // /namespace Scene

namespace Physics
{


DECL_SHR_PTR(World);

//! Physics world interface.
class FORK_EXPORT World
{
    
    public:
        
        /* === Enumerations === */

        /**
        Contact proceeding enumerations.
        \see OnContact
        */
        enum class RayCastProceedings
        {
            Continue,   //!< Continue ray casting.
            Nearest,    //!< Continue ray casting only if the current contact is the nearest.
            Cancel,     //!< Cancel ray casting after the current contact.
        };

        /* === Classes === */

        //! Physics world ray-cast event handler interface.
        class FORK_EXPORT RayCastEventHandler
        {
            
            public:
                
                virtual ~RayCastEventHandler()
                {
                }

                /**
                Prefilter ray-cast contact. This can be used to speed up ray-casting.
                \param[in] body Raw-pointer to the physics body which is potentially hit by the ray.
                \param[in] geometry Raw-pointer to the collision geometry of the physics body.
                \param[in] userData User defined raw-pointer which has been passed to the "World::RayCast" function.
                \return False if the contact is to be omitted. In this case, no further
                intersection tests for this body will be made. By default true.
                \see World::RayCast
                */
                virtual bool OnContactAABB(const Body* body, const Collider* collider, void* userData)
                {
                    return true;
                }

                /**
                Ray cast contact callback.
                \return False if further intersection tests are to be omitted. By default true.
                */
                virtual RayCastProceedings OnContact(const Body* body, const Collider* collider, const Math::Intersection3f& intersection, void* userData)
                {
                    return RayCastProceedings::Continue;
                }

        };

        typedef std::shared_ptr<RayCastEventHandler> RayCastEventHandlerPtr;

        /**
        Ray cast intersection structure.
        \see RayCast
        */
        struct FORK_EXPORT RayCastIntersection
        {
            RayCastIntersection() = default;
            RayCastIntersection(
                const Body* body, const Collider* collider,
                const Math::Point3f& point, const Math::Vector3f& normal
            );

            const Body*     body        = nullptr;
            const Collider* collider    = nullptr;
            Math::Point3f   point;
            Math::Vector3f  normal;
        };

        //! Type for 'RayCastIntersection' lists.
        typedef std::vector<RayCastIntersection> RayCastIntersectionList;

        /* === Interface === */

        virtual ~World();

        /* --- Material --- */

        /**
        Creates new physics material.
        \see Material
        */
        virtual MaterialPtr CreateMaterial() = 0;
        //! Releases the specified physics material.
        virtual void ReleaseMaterial(const MaterialPtr& material);

        /**
        Sets the behavior of the two specified physics materials.
        \see Material::Behavior
        */
        virtual void SetupMaterialBehavior(
            Material* materialA, Material* materialB, const Material::Behavior& behavior
        ) = 0;

        /* --- Collider --- */

        virtual ColliderPtr CreateNullCollider              () = 0;
        virtual ColliderPtr CreateSphereCollider            (float radius, const Math::Matrix4f* offsetMatrix = nullptr) = 0;
        virtual ColliderPtr CreateBoxCollider               (const Math::Size3f& size, const Math::Matrix4f* offsetMatrix = nullptr) = 0;
        virtual ColliderPtr CreateConeCollider              (float radius, float height, const Math::Matrix4f* offsetMatrix = nullptr) = 0;
        virtual ColliderPtr CreateCapsuleCollider           (float radius, float height, const Math::Matrix4f* offsetMatrix = nullptr) = 0;
        virtual ColliderPtr CreateCylinderCollider          (float radius, float height, const Math::Matrix4f* offsetMatrix = nullptr) = 0;
        virtual ColliderPtr CreateChamferCylinderCollider   (float radius, float height, const Math::Matrix4f* offsetMatrix = nullptr) = 0;

        virtual ColliderPtr CreateConvexHullCollider(
            const void* vertexData, unsigned int numVertices, unsigned int vertexStride,
            const Math::Matrix4f* offsetMatrix = nullptr, float tolerance = 0.002f
        ) = 0;

        virtual ColliderPtr CreateMeshCollider(
            const void* vertexData, unsigned int numVertices, unsigned int vertexStride,
            const Math::Matrix4f* offsetMatrix = nullptr, bool optimize = true
        ) = 0;

        virtual ColliderPtr CreateMeshCollider(
            const void* vertexData, unsigned int numVertices, unsigned int vertexStride,
            const void* indexData, unsigned int numIndices, unsigned int indexStride,
            const Math::Matrix4f* offsetMatrix = nullptr, bool optimize = true
        ) = 0;

        /**
        Creates a mesh collider for the specifid geometry.
        \param[in] geometry Specifies the geometry hierarchy for which a (static) collider will be created.
        \param[in] offsetMatrix Optional offset matrix. By default null.
        \param[in] optimize Specifies whether to optimize the collider construction or not. By default true.
        */
        ColliderPtr CreateMeshCollider(const Scene::Geometry& geometry, const Math::Matrix4f* offsetMatrix = nullptr, bool optimize = true);

        template <class VtxT> ColliderPtr CreateConvexHullCollider(
            const std::vector<VtxT>& vertices, const Math::Matrix4f* offsetMatrix = nullptr, float tolerance = 0.002f)
        {
            return CreateConvexHull(
                vertices.data(), vertices.size(), sizeof(VtxT), offsetMatrix, tolerance
            );
        }

        template <class VtxT> ColliderPtr CreateMeshCollider(
            const std::vector<VtxT>& vertices, const Math::Matrix4f* offsetMatrix = nullptr, bool optimize = true)
        {
            return CreateMeshCollider(
                vertices.data(), vertices.size(), sizeof(VtxT), offsetMatrix, optimize
            );
        }

        template <class VtxT, class IdxT> ColliderPtr CreateMeshCollider(
            const std::vector<VtxT>& vertices, const std::vector<IdxT>& indices,
            const Math::Matrix4f* offsetMatrix = nullptr, bool optimize = true)
        {
            return CreateMeshCollider(
                vertices.data(), vertices.size(), sizeof(VtxT),
                indices.data(), indices.size(), sizeof(IdxT),
                offsetMatrix, optimize
            );
        }

        /* --- Bodies --- */

        virtual RigidBodyPtr CreateRigidBody(
            const ColliderPtr& collider, const Math::Matrix4f& initTransform = {}
        ) = 0;

        virtual StaticBodyPtr CreateStaticBody(
            const ColliderPtr& collider, const Math::Matrix4f& initTransform = {}
        ) = 0;

        /* --- Physics simulation --- */

        /**
        Advances the simulation by the specified amount of time.
        This function is the heart of physics simulation.
        \param[in] timeStep Specifies the amount of time (in seconds) to update the physics simulation.
        */
        virtual void Simulate(float timeStep) = 0;

        /* --- Intersection tests --- */

        /**
        Makes a ray cast intersection test with the entire physics world.
        \param[in] line Specifies the line (actually the 'ray') which is to be tested for intersection against the entire physics world.
        \param[in] eventHandler Specifies the event handler which will be called when an intersection occurs.
        If 'eventHandler' is null, the function call has no effect.
        \param[in] userData Optional user defined raw-pointer which will be passed to the event handler callbacks.
        \see RayCastEventHandler
        */
        virtual void RayCast(
            const Math::Line3f& line, RayCastEventHandler* eventHandler, void* userData = nullptr
        ) const = 0;

        /**
        Makes a ray cast intersection test with the entire physics world and returns all intersection results in an array list.
        \param[in] line Specifies the line (actually the 'ray') which is to be tested for intersection against the entire physics world.
        \param[in] sortList Specifies whether the intersection list is to be sorted or not.
        If true, the first entry is the closest entry to the line start point and the last entry is
        most far away from the line start point. By default true.
        \return List of all ray cast intersections.
        \see RayCastIntersection
        \see RayCastIntersectionList
        */
        virtual RayCastIntersectionList RayCast(const Math::Line3f& line, bool sortList = true) const;

        /**
        Makes a ray cast intersection test with the entire physics world and returns the first intersection.
        \param[in] line Specifies the line (actually the 'ray') which is to be tested for intersection against the entire physics world.
        \param[out] intersection Specifies the resulting ray-cast intersection. This will only be written if any intersection occured.
        \return True if any intersection occured. Otherwise false.
        \see RayCast(const Math::Line3f&)
        */
        virtual bool RayCast(const Math::Line3f& line, RayCastIntersection& intersection) const;

        /* === Inline functions === */

        inline const std::vector<MaterialPtr>& GetMaterials() const
        {
            return materials_;
        }

    protected:
        
        World() = default;

        std::vector<MaterialPtr> materials_;

    private:
        
        class StandardRayCastEventHandler;

};


} // /namespace Physics

} // /namespace Fork


#endif



// ========================