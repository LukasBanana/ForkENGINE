/*
 * Scoped active render context header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SCOPED_ACTIVE_RENDER_CONTEXT_H__
#define __FORK_SCOPED_ACTIVE_RENDER_CONTEXT_H__


#include "Core/Export.h"


namespace Fork
{

namespace Video
{


class RenderContext;

/**
Scoped activation for shared render context.
\remarks This can be used to easily activate a render context temporarily for the current scope.
At the end of the respective scope, the previous render context will be activated again.
\remarks This is used in the OpenGL render system when GL objects are created (e.g. "CreateSamplerState"),
to make sure all GL objects are shared with all render contexts.
*/
class FORK_EXPORT ScopedActiveRenderContext
{

    public:

        ScopedActiveRenderContext(RenderContext* renderContext);
        ScopedActiveRenderContext(ScopedActiveRenderContext&& other);
        ~ScopedActiveRenderContext();

        ScopedActiveRenderContext(const ScopedActiveRenderContext&) = delete;
        ScopedActiveRenderContext& operator = (const ScopedActiveRenderContext&) = delete;

    private:

        RenderContext* prevRenderContext_;

};



} // /namespace Video

} // /namespace Fork


#endif



// ========================