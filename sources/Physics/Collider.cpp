/*
 * Physics collider file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Physics/Collider.h"
#include "Math/Common/Transform.h"


namespace Fork
{

namespace Physics
{


Collider::~Collider()
{
}

bool Collider::RayCast(
    const Math::Matrix4f& worldMatrix, const Math::Line3f& line, Math::Intersection3f& intersection) const
{
    /* Compute inverse line with specified world matrix */
    auto invWorldMatrix = worldMatrix.Inverse();
    auto invLine = Math::Transform(invWorldMatrix, line);

    /* Make intersection tests with inverse line */
    if (!RayCast(invLine, intersection))
        return false;

    /* Transform intersection point back to world space */
    intersection = Math::Transform(worldMatrix, intersection);

    return true;
}


} // /namespace Physics

} // /namespace Fork



// ========================