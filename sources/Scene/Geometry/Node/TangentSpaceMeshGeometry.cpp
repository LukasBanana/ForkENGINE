/*
 * Tangent space mesh geometry file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/Node/TangentSpaceMeshGeometry.h"
#include "Scene/Geometry/GeometryConverter.h"


namespace Fork
{

namespace Scene
{


void TangentSpaceMeshGeometry::Visit(SceneVisitor* visitor)
{
    visitor->VisitTangentSpaceMeshGeometry(this);
}

Geometry::Types TangentSpaceMeshGeometry::Type() const
{
    return Geometry::Types::TangentSpaceMesh;
}

void TangentSpaceMeshGeometry::AddVertex(
    const Math::Vector3f& coord, const Math::Vector3f& normal, const Math::Vector2f& texCoord)
{
    vertices.push_back({ coord, normal, texCoord, {}, {} });
}

void TangentSpaceMeshGeometry::AddVertex(
    const Math::Vector3f& coord, const Math::Vector3f& normal, const Math::Vector2f& texCoord,
    const Math::Vector3f& tangent, const Math::Vector3f& bitangent)
{
    vertices.push_back({ coord, normal, texCoord, tangent, bitangent });
}

void TangentSpaceMeshGeometry::GenerateTangentSpaceFlat(bool ignoreNormals)
{
    const auto numVerts = vertices.size();
    const auto stride   = sizeof(Video::TangentSpaceVertex);

    EnsureHardwareBuffer();
    GeometryConverter::GenerateTangentSpaceFlat(
        { &vertices[0].coord,     numVerts, stride },
        { &vertices[0].texCoord,  numVerts, stride },
        { &vertices[0].tangent,   numVerts, stride },
        { &vertices[0].bitangent, numVerts, stride },
        { &vertices[0].normal,    numVerts, stride },
        indices,
        ignoreNormals
    );
    UpdateVertexBuffer();
}


} // /namespace Scene

} // /namespace Fork



// ========================