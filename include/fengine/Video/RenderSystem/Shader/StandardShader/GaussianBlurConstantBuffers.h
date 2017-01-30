/*
 * Gaussian blur shader constant buffers header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GAUSSIAN_BLUR_SHADER_CONSTANT_BUFFERS_H__
#define __FORK_GAUSSIAN_BLUR_SHADER_CONSTANT_BUFFERS_H__


#include "Math/Core/Vector4.h"


namespace Fork
{

namespace Video
{

namespace StandardShader
{

//! Namespace for all "GaussianBlur" shader constant buffer structures.
namespace GaussianBlurShader
{

#include "Core/PackPush.h"


struct ConfigConstBuffer
{
    static const size_t maxNumSamples = 64;

    Math::Vector4f  offsetsAndWeights[maxNumSamples];
    int             numSamples = 8;
    int             __pad0[3];
}
PACK_STRUCT;

struct RenderPassConstBuffer
{
    int renderPass = 0;
    int __pad0[3];
}
PACK_STRUCT;


#include "Core/PackPop.h"

} // /namespace GaussianBlurShader

} // /namespace StandardShader

} // /namespace Video

} // /namespace Fork


#endif



// ========================