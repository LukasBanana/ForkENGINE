/*
 * Terrain vertex header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TERRAIN_VERTEX_H__
#define __FORK_TERRAIN_VERTEX_H__


#include "Math/Core/Vector2.h"
#include "Video/BufferFormat/VertexFormat.h"
#include "Core/Export.h"


namespace Fork
{

namespace Video
{


/**
Terrain vertex structure with components: 2D coordinate.
A terrain vertex only needs a 2D coordinate, because the actual vertex
transformation is done in the vertex shader with a height field texture.
\ingroup vertex_struct
*/
struct FORK_EXPORT TerrainVertex
{
    TerrainVertex() = default;
    TerrainVertex(const Math::Point2f& coordinate);

    /**
    Returns the vertex format for this vertex structure.
    \note The attributes will have the same name as the structure's data members.
    You may want to change them, to use different names in your vertex shader.
    */
    static VertexFormat Format();

    Math::Point2f coord; //!< 2D vertex coordinate.
};


} // /namespace Video

} // /namespace Fork


#endif



// ========================