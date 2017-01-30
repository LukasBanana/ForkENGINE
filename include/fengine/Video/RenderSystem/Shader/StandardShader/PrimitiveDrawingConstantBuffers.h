/*
 * Primitive drawing shader constant buffers header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PRIMITIVE_DRAWING_SHADER_CONSTANT_BUFFERS_H__
#define __FORK_PRIMITIVE_DRAWING_SHADER_CONSTANT_BUFFERS_H__


#include "Math/Core/Vector4.h"
#include "Math/Core/Matrix4.h"
#include "Video/Core/ColorRGBA.h"


namespace Fork
{

namespace Video
{

namespace StandardShader
{

//! Namespace for all "PrimitiveDrawing" shader constant buffer structures.
namespace PrimitiveDrawingShader
{

#include "Core/PackPush.h"


struct VertexConstBuffer
{
    static const size_t maxNumVertices = 32;

    Math::Matrix4f      wvpMatrix;
    Math::Vector4f      coords      [maxNumVertices];
    Video::ColorRGBAf   colors      [maxNumVertices];
    Math::Vector4f      texCoords   [maxNumVertices];
}
PACK_STRUCT;

struct PixelConstBuffer
{
    int texEnabled = 0;
    int __pad0[3];
}
PACK_STRUCT;


#include "Core/PackPop.h"

} // /namespace PrimitiveDrawingShader

} // /namespace StandardShader

} // /namespace Video

} // /namespace Fork


#endif



// ========================