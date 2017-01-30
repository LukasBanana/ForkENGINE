/*
 * Wire cube geometry generator header
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


/*
The vertices are generated as followes:

     0--------1
    /|       /|
   / |      / |
  3--------2  |
  |  4-----|--5
  | /      | /
  |/       |/
  7--------6
*/
FORK_EXPORT void GenerateWireCube(GeometryType& geometry, const CubeDescription& desc)
{
    const auto& r = desc.radius;

    geometry.Clear();
    geometry.primitiveType = Video::GeometryPrimitives::Lines;
    geometry.boundingVolume.SetupBox({ Math::Vector3f(-r), Math::Vector3f(r) });

    /* Generate vertices (top) */
    geometry.AddVertex({ -r,  r,  r });
    geometry.AddVertex({  r,  r,  r });
    geometry.AddVertex({  r,  r, -r });
    geometry.AddVertex({ -r,  r, -r });

    /* Generate vertices (bottom) */
    geometry.AddVertex({ -r, -r,  r });
    geometry.AddVertex({  r, -r,  r });
    geometry.AddVertex({  r, -r, -r });
    geometry.AddVertex({ -r, -r, -r });

    /* Generate indices */
    geometry.AddIndices({ 0,1,  3,2,  4,5,  7,6 }); // <-- Line indices in X direction.
    geometry.AddIndices({ 0,4,  1,5,  2,6,  3,7 }); // <-- Line indices in Y direction.
    geometry.AddIndices({ 3,0,  2,1,  6,5,  7,4 }); // <-- Line indices in Z direction.
}


} // /namespace GeometryGenerator

} // /namespace Scene

} // /namespace Fork



// ========================