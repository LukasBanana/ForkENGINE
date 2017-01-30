/*
 * Simple 2D vertex header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SIMPLE_2D_VERTEX_H__
#define __FORK_SIMPLE_2D_VERTEX_H__


#include "Math/Core/Vector2.h"
#include "Video/BufferFormat/VertexFormat.h"
#include "Core/Export.h"


namespace Fork
{

namespace Video
{


/**
Simple 2D vertex structure with components:
2D coordinate and 2D texture coordinate.
\ingroup vertex_struct
*/
struct FORK_EXPORT Simple2DVertex
{
    Simple2DVertex() = default;
    Simple2DVertex(const Math::Point2f& coordinate, const Math::Vector2f& texCoordinate);

    /**
    Returns the vertex format for this vertex structure.
    \note The attributes will have the same name as the structure's data members.
    You may want to change them, to use different names in your vertex shader.
    */
    static VertexFormat Format();

    Math::Point2f   coord;      //!< 2D vertex coordinate.
    Math::Vector2f  texCoord;   //!< 2D texture coordinate.
};


} // /namespace Video

} // /namespace Fork


#endif



// ========================