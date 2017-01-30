/*
 * Cone geometry generator header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/Generator/GeometryGenerator.h"
#include "Core/AuxiliaryMacros.h"

#include <algorithm>


namespace Fork
{

namespace Scene
{

namespace GeometryGenerator
{


ConeDescription::ConeDescription(float radius, float height, unsigned int segments) :
    radius  { radius   },
    height  { height   },
    segments{ segments }
{
}

bool ConeDescription::CompareSWO(const ConeDescription& other) const
{
    RETURN_COMPARE_MEMBER_SWO( radius )
    RETURN_COMPARE_MEMBER_SWO( height )
    return segments < other.segments;
}


FORK_EXPORT void GenerateCone(GeometryType& geometry, const ConeDescription& desc)
{
    const auto indexBaseOffset = static_cast<unsigned int>(geometry.vertices.size());

    /* Map input parameters */
    float radius = desc.radius;
    float height = desc.height;
    unsigned int segments = std::max(3u, desc.segments);

    float steps = 1.0f / static_cast<float>(segments);
    float angleSteps = (Math::pi*2.0f) * steps;

    /* Initialize geometry settings */
    geometry.primitiveType = Video::GeometryPrimitives::Triangles;
    geometry.boundingVolume.SetupBox({ { -radius, -height*0.5f, -radius }, { radius, height*0.5f, radius } });

    /* Generate all vertices (for the mantle, top and bottom ) */
    Math::Vector3f coord, normal;
    Math::Vector2f texCoordMantle, texCoordCap;

    const Math::Vector3f tip { 0, height*0.5f, 0 };

    float angle = 0.0f;
    coord.y = -height*0.5f;

    for (unsigned int i = 0; i <= segments; ++i)
    {
        /* Compute X- and Z coordinates */
        texCoordCap.x = std::sin(angle);
        texCoordCap.y = std::cos(angle);

        coord.x = texCoordCap.x * radius;
        coord.z = texCoordCap.y * radius;

        texCoordMantle.x = static_cast<float>(i) * steps;

        /* Compute normal vector */
        normal = { coord.x, 0.0f, coord.z };
        normal.Normalize();

        /* Add top vertex */
        texCoordMantle.y = 0.0f;

        texCoordCap.x = texCoordCap.x*0.5f + 0.5f;
        texCoordCap.y = texCoordCap.y*0.5f + 0.5f;

        geometry.AddVertex(tip, { 0, 1, 0 }, texCoordMantle);

        /* Add bottom vertex */
        texCoordMantle.y = 1.0f;

        geometry.AddVertex(coord, normal, texCoordMantle);
        geometry.AddVertex(coord, { 0, -1, 0 }, texCoordCap);

        /* Increase angle for the next iteration */
        angle += angleSteps;
    }

    /* Generate indices for the mantle */
    unsigned int indexOffset = indexBaseOffset;

    for (unsigned int i = 0; i < segments; ++i)
    {
        geometry.AddIndices({ 0, 1, 4 }, indexOffset);
        indexOffset += 3;
    }

    /* Generate indices for the bottom */
    indexOffset = 0;

    for (unsigned int i = 0; i + 2 < segments; ++i)
    {
        geometry.AddIndices({ 2, indexOffset + 8, indexOffset + 5 }, indexBaseOffset);
        indexOffset += 3;
    }
}


} // /namespace GeometryGenerator

} // /namespace Scene

} // /namespace Fork



// ========================