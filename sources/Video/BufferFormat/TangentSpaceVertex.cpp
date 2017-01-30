/*
 * Tangent sapce vertex file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/BufferFormat/TangentSpaceVertex.h"
#include "Math/Core/Vector2.h"
#include "Core/Export.h"


namespace Fork
{

namespace Video
{


TangentSpaceVertex::TangentSpaceVertex(
    const Math::Point3f& coordinate, const Math::Vector3f& vtxNormal, const Math::Vector2f& texCoordinate,
    const Math::Vector3f& vtxTangent, const Math::Vector3f& vtxBitangent) :
        coord       { coordinate    },
        normal      { vtxNormal     },
        texCoord    { texCoordinate },
        tangent     { vtxTangent    },
        bitangent   { vtxBitangent  }
{
}

VertexFormat TangentSpaceVertex::Format()
{
    VertexFormat format;

    format.SetupAttributes(
        {
            { "coord",      RendererDataTypes::Float, 3 },
            { "normal",     RendererDataTypes::Float, 3 },
            { "texCoord",   RendererDataTypes::Float, 2 },
            { "tangent",    RendererDataTypes::Float, 3 },
            { "bitangent",  RendererDataTypes::Float, 3 },
        }
    );

    return format;
}


} // /namespace Video

} // /namespace Fork



// ========================