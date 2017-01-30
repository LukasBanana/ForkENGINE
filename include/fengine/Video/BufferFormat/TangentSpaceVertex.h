/*
 * Tangent sapce vertex header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_TANGENT_SPACE_VERTEX_H__
#define __FORK_TANGENT_SPACE_VERTEX_H__


#include "Math/Core/Vector3.h"
#include "Math/Core/Vector2.h"
#include "Video/BufferFormat/VertexFormat.h"
#include "Core/Export.h"


namespace Fork
{

namespace Video
{


/**
Tangent space vertex structure with components:
3D coordinate, normal vector, tangent vector, bitangent vector (often called "binormal") and 2D texture coordinate.
\ingroup vertex_struct
*/
struct FORK_EXPORT TangentSpaceVertex
{
    TangentSpaceVertex() = default;
    TangentSpaceVertex(
        const Math::Point3f& coordinate, const Math::Vector3f& vtxNormal, const Math::Vector2f& texCoordinate,
        const Math::Vector3f& vtxTangent, const Math::Vector3f& vtxBitangent
    );

    /**
    Returns the vertex format for this vertex structure.
    \note The attributes will have the same name as the structure's data members.
    You may want to change them, to use different names in your vertex shader.
    */
    static VertexFormat Format();

    Math::Point3f   coord;      //!< 3D vertex coordinate.
    Math::Vector3f  normal;     //!< Normal vector.
    Math::Vector2f  texCoord;   //!< 2D texture coordinate.
    Math::Vector3f  tangent;    //!< Tangent vector.
    Math::Vector3f  bitangent;  //!< Bitangent vector (also called "binormal").
};


} // /namespace Video

} // /namespace Fork


#endif



// ========================