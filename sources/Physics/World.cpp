/*
 * Physics world file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Physics/World.h"
#include "Core/STLHelper.h"
#include "Scene/Geometry/Node/Geometry.h"
#include "Scene/Node/DefaultSceneVisitor.h"
#include "GeometryTriangulator.h"

#include <algorithm>


namespace Fork
{

namespace Physics
{


/*
 * StandardRayCastIntersection class
 */

class World::StandardRayCastEventHandler : public World::RayCastEventHandler
{
    
    public:
        
        RayCastProceedings OnContact(const Body* body, const Collider* collider, const Math::Intersection3f& intersection, void* userData) override
        {
            /* Add ray-cast intersection to the list */
            auto intersectionListRef = reinterpret_cast<RayCastIntersectionList*>(userData);

            intersectionListRef->push_back(
                { body, collider, intersection.point, intersection.normal }
            );

            return RayCastProceedings::Continue;
        }

};


/*
 * RayCastIntersection structure
 */

World::RayCastIntersection::RayCastIntersection(
    const Body* body, const Collider* collider,
    const Math::Point3f& point, const Math::Vector3f& normal) :
        body    { body     },
        collider{ collider },
        point   { point    },
        normal  { normal   }
{
}



/*
 * World class
 */

World::~World()
{
}

void World::ReleaseMaterial(const MaterialPtr& material)
{
    RemoveFromList(materials_, material);
}

ColliderPtr World::CreateMeshCollider(
    const Scene::Geometry& geometry, const Math::Matrix4f* offsetMatrix, bool optimize)
{
    /* Create mesh collider from triangulated geometry */
    GeometryTriangulator triangulator;

    std::vector<Math::Triangle3f> triangles;
    triangulator.Triangulate(geometry, triangles);

    return CreateMeshCollider(triangles.data(), triangles.size()*3, sizeof(Math::Point3f), offsetMatrix, optimize);
}

World::RayCastIntersectionList World::RayCast(const Math::Line3f& line, bool sortList) const
{
    /* Fill intersection list */
    RayCastIntersectionList intersectionList;

    StandardRayCastEventHandler rayCastEventHandler;
    RayCast(line, &rayCastEventHandler, &intersectionList);

    /* Sort intersection list */
    if (sortList)
    {
        std::sort(
            intersectionList.begin(),
            intersectionList.end(),
            [&line](const RayCastIntersection& a, const RayCastIntersection& b)
            {
                return Math::DistanceSq(a.point, line.start) < Math::DistanceSq(b.point, line.start);
            }
        );
    }

    return intersectionList;
}

/**
TODO -> check if "Newton" will always return the NEAREST intersection
when the event handler is called!!!
*/
bool World::RayCast(const Math::Line3f& line, RayCastIntersection& intersection) const
{
    auto intersectionList = RayCast(line);
    
    if (!intersectionList.empty())
    {
        intersection = intersectionList.front();
        return true;
    }

    return false;
}


} // /namespace Physics

} // /namespace Fork



// ========================