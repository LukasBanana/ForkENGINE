/*
 * Render system/context access header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_RENDER_SYS_CTX_H__
#define __FORK_RENDER_SYS_CTX_H__


#include "Core/StaticConfig.h"
#include "Video/RenderSystem/RenderSystemException.h"
#include "Video/RenderSystem/RenderContextException.h"


static inline Fork::Video::RenderSystem* RenderSys()
{
    #ifdef FORK_DEBUG
    auto renderSystem = Fork::Video::RenderSystem::Active();
    if (!renderSystem)
        throw Fork::RenderSystemException("Render system required but none is active");
    return renderSystem;
    #else
    return Fork::Video::RenderSystem::Active();
    #endif
}

static inline Fork::Video::RenderContext* RenderCtx()
{
    #ifdef FORK_DEBUG
    auto renderContext = Fork::Video::RenderContext::Active();
    if (!renderContext)
        throw Fork::RenderContextException("Render context required but none is active");
    return renderContext;
    #else
    return Fork::Video::RenderContext::Active();
    #endif
}


#endif



// ========================