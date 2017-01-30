/*
 * Geometry auto UV map modifier file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Geometry/Modifier/GeometryAutoUVMapModifier.h"
#include "Scene/Geometry/Node/Simple2DMeshGeometry.h"
#include "Scene/Geometry/Node/Simple3DMeshGeometry.h"
#include "Scene/Geometry/Node/TangentSpaceMeshGeometry.h"
#include "Math/Common/ExtMathFunctions.h"


namespace Fork
{

namespace Scene
{


/*
 * ======= Private: =======
 */

void GeometryAutoUVMapModifier::VisitSimple2DMeshGeometry(Simple2DMeshGeometry* node)
{
    for (auto& vert : node->vertices)
        GenerateTexCoord({ vert.coord.x, vert.coord.y, 0.0f }, { 0, 0, -1 }, vert.texCoord);
    node->UpdateVertexBuffer();
}

void GeometryAutoUVMapModifier::VisitSimple3DMeshGeometry(Simple3DMeshGeometry* node)
{
    for (auto& vert : node->vertices)
        GenerateTexCoord(vert.coord, vert.normal, vert.texCoord);
    node->UpdateVertexBuffer();
}

void GeometryAutoUVMapModifier::VisitTangentSpaceMeshGeometry(TangentSpaceMeshGeometry* node)
{
    for (auto& vert : node->vertices)
        GenerateTexCoord(vert.coord, vert.normal, vert.texCoord);
    node->UpdateVertexBuffer();
}

void GeometryAutoUVMapModifier::GenerateTexCoord(
    Math::Point3f coord, const Math::Vector3f& normal, Math::Vector2f& texCoord)
{
    coord = uvMapTransform * coord;

    auto x = coord.x;
    auto y = coord.y;
    auto z = coord.z;

    switch (Math::DominantAxis(normal))
    {
        case Math::AxisDirections::XPositive:
            texCoord = TexCoord(z, y);
            break;
        case Math::AxisDirections::XNegative:
            texCoord = TexCoord(-z, y);
            break;
        case Math::AxisDirections::YPositive:
            texCoord = TexCoord(x, z);
            break;
        case Math::AxisDirections::YNegative:
            texCoord = TexCoord(x, -z);
            break;
        case Math::AxisDirections::ZPositive:
            texCoord = TexCoord(-x, y);
            break;
        case Math::AxisDirections::ZNegative:
            texCoord = TexCoord(x, y);
            break;
    }
}

Math::Vector2f GeometryAutoUVMapModifier::TexCoord(float u, float v) const
{
    return uvMapOffset + uvMapScale * Math::Vector2f(u, v);
}


} // /namespace Scene

} // /namespace Fork



// ========================