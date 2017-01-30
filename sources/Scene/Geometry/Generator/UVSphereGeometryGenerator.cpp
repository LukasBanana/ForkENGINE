/*
 * UV-sphere geometry generator header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/Generator/GeometryGenerator.h"
#include "Math/Common/CoordinateSpace.h"
#include "Core/AuxiliaryMacros.h"

#include <algorithm>


namespace Fork
{

namespace Scene
{

namespace GeometryGenerator
{


UVSphereDescription::UVSphereDescription(float radius, unsigned int segmentsU, unsigned int segmentsV) :
    radius      { radius    },
    segmentsU   { segmentsU },
    segmentsV   { segmentsV }
{
}
UVSphereDescription::UVSphereDescription(float radius, unsigned int segments) :
    UVSphereDescription(radius, segments*2, segments)
{
}

bool UVSphereDescription::CompareSWO(const UVSphereDescription& other) const
{
    RETURN_COMPARE_MEMBER_SWO( radius    )
    RETURN_COMPARE_MEMBER_SWO( segmentsU )
    return segmentsV < other.segmentsV;
}


FORK_EXPORT void GenerateUVSphere(GeometryType& geometry, const UVSphereDescription& desc)
{
    /* Map input parameters */
    float radius = desc.radius;
    unsigned int segmentsU = std::max(3u, desc.segmentsU);
    unsigned int segmentsV = std::max(2u, desc.segmentsV);

    float stepsU = 1.0f / static_cast<float>(segmentsU);
    float stepsV = 1.0f / static_cast<float>(segmentsV);

    const auto indexBaseOffset = static_cast<unsigned int>(geometry.indices.size());

    /* Initialize geometry settings */
    geometry.primitiveType = Video::GeometryPrimitives::Triangles;
    geometry.boundingVolume.SetupSphere(Math::Sphere<>(radius));

    /* Generate all vertices */
    Math::SpherePointf point(radius, 0, 0);
    Math::Vector3f coord, normal;
    Math::Vector2f texCoord;

    for (unsigned int v = 0; v <= segmentsV; ++v)
    {
        texCoord.y = static_cast<float>(v) * stepsV;
        point.theta = texCoord.y * Math::pi;
        
        for (unsigned int u = 0; u <= segmentsU; ++u)
        {
            texCoord.x = static_cast<float>(u) * stepsU;
            point.phi = texCoord.x * Math::pi*2.0f;

            /* Convert spherical coordinate into cartesian coordinate */
            Math::ConvertToCartesianCoordiantes(coord, point);

            /* Setup normal vector */
            normal = coord;
            normal.Normalize();

            /* Add new vertex */
            geometry.AddVertex(coord, normal, texCoord);
        }
    }

    /* Generate all indices */
    unsigned int i0, i1, i2, i3;

    for (unsigned int v = 0; v < segmentsV; ++v)
    {
        for (unsigned int u = 0; u < segmentsU; ++u)
        {
            /* Compute indices for current face */
            i0 = v*(segmentsU + 1) + u;
            i1 = v*(segmentsU + 1) + (u + 1);

            i2 = (v + 1)*(segmentsU + 1) + (u + 1);
            i3 = (v + 1)*(segmentsU + 1) + u;

            /* Add new indices */
            geometry.AddIndices({ i0, i1, i2 }, indexBaseOffset);
            geometry.AddIndices({ i0, i2, i3 }, indexBaseOffset);
        }
    }
}


} // /namespace GeometryGenerator

} // /namespace Scene

} // /namespace Fork



// ========================