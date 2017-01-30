/*
 * Cube geometry generator header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/Generator/GeometryGenerator.h"


namespace Fork
{

namespace Scene
{

namespace GeometryGenerator
{


CubeDescription::CubeDescription(float radius) :
    radius{ radius }
{
}

bool CubeDescription::CompareSWO(const CubeDescription& other) const
{
    return radius < other.radius;
}


/*
The cube faces are generated in the same pattern
-> texture-coordinates are in brackets, the other numbers are the base indices:

(0,0)   (1,0)
    0---1
    |   |
    3---2
(0,1)   (1,1)
*/
FORK_EXPORT void GenerateCube(GeometryType& geometry, const CubeDescription& desc)
{
    auto indexOffset = static_cast<unsigned int>(geometry.vertices.size());
    const auto& r = desc.radius;

    auto EmitFace = [&]
    {
        /* Add indices for two triangles (0, 1, 2) and (0, 2, 3) */
        geometry.AddIndices({ 0, 1, 2, 0, 2, 3 }, indexOffset);

        /* Increase offset by 4 because we just added 4 vertices */
        indexOffset += 4;
    };

    /* Initialize geometry settings */
    geometry.primitiveType = Video::GeometryPrimitives::Triangles;
    geometry.boundingVolume.SetupBox({ Math::Vector3f(-r), Math::Vector3f(r) });

    /* Generate front face */
    geometry.AddVertex({ -r,  r, -r }, {  0,  0, -1 }, { 0, 0 });
    geometry.AddVertex({  r,  r, -r }, {  0,  0, -1 }, { 1, 0 });
    geometry.AddVertex({  r, -r, -r }, {  0,  0, -1 }, { 1, 1 });
    geometry.AddVertex({ -r, -r, -r }, {  0,  0, -1 }, { 0, 1 });
    EmitFace();

    /* Generate back face */
    geometry.AddVertex({  r,  r,  r }, {  0,  0,  1 }, { 0, 0 });
    geometry.AddVertex({ -r,  r,  r }, {  0,  0,  1 }, { 1, 0 });
    geometry.AddVertex({ -r, -r,  r }, {  0,  0,  1 }, { 1, 1 });
    geometry.AddVertex({  r, -r,  r }, {  0,  0,  1 }, { 0, 1 });
    EmitFace();

    /* Generate left face */
    geometry.AddVertex({ -r,  r,  r }, { -1,  0,  0 }, { 0, 0 });
    geometry.AddVertex({ -r,  r, -r }, { -1,  0,  0 }, { 1, 0 });
    geometry.AddVertex({ -r, -r, -r }, { -1,  0,  0 }, { 1, 1 });
    geometry.AddVertex({ -r, -r,  r }, { -1,  0,  0 }, { 0, 1 });
    EmitFace();

    /* Generate right face */
    geometry.AddVertex({  r,  r, -r }, {  1,  0,  0 }, { 0, 0 });
    geometry.AddVertex({  r,  r,  r }, {  1,  0,  0 }, { 1, 0 });
    geometry.AddVertex({  r, -r,  r }, {  1,  0,  0 }, { 1, 1 });
    geometry.AddVertex({  r, -r, -r }, {  1,  0,  0 }, { 0, 1 });
    EmitFace();

    /* Generate top face */
    geometry.AddVertex({ -r,  r,  r }, {  0,  1,  0 }, { 0, 0 });
    geometry.AddVertex({  r,  r,  r }, {  0,  1,  0 }, { 1, 0 });
    geometry.AddVertex({  r,  r, -r }, {  0,  1,  0 }, { 1, 1 });
    geometry.AddVertex({ -r,  r, -r }, {  0,  1,  0 }, { 0, 1 });
    EmitFace();

    /* Generate bottom face */
    geometry.AddVertex({ -r, -r, -r }, {  0, -1,  0 }, { 0, 0 });
    geometry.AddVertex({  r, -r, -r }, {  0, -1,  0 }, { 1, 0 });
    geometry.AddVertex({  r, -r,  r }, {  0, -1,  0 }, { 1, 1 });
    geometry.AddVertex({ -r, -r,  r }, {  0, -1,  0 }, { 0, 1 });
    EmitFace();
}


} // /namespace GeometryGenerator

} // /namespace Scene

} // /namespace Fork



// ========================