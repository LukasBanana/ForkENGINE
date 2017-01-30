/*
 * Simple colored shader constant buffers header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SIMPLE_COLORED_SHADER_CONSTANT_BUFFERS_H__
#define __FORK_SIMPLE_COLORED_SHADER_CONSTANT_BUFFERS_H__


#include "Math/Core/Matrix4.h"
#include "Video/Core/ColorRGBA.h"


namespace Fork
{

namespace Video
{

namespace StandardShader
{

//! Namespace for all "SimpleColored" shader constant buffer structures.
namespace SimpleColoredShader
{

#include "Core/PackPush.h"


struct ConstBuffer
{
    Math::Matrix4f      wvpMatrix;
    Video::ColorRGBAf   baseColor;
}
PACK_STRUCT;


#include "Core/PackPop.h"

} // /namespace SimpleColoredShader

} // /namespace StandardShader

} // /namespace Video

} // /namespace Fork


#endif



// ========================