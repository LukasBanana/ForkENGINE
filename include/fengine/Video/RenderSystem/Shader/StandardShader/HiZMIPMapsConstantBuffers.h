/*
 * Hi-Z MIP-maps shader constant buffers header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_HIZ_MIP_MAPS_SHADER_CONSTANT_BUFFERS_H__
#define __FORK_HIZ_MIP_MAPS_SHADER_CONSTANT_BUFFERS_H__


namespace Fork
{

namespace Video
{

namespace StandardShader
{

//! Namespace for all "HiZMIPMaps" shader constant buffer structures.
namespace HiZMIPMapsShader
{

#include "Core/PackPush.h"


struct ConfigConstBuffer
{
    static const int sampleModeMin      = 0;
    static const int sampleModeMax      = 1;

    int sampleMode = sampleModeMin;
    int __pad0[3];
}
PACK_STRUCT;


#include "Core/PackPop.h"

} // /namespace HiZMIPMapsShader

} // /namespace StandardShader

} // /namespace Video

} // /namespace Fork


#endif



// ========================