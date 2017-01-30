/*
 * Simple 2D mesh geometry file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/Node/Simple2DMeshGeometry.h"


namespace Fork
{

namespace Scene
{


void Simple2DMeshGeometry::Visit(SceneVisitor* visitor)
{
    visitor->VisitSimple2DMeshGeometry(this);
}

Geometry::Types Simple2DMeshGeometry::Type() const
{
    return Geometry::Types::Simple2DMesh;
}

void Simple2DMeshGeometry::AddVertex(const Math::Point2f& coord, const Math::Vector2f& texCoord)
{
    vertices.push_back({ coord, texCoord });
}


} // /namespace Scene

} // /namespace Fork



// ========================