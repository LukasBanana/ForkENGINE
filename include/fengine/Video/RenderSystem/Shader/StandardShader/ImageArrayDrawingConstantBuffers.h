/*
 * Image array drawing shader constant buffers header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IMAGE_ARRAY_DRAWING_SHADER_CONSTANT_BUFFERS_H__
#define __FORK_IMAGE_ARRAY_DRAWING_SHADER_CONSTANT_BUFFERS_H__


#include "Math/Core/Vector4.h"
#include "Math/Core/Matrix4.h"
#include "Video/Core/ColorRGBA.h"


namespace Fork
{

namespace Video
{

namespace StandardShader
{

//! Namespace for all "ImageArrayDrawing" shader constant buffer structures.
namespace ImageArrayDrawingShader
{

#include "Core/PackPush.h"


struct MainConstBuffer
{
    Math::Matrix4f      wvpMatrix;
    Math::Vector4f      basePositionAndSize;
    Video::ColorRGBAf   baseColor;
}
PACK_STRUCT;

struct EntryConstBuffer
{
    static const size_t maxNumImages = 1024;//2048;

    struct ImageEntry
    {
        Math::Vector4f      positionSizeLayer;
        Video::ColorRGBAf   color;
    };

    ImageEntry entries[maxNumImages];
}
PACK_STRUCT;


#include "Core/PackPop.h"

} // /namespace ImageArrayDrawingShader

} // /namespace StandardShader

} // /namespace Video

} // /namespace Fork


#endif



// ========================