/*
 * Cube geometry generator header
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


CylinderDescription::CylinderDescription(float radius, float height, unsigned int segments) :
    radius  { radius   },
    height  { height   },
    segments{ segments }
{
}

bool CylinderDescription::CompareSWO(const CylinderDescription& other) const
{
    RETURN_COMPARE_MEMBER_SWO( radius )
    RETURN_COMPARE_MEMBER_SWO( height )
    return segments < other.segments;
}


FORK_EXPORT void GenerateCylinder(GeometryType& geometry, const CylinderDescription& desc)
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

    float angle = 0.0f;

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
        coord.y = height*0.5f;
        texCoordMantle.y = 0.0f;

        texCoordCap.x = texCoordCap.x*0.5f + 0.5f;
        texCoordCap.y = texCoordCap.y*0.5f + 0.5f;

        geometry.AddVertex(coord, normal, texCoordMantle);
        geometry.AddVertex(coord, { 0, 1, 0 }, { texCoordCap.x, 1.0f - texCoordCap.y });

        /* Add bottom vertex */
        coord.y = -coord.y;
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
        geometry.AddIndices({ 0,6,4,  0,2,6 }, indexOffset);
        indexOffset += 4;
    }

    /* Generate indices for the top and bottom */
    indexOffset = indexBaseOffset;

    for (unsigned int i = 2; i < segments; ++i)
    {
        geometry.AddIndices({ 1, indexOffset + 5, indexOffset + 9 });
        geometry.AddIndices({ 3, indexOffset + 11, indexOffset + 7 });
        indexOffset += 4;
    }
}


} // /namespace GeometryGenerator

} // /namespace Scene

} // /namespace Fork



// ========================