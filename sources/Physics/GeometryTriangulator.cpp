/*
 * Physics geometry triangulator file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GeometryTriangulator.h"
#include "Scene/Geometry/Node/Simple3DMeshGeometry.h"
#include "Scene/Geometry/Node/TangentSpaceMeshGeometry.h"
#include "Scene/Geometry/Node/CommonMeshGeometry.h"


namespace Fork
{

namespace Physics
{


void GeometryTriangulator::Triangulate(const Scene::Geometry& geometry, std::vector<Math::Triangle3f>& triangles)
{
    /* Store reference to triangle list */
    triangles_ = (&triangles);
    {
        /*
        Do a const cast. We promise here to not modify this object!
        But the scene visitor interface requires mutable nodes.
        This class is private, does not allow further inheritance (since 'GeometryTriangulator' is final)
        and won't modify the 'geometry' object, thus this const cast is safe!
        */
        auto mutableGeometry = const_cast<Scene::Geometry*>(&geometry);
        mutableGeometry->Visit(this);
    }
    triangles_ = nullptr;
}


/*
 * ======= Private: =======
 */

template <class Geom> void TriangulateMeshGeometry(Geom* node, std::vector<Math::Triangle3f>& triangles)
{
    auto offset = triangles.size();
    auto numIndices = node->indices.size();

    auto VertexCoord = [&node](size_t index)
    {
        return node->vertices[node->indices[index]].coord;
    };

    switch (node->primitiveType)
    {
        case Video::GeometryPrimitives::Triangles:
        {
            const auto numTriangles = numIndices / 3;
            triangles.resize(offset + numTriangles);

            for (size_t i = 0; i < numTriangles; ++i)
            {
                triangles[offset + i] = {
                    VertexCoord(i*3    ),
                    VertexCoord(i*3 + 1),
                    VertexCoord(i*3 + 2)
                };
            }
        }
        break;

        //TODO -> add more cases!!!
    }
}

void GeometryTriangulator::VisitSimple3DMeshGeometry(Scene::Simple3DMeshGeometry* node)
{
    TriangulateMeshGeometry(node, *triangles_);
}

void GeometryTriangulator::VisitTangentSpaceMeshGeometry(Scene::TangentSpaceMeshGeometry* node)
{
    TriangulateMeshGeometry(node, *triangles_);
}

void GeometryTriangulator::VisitCommonMeshGeometry(Scene::CommonMeshGeometry* node)
{
    TriangulateMeshGeometry(node, *triangles_);
}


} // /namespace Physics

} // /namespace Fork



// ========================