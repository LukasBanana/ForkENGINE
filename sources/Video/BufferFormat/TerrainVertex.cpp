/*
 * Terrain vertex file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/BufferFormat/TerrainVertex.h"


namespace Fork
{

namespace Video
{


TerrainVertex::TerrainVertex(const Math::Point2f& coordinate) :
    coord { coordinate }
{
}

VertexFormat TerrainVertex::Format()
{
    VertexFormat format;
    format.SetupAttributes({ { "coord", RendererDataTypes::Float, 2 } });
    return format;
}


} // /namespace Video

} // /namespace Fork



// ========================