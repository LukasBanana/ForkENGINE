/*
 * Engine format base structures header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_ENGINE_FORMAT_BASE_STRUCTS_H__
#define __FORK_ENGINE_FORMAT_BASE_STRUCTS_H__


#include "BaseTypes.h"


namespace Fork
{

namespace Engine
{

namespace Format
{


#include "Core/PackPush.h"

//! Common chunk description structure.
struct ChunkDesc
{
    UInt32 id       = 0; //!< Chunk ID number.
    UInt32 offset   = 0; //!< Chunk offset (in bytes) from the beginning of the file.
    UInt32 size     = 0; //!< Chunk size (in bytes).
}
PACK_STRUCT;

#include "Core/PackPop.h"


} // /namespace Format

} // /namespace Engine

} // /namespace Fork


#endif



// ========================