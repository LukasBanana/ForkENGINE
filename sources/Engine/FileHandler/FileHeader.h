/*
 * Engine file format header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_ENGINE_FORMAT_HEADER_H__
#define __FORK_ENGINE_FORMAT_HEADER_H__


#include "BaseTypes.h"


namespace Fork
{

namespace Engine
{

namespace Format
{


#include "Core/PackPush.h"

//! Common engine file format header.
struct FileHeader
{
    /**
    Magic number. This is file format specific:
    - ForkGame (*.forkgame) -> "fkgm"
    - ForkScene (*.forkscene) -> "fksc"
    - ForkAssets (*.forkassets) -> "fkas"
    - ForkState (*.forkstate) -> "fkst"
    */
    UInt32 magic                = 0;

    /**
    Format version number.
    100 for version 1.00,
    101 for version 1.01 etc.
    */
    UInt16 version              = 0;

    /**
    Version number of the last compatible format version.
    If one has a scene loader which supports the version 103,
    the member "version" is 105 and the member "compatibleVersion" is 102,
    the scene loader will be able to load this scene file.
    This member is always less then or equal to "version".
    */
    UInt16 compatibleVersion    = 0;

    //! General purpose format flags.
    UInt32 flags                = 0;
}
PACK_STRUCT;

#include "Core/PackPop.h"


} // /namespace Format

} // /namespace Engine

} // /namespace Fork


#endif



// ========================