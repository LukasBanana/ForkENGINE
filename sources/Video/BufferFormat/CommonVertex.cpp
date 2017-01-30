/*
 * Common vertex file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/BufferFormat/CommonVertex.h"


namespace Fork
{

namespace Video
{


CommonVertex::CommonVertex(
    const Math::Point3f& coordinate, const Math::Vector3f& vertexNormal, const Video::ColorRGBAf& vertexColor) :
        coord   { coordinate   },
        normal  { vertexNormal },
        color   { vertexColor  }
{
}

VertexFormat CommonVertex::Format()
{
    VertexFormat format;

    format.SetupAttributes(
        {
            { "coord",      RendererDataTypes::Float, 3 },
            { "normal",     RendererDataTypes::Float, 3 },
            { "color",      RendererDataTypes::Float, 4 },
            { "texCoord",   RendererDataTypes::Float, 4 },
            { "texCoord",   RendererDataTypes::Float, 4 },
            { "texCoord",   RendererDataTypes::Float, 4 },
            { "texCoord",   RendererDataTypes::Float, 4 }
        }
    );

    return format;
}


} // /namespace Video

} // /namespace Fork



// ========================