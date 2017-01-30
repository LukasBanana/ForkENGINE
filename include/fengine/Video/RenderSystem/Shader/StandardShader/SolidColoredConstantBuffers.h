/*
 * Solid colored shader constant buffers header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SOLID_COLORED_SHADER_CONSTANT_BUFFERS_H__
#define __FORK_SOLID_COLORED_SHADER_CONSTANT_BUFFERS_H__


#include "Math/Core/Matrix4.h"
#include "Video/Core/ColorRGBA.h"


namespace Fork
{

namespace Video
{

namespace StandardShader
{

//! Namespace for all "SolidColored" shader constant buffer structures.
namespace SolidColoredShader
{

#include "Core/PackPush.h"


struct ConstBuffer
{
    Math::Matrix4f      wvpMatrix;
    Video::ColorRGBAf   solidColor;
}
PACK_STRUCT;


#include "Core/PackPop.h"

} // /namespace SimpleColoredShader

} // /namespace StandardShader

} // /namespace Video

} // /namespace Fork


#endif



// ========================