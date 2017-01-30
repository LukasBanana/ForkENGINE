/*
 * Common mesh geometry file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/Node/CommonMeshGeometry.h"
#include "Scene/Geometry/Modifier/GeometryModifier.h"


namespace Fork
{

namespace Scene
{


void CommonMeshGeometry::Visit(SceneVisitor* visitor)
{
    visitor->VisitCommonMeshGeometry(this);
}

Geometry::Types CommonMeshGeometry::Type() const
{
    return Geometry::Types::CommonMesh;
}

void CommonMeshGeometry::AddVertex(
    const Math::Vector3f& coord, const Math::Vector3f& normal, const Video::ColorRGBAf& color)
{
    vertices.push_back({ coord, normal, color });
}


} // /namespace Scene

} // /namespace Fork



// ========================