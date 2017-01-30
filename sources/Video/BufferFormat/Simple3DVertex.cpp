/*
 * Simple 3D vertex file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/BufferFormat/Simple3DVertex.h"


namespace Fork
{

namespace Video
{


Simple3DVertex::Simple3DVertex(
    const Math::Point3f& coordinate, const Math::Vector3f& vertexNormal, const Math::Vector2f& texCoordinate) :
        coord   { coordinate    },
        normal  { vertexNormal  },
        texCoord{ texCoordinate }
{
}

VertexFormat Simple3DVertex::Format()
{
    VertexFormat format;

    format.SetupAttributes(
        {
            { "coord",      RendererDataTypes::Float, 3 },
            { "normal",     RendererDataTypes::Float, 3 },
            { "texCoord",   RendererDataTypes::Float, 2 }
        }
    );

    return format;
}


} // /namespace Video

} // /namespace Fork



// ========================