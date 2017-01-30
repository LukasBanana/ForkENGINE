/*
 * Font drawing shader constant buffers header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_FONT_DRAWING_SHADER_CONSTANT_BUFFERS_H__
#define __FORK_FONT_DRAWING_SHADER_CONSTANT_BUFFERS_H__


#include "Math/Core/Matrix4.h"
#include "Video/Core/ColorRGBA.h"


namespace Fork
{

namespace Video
{

namespace StandardShader
{

//! Namespace for all "FontDrawing" shader constant buffer structures.
namespace FontDrawingShader
{

#include "Core/PackPush.h"


struct ConstBuffer
{
    Math::Matrix4f      wvpMatrix;
    Video::ColorRGBAf   color;
}
PACK_STRUCT;


#include "Core/PackPop.h"

} // /namespace FontDrawingShader

} // /namespace StandardShader

} // /namespace Video

} // /namespace Fork


#endif



// ========================