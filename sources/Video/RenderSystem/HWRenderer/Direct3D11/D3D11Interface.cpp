/*
 * D3D11 hardware renderer library interface file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "../Interface.h"
#include "D3D11RenderSystem.h"
#include "D3D11ProfilerRenderSystem.h"


extern "C"
{

/*
Allocate the Direct3D11 render systems and return the raw pointer.
-> This is an exception in memory management of this engine.
   All other memory objects must be created with managed memory (i.e. std::shared_ptr)!
*/

FORK_EXPORT void* AllocRenderSystem()
{
    return new Fork::Video::D3D11RenderSystem();
}

FORK_EXPORT void* AllocProfilerRenderSystem()
{
    return new Fork::Video::D3D11ProfilerRenderSystem();
}

FORK_EXPORT const char* GetRenderSystemName()
{
    return "Direct3D 11";
}

}



// ========================