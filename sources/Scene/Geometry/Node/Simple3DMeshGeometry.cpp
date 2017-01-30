/*
 * Simple 3D mesh geometry file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/Node/Simple3DMeshGeometry.h"


namespace Fork
{

namespace Scene
{


void Simple3DMeshGeometry::Visit(SceneVisitor* visitor)
{
    visitor->VisitSimple3DMeshGeometry(this);
}

Geometry::Types Simple3DMeshGeometry::Type() const
{
    return Geometry::Types::Simple3DMesh;
}

void Simple3DMeshGeometry::AddVertex(
    const Math::Point3f& coord, const Math::Vector3f& normal, const Math::Vector2f& texCoord)
{
    vertices.push_back({ coord, normal, texCoord });
}

void Simple3DMeshGeometry::GenerateNormalsFlat()
{
    EnsureHardwareBuffer();
    GeometryConverter::GenerateNormalsFlat(
        { &vertices[0].coord, vertices.size(), sizeof(Video::Simple3DVertex) },
        { &vertices[0].normal, vertices.size(), sizeof(Video::Simple3DVertex) },
        indices
    );
    UpdateVertexBuffer();
}


} // /namespace Scene

} // /namespace Fork



// ========================