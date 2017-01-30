/*
 * Simple 2D vertex file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/BufferFormat/Simple2DVertex.h"


namespace Fork
{

namespace Video
{


Simple2DVertex::Simple2DVertex(
    const Math::Point2f& coordinate, const Math::Vector2f& texCoordinate) :
        coord   { coordinate    },
        texCoord{ texCoordinate }
{
}

VertexFormat Simple2DVertex::Format()
{
    VertexFormat format;

    format.SetupAttributes(
        {
            { "coord",      RendererDataTypes::Float, 2 },
            { "texCoord",   RendererDataTypes::Float, 2 }
        }
    );

    return format;
}


} // /namespace Video

} // /namespace Fork



// ========================