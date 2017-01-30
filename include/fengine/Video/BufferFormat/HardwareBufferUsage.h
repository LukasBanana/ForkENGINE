/*
 * Hardware buffer usage header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_HARDWARE_BUFFER_USAGE_H__
#define __FORK_HARDWARE_BUFFER_USAGE_H__


namespace Fork
{

namespace Video
{


/**
Hardware buffer usage enumeration.
\see RenderSystem::WriteBuffer
*/
enum class HardwareBufferUsage
{
    Static,     //!< The hardware buffer will change rarely by the client but often used by the hardware.
    Dynamic,    //!< The hardware buffer will change often by the client.
};

/**
Hardware buffer CPU acccess enumeration.
\see RenderSystem::MapBuffer
*/
enum class HardwareBufferCPUAccess
{
    ReadOnly,   //!< CPU read access only.
    WriteOnly,  //!< CPU write access only.
    ReadWrite,  //!< CPU read and write access.
};


} // /namespace Video

} // /namespace Fork


#endif



// ========================