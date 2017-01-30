/*
 * LOD geometry file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/Node/LODGeometry.h"
#include "Scene/Geometry/GeometryConverter.h"
#include "Math/Core/BaseMath.h"
#include "Math/Core/MathConstants.h"

#include <algorithm>


namespace Fork
{

namespace Scene
{


void LODGeometry::Visit(SceneVisitor* visitor)
{
    visitor->VisitLODGeometry(this);
}

Geometry::Types LODGeometry::Type() const
{
    return Geometry::Types::LOD;
}

void LODGeometry::ComputeBoundingVolume()
{
    GeometryConverter::ComputeBoundingVolume(boundingVolume, lodGeometries);
}

Geometry* LODGeometry::SelectLOD(float distance) const
{
    if (lodGeometries.empty())
        return nullptr;

    /* Check if distance is out of range */
    if (distance <= minDistance_)
        return lodGeometries.front().get();
    if (distance >= maxDistance_)
        return lodGeometries.back().get();

    /* Transform distance from [min .. max] to [0 .. 1] */
    distance = Math::TransformSaturate(distance, minDistance_, maxDistance_);

    /* Transform from linear to non-linear */
    distance = std::pow(distance, threshold_);

    /* Get geometry index from transformed distance */
    const size_t maxIndex = lodGeometries.size() - 1;
    const size_t index = static_cast<size_t>(distance * maxIndex);

    return lodGeometries[index].get();
}

void LODGeometry::SetupRange(float minDistance, float maxDistance)
{
    minDistance_ = std::max(0.0f, minDistance);
    maxDistance_ = std::max(minDistance_ + Math::epsilon, maxDistance);
}

void LODGeometry::SetupThreshold(float threshold)
{
    threshold_ = std::max(Math::epsilon, threshold);
}


} // /namespace Scene

} // /namespace Fork



// ========================