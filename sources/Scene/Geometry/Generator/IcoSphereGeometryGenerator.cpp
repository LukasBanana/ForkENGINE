/*
 * Ico-sphere geometry generator header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/Generator/GeometryGenerator.h"
#include "Math/Common/CoordinateSpace.h"
#include "Core/AuxiliaryMacros.h"


namespace Fork
{

namespace Scene
{

namespace GeometryGenerator
{


IcoSphereDescription::IcoSphereDescription(float radius, unsigned int tessellation) :
    radius      { radius       },
    tessellation{ tessellation }
{
}

bool IcoSphereDescription::CompareSWO(const IcoSphereDescription& other) const
{
    RETURN_COMPARE_MEMBER_SWO(radius)
    return tessellation < other.tessellation;
}


using Math::Vector3f;

/*
First of all, an ordinary icosahedron will be generated.
Then each triangle will be sub-divided (specified by 'tessellation').
*/
FORK_EXPORT void GenerateIcoSphere(GeometryType& geometry, const IcoSphereDescription& desc)
{
    /* Map input parameters */
    float radius = desc.radius;
    unsigned int tessellation = desc.tessellation;

    auto indexOffset = static_cast<unsigned int>(geometry.indices.size());

    const float invRowColTessFactor = 1.0f / static_cast<float>(tessellation + 1);

    /* Initialize geometry settings */
    geometry.primitiveType = Video::GeometryPrimitives::Triangles;
    geometry.boundingVolume.SetupSphere(Math::Sphere<>(radius));

    /* Construct ordinary icosahedron */
    static const float offset = 0.30902f; // <-- Magic number offset to construct the icosahedron

    const Vector3f baseVertices[] =
    {
        { 0,  .5f, -.5f - offset },
        { 0,  .5f,  .5f + offset },
        { 0, -.5f,  .5f + offset },
        { 0, -.5f, -.5f - offset },
        
        { -.5f,  .5f + offset, 0 },
        {  .5f,  .5f + offset, 0 },
        {  .5f, -.5f - offset, 0 },
        { -.5f, -.5f - offset, 0 },
        
        { -.5f - offset, 0, -.5f },
        { -.5f - offset, 0,  .5f },
        {  .5f + offset, 0,  .5f },
        {  .5f + offset, 0, -.5f },
    };

    /*
    This function will generate the vertices and indices for one tessellated triangle:
    
      /\         /\    <-- first row
     /  \   ->  /\/\   <-- second row
    ------     ------

    It will iterate over all 'rows', and then over all sub-triangles.
    - For each row, the first vertex in that row, will be added outside the column-for-loop.
    - For each row, only the bottom vertices will be added.
    */
    auto TessellateTriangle = [&](unsigned int i0, unsigned int i1, unsigned int i2)
    {
        const Vector3f ctrlPoints[3] = { baseVertices[i0], baseVertices[i1], baseVertices[i2] };

        const auto vecU = ctrlPoints[2] - ctrlPoints[0];
        const auto vecV = ctrlPoints[1] - ctrlPoints[2];

        /*
        This function interpolates the control points at the specified location (row and column).
        This is done by computing the interpolation factors 'u' and 'v'.
        */
        auto LerpVertex = [&](unsigned int row, unsigned int col) -> Vector3f
        {
            float u = static_cast<float>(row) * invRowColTessFactor;
            float v = static_cast<float>(col) * invRowColTessFactor;

            return ctrlPoints[0] + vecU*u + vecV*v;
        };

        unsigned int offsetTop = indexOffset, offsetBottom = indexOffset + 1;

        /* Add first vertex (the main triangle's top vertex) */
        geometry.AddVertex(ctrlPoints[0]);

        for (unsigned int row = 0; row <= tessellation; ++row)
        {
            /* Add first vertex for this row */
            geometry.AddVertex(LerpVertex(row + 1, 0));

            for (unsigned int col = 0; col <= row; ++col)
            {
                /* Add last vertex for next sub-triangle */
                geometry.AddVertex(LerpVertex(row + 1, col + 1));
                
                /*
                Add indices for the left triangle of the following ASCII art:
                   ____
                  /\  /
                 /__\/

                */
                geometry.AddIndices({ offsetTop + col, offsetBottom + col + 1, offsetBottom + col });

                if (col > 0)
                {
                    /* Add indices for the right triangle of the above ASCII art */
                    geometry.AddIndices({ offsetTop + col - 1, offsetTop + col, offsetBottom + col });
                }
            }

            /* Increase temporary index offsets */
            offsetTop += (row + 1);
            offsetBottom += (row + 2);
        }

        /*
        Increase index offset (in closed form, determined by http://www.wolframalpha.com).
        This is equivalent to the following code:
        {
            for (unsigned int i = 0; i <= tessellation + 1; ++i)
                indexOffset += (i + 1);
        }
        */
        const unsigned int n = tessellation + 1;
        indexOffset += (n*n + 3*n + 2) / 2;
    };

    /* Generate all tessellated triangles */
    TessellateTriangle( 0,  4,  5);
    TessellateTriangle( 0,  5, 11);
    TessellateTriangle( 3, 11,  6);
    TessellateTriangle( 3,  7,  8);
    TessellateTriangle( 3,  6,  7);
    TessellateTriangle( 0, 11,  3);
    TessellateTriangle( 0,  3,  8);
    TessellateTriangle( 8,  4,  0);
    TessellateTriangle( 5, 10, 11);
    TessellateTriangle( 5,  1, 10);
    TessellateTriangle( 6, 11, 10);
    TessellateTriangle( 9,  8,  7);
    TessellateTriangle( 1,  4,  9);
    TessellateTriangle( 1,  5,  4);
    TessellateTriangle( 6,  2,  7);
    TessellateTriangle( 4,  8,  9);
    TessellateTriangle(10,  1,  2);
    TessellateTriangle( 2,  1,  9);
    TessellateTriangle(10,  2,  6);
    TessellateTriangle( 9,  7,  2);

    /* Setup normals, texture-coordinates and  */
    for (auto& vertex : geometry.vertices)
    {
        /* Setup normal from coordinate and normalize it */
        vertex.coord.Normalize();
        vertex.normal = vertex.coord;

        /* Setup texture coordiante by converting the cartesian-coordinate to a spherical-coordinate */
        Math::SpherePointf point;
        Math::ConvertToSphericalCoordiantes(vertex.coord, point);

        vertex.texCoord.x = (point.theta + Math::pi) / (Math::pi*2.0f);
        vertex.texCoord.y = (point.phi) / (Math::pi*2.0f);

        /* Resize coordinate to radius */
        vertex.coord *= radius;
    }
}


} // /namespace GeometryGenerator

} // /namespace Scene

} // /namespace Fork



// ========================