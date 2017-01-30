/*
 * GL hardware renderer library interface file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "../Interface.h"
#include "GLRenderSystem.h"
#include "GLProfilerRenderSystem.h"


extern "C"
{

/*
Allocate the OpenGL render systems and return the raw pointer.
-> This is an exception in memory management of this engine.
   All other memory objects must be created with managed memory (i.e. std::shared_ptr)!
*/

FORK_EXPORT void* AllocRenderSystem()
{
    return new Fork::Video::GLRenderSystem();
}

FORK_EXPORT void* AllocProfilerRenderSystem()
{
    return new Fork::Video::GLProfilerRenderSystem();
}

FORK_EXPORT const char* GetRenderSystemName()
{
    return "OpenGL";
}

}



// ========================