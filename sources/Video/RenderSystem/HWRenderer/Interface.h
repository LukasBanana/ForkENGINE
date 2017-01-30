/*
 * Hardware renderer library interface header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_RENDERER_LIB_INTERFACE_H__
#define __FORK_RENDERER_LIB_INTERFACE_H__


#include "Core/Export.h"


extern "C"
{

/**
Returns a raw-pointer to the allocated render system.
This should be wrapped into a smart pointer:
\code
auto renderSystem = std::shared_ptr<RenderSystem>(AllocRenderSystem());
\endcode
*/
FORK_EXPORT void* AllocRenderSystem();

/**
Returns a raw-pointer to the allocated profiler render system.
This should be wrapped into a smart pointer.
\code
auto renderSystem = std::shared_ptr<RenderSystem>(AllocProfilerRenderSystem());
\endcode
*/
FORK_EXPORT void* AllocProfilerRenderSystem();

/**
Returns a null terminated UTF-8 string
which describes the name of the render system.
*/
FORK_EXPORT const char* GetRenderSystemName();

}


#endif



// ========================