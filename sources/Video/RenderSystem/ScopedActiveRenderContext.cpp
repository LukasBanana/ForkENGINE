/*
 * Scoped active render context file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/RenderSystem/ScopedActiveRenderContext.h"
#include "Video/RenderSystem/RenderContext.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/StaticConfig.h"


namespace Fork
{

namespace Video
{


ScopedActiveRenderContext::ScopedActiveRenderContext(RenderContext* renderContext) :
    prevRenderContext_(RenderContext::Active())
{
    if (renderContext)
        renderContext->Activate();
}
ScopedActiveRenderContext::ScopedActiveRenderContext(ScopedActiveRenderContext&& other) :
    prevRenderContext_(other.prevRenderContext_)
{
    other.prevRenderContext_ = nullptr;
}
ScopedActiveRenderContext::~ScopedActiveRenderContext()
{
    if (prevRenderContext_)
        prevRenderContext_->Activate();
}


} // /namespace Video

} // /namespace Fork



// ========================