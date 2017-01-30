/*
 * Billboard geometry file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/Node/BillboardGeometry.h"


namespace Fork
{

namespace Scene
{


static const float blbrdBoxSize = 1.0f;

BillboardGeometry::~BillboardGeometry()
{
}

void BillboardGeometry::Visit(SceneVisitor* visitor)
{
    visitor->VisitBillboardGeometry(this);
}

Geometry::Types BillboardGeometry::Type() const
{
    return Geometry::Types::Billboard;
}

void BillboardGeometry::ComputeBoundingVolume()
{
    /* Set bounding box with billboard's quad dimension */
    boundingVolume.box = { { -blbrdBoxSize, -blbrdBoxSize, 0 }, { blbrdBoxSize, blbrdBoxSize, 0 } };
    
    /*
    Compute billboard bounding radius with:
    r := sqrt(a^2 + b^2) = sqrt(blbrdBoxSize^2 + blbrdBoxSize^2) = sqrt((blbrdBoxSize*blbrdBoxSize)*2)
    */
    boundingVolume.sphere.radius = std::sqrt((blbrdBoxSize*blbrdBoxSize)*2);
    boundingVolume.sphere.point = Math::Point3f();

    /* Determine bounding volume type */
    boundingVolume.DetermineType();
}


} // /namespace Scene

} // /namespace Fork



// ========================