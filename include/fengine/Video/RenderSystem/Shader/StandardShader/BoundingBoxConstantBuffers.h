/*
 * Bounding box shader constant buffers header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_BOUNDING_BOX_SHADER_CONSTANT_BUFFERS_H__
#define __FORK_BOUNDING_BOX_SHADER_CONSTANT_BUFFERS_H__


#include "Math/Core/Vector4.h"
#include "Math/Core/Matrix4.h"
#include "Video/Core/ColorRGBA.h"


namespace Fork
{

namespace Video
{

namespace StandardShader
{

//! Namespace for all "BoundingBox" shader constant buffer structures.
namespace BoundingBoxShader
{

#include "Core/PackPush.h"


struct ConstBuffer
{
    Math::Matrix4f      vpMatrix;
    Video::ColorRGBAf   color;
    Math::Vector4i      texSize;
}
PACK_STRUCT;


#include "Core/PackPop.h"

} // /namespace BoundingBox

} // /namespace StandardShader

} // /namespace Video

} // /namespace Fork


#endif



// ========================