/*
 * Composition geometry file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/Node/CompositionGeometry.h"
#include "Scene/Geometry/GeometryConverter.h"


namespace Fork
{

namespace Scene
{


void CompositionGeometry::Visit(SceneVisitor* visitor)
{
    visitor->VisitCompositionGeometry(this);
}

Geometry::Types CompositionGeometry::Type() const
{
    return Geometry::Types::Composition;
}

void CompositionGeometry::ComputeBoundingVolume()
{
    GeometryConverter::ComputeBoundingVolume(boundingVolume, subGeometries);
}


} // /namespace Scene

} // /namespace Fork



// ========================