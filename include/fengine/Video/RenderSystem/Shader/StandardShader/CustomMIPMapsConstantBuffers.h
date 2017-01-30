/*
 * Custom MIP-maps shader constant buffers header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_CUSTOM_MIP_MAPS_SHADER_CONSTANT_BUFFERS_H__
#define __FORK_CUSTOM_MIP_MAPS_SHADER_CONSTANT_BUFFERS_H__


namespace Fork
{

namespace Video
{

namespace StandardShader
{

//! Namespace for all "CustomMIPMaps" shader constant buffer structures.
namespace CustomMIPMapsShader
{

#include "Core/PackPush.h"


struct ConfigConstBuffer
{
    int layer = 0;
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