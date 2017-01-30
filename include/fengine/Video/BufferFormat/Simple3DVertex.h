/*
 * Simple 3D vertex header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SIMPLE_3D_VERTEX_H__
#define __FORK_SIMPLE_3D_VERTEX_H__


#include "Math/Core/Vector3.h"
#include "Math/Core/Vector2.h"
#include "Video/BufferFormat/VertexFormat.h"
#include "Core/Export.h"


namespace Fork
{

namespace Video
{


/**
Simple 3D vertex structure with components:
3D coordinate, normal vector and 2D texture coordinate.
\ingroup vertex_struct
*/
struct FORK_EXPORT Simple3DVertex
{
    Simple3DVertex() = default;
    Simple3DVertex(const Math::Point3f& coordinate, const Math::Vector3f& vertexNormal, const Math::Vector2f& texCoordinate);

    /**
    Returns the vertex format for this vertex structure.
    \note The attributes will have the same name as the structure's data members.
    You may want to change them, to use different names in your vertex shader.
    */
    static VertexFormat Format();

    Math::Point3f   coord;      //!< 3D vertex coordinate.
    Math::Vector3f  normal;     //!< Normal vector.
    Math::Vector2f  texCoord;   //!< 2D texture coordinate.
};


} // /namespace Video

} // /namespace Fork


#endif



// ========================