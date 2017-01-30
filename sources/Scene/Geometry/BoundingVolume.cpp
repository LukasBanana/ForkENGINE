/*
 * Bouding volume file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/BoundingVolume.h"
#include "Math/Common/ExtMathFunctions.h"

#include <cmath>
#include <algorithm>


namespace Fork
{

namespace Scene
{


void BoundingVolume::DetermineType()
{
    if (Math::ComputeAABBVolume(box) < sphere.Volume())
        type = Types::Box;
    else
        type = Types::Sphere;
}

void BoundingVolume::SetupBox(const Math::AABB3f& boundingBox)
{
    /* Setup bounding box */
    type = Types::Box;
    box = boundingBox;

    /* Update bounding sphere */
    auto halfBoxSize = box.Size()*0.5f;
    sphere.point = box.Center();
    sphere.radius = std::sqrt(halfBoxSize.Max());
}

void BoundingVolume::SetupSphere(const Math::Sphere<>& boundingSphere)
{
    /* Setup bounding sphere */
    type = Types::Sphere;
    sphere = boundingSphere;

    /* Update bounding box */
    auto r = boundingSphere.radius;

    box.min = { -r, -r, -r };
    box.max = { r, r, r };

    box.min += sphere.point;
    box.max += sphere.point;
}


} // /namespace Scene

} // /namespace Fork



// ========================