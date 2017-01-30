/*
 * Common vertex header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_COMMON_VERTEX_H__
#define __FORK_COMMON_VERTEX_H__


#include "Math/Core/Vector3.h"
#include "Math/Core/Vector4.h"
#include "Video/Core/ColorRGBA.h"
#include "Video/BufferFormat/VertexFormat.h"
#include "Core/Export.h"


namespace Fork
{

namespace Video
{


/**
Common vertex structure with components:
3D coordinate, normal vector and 2D texture coordinate.
\ingroup vertex_struct
*/
struct FORK_EXPORT CommonVertex
{
    CommonVertex() = default;
    CommonVertex(const Math::Point3f& coordinate, const Math::Vector3f& vertexNormal, const Video::ColorRGBAf& vertexColor);

    /**
    Returns the vertex format for this vertex structure.
    \note The attributes will have the same name as the structure's data members.
    You may want to change them, to use different names in your vertex shader.
    */
    static VertexFormat Format();

    Math::Point3f       coord;          //!< 3D vertex coordinate.
    Math::Vector3f      normal;         //!< Normal vector.
    Video::ColorRGBAf   color;          //!< RGBA color.
    Math::Vector4f      texCoord[4];    //!< 4D texture coordinates for general purposes.
};


} // /namespace Video

} // /namespace Fork


#endif



// ========================