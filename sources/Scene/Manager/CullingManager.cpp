/*
 * Culling manager file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Manager/CullingManager.h"
#include "Math/Common/Transform.h"

#include <array>


namespace Fork
{

namespace Scene
{


void CullingManager::SetupProjectionMatrix(const Projection& projection)
{
    projection_ = projection.GetMatrixLH();
}

void CullingManager::SetupViewMatrix(const Math::Matrix4f& matrix)
{
    viewProjection_ = projection_ * matrix;
    viewFrustum_.Build(viewProjection_);
}

void CullingManager::SetupWorldMatrix(const Math::Matrix4f& matrix)
{
    worldMatrix_ = matrix;
    worldViewProjection_ = viewProjection_ * matrix;
}

bool CullingManager::IsBoundingVolumeInsideFrustum(const BoundingVolume& boundingVolume) const
{
    switch (boundingVolume.type)
    {
        case BoundingVolume::Types::None:
            return true;
        case BoundingVolume::Types::Box:
            return IsBoundingBoxInsideFrustum(boundingVolume.box);
        case BoundingVolume::Types::Sphere:
            //return IsBoundingSphereInsideFrustum(boundingVolume.sphere);
            return IsBoundingBoxInsideFrustum(boundingVolume.box);
    }
    return !cullingEnabled;
}

bool CullingManager::IsBoundingBoxInsideFrustum(const Math::AABB3f& boundingBox) const
{
    if (!cullingEnabled)
        return true;

    const Math::Point3f& mi = boundingBox.min;
    const Math::Point3f& ma = boundingBox.max;

    /* Transform box corners into clip-space */
    std::array<Math::Point4f, 8> pointsCS =
    {
        ToClipSpace(mi.x, mi.y, mi.z),
        ToClipSpace(mi.x, mi.y, ma.z),
        ToClipSpace(mi.x, ma.y, mi.z),
        ToClipSpace(mi.x, ma.y, ma.z),
        ToClipSpace(ma.x, mi.y, mi.z),
        ToClipSpace(ma.x, mi.y, ma.z),
        ToClipSpace(ma.x, ma.y, mi.z),
        ToClipSpace(ma.x, ma.y, ma.z)
    };

    /* Test box corners against clip-space plane: NEAR */
    bool outside = true;

    for (const auto& point : pointsCS)
    {
        if (point.z > 0.0f)
        {
            outside = false;
            break;
        }
    }

    if (outside)
        return false;

    /* Test box corners against clip-space plane: LEFT */
    outside = true;

    for (const auto& point : pointsCS)
    {
        if (point.x > -point.w)
        {
            outside = false;
            break;
        }
    }

    if (outside)
        return false;

    /* Test box corners against clip-space plane: RIGHT */
    outside = true;

    for (const auto& point : pointsCS)
    {
        if (point.x < point.w)
        {
            outside = false;
            break;
        }
    }

    if (outside)
        return false;

    /* Test box corners against clip-space plane: TOP */
    outside = true;

    for (const auto& point : pointsCS)
    {
        if (point.y < point.w)
        {
            outside = false;
            break;
        }
    }

    if (outside)
        return false;

    /* Test box corners against clip-space plane: BOTTOM */
    outside = true;

    for (const auto& point : pointsCS)
    {
        if (point.y > -point.w)
        {
            outside = false;
            break;
        }
    }

    if (outside)
        return false;

    /* Test box corners against clip-space plane: FAR */
    outside = true;

    for (const auto& point : pointsCS)
    {
        if (point.z < point.w)
        {
            outside = false;
            break;
        }
    }

    if (outside)
        return false;

    return true;
}

//!TODO! -> this is incomplete!!!
bool CullingManager::IsBoundingSphereInsideFrustum(const Math::Sphere<>& boundingSphere) const
{
    /* Make culling test with inverse view frustum (to support non-uniform scaled spheres) */
    //return !cullingEnabled || viewFrustum_.IsSphereInside(boundingSphere);
    return true;//!!!
}


/*
 * ======= Private: =======
 */

Math::Point4f CullingManager::ToClipSpace(float x, float y, float z) const
{
    return worldViewProjection_ * Math::Point4f(x, y, z);
}


} // /namespace Scene

} // /namespace Fork



// ========================