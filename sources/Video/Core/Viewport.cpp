/*
 * Viewport file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/Core/Viewport.h"
#include "Math/Core/Equal.h"


namespace Fork
{

namespace Video
{


Viewport::Viewport(const Math::Point2i& vpPosition, const Math::Size2i& vpSize) :
    position{ vpPosition },
    size    { vpSize     }
{
}
Viewport::Viewport(const Math::Point2i& vpPosition, const Math::Size2i& vpSize, float vpMinDepth, float vpMaxDepth) :
    position{ vpPosition },
    size    { vpSize     },
    minDepth{ vpMinDepth },
    maxDepth{ vpMaxDepth }
{
}

FORK_EXPORT bool operator == (const Viewport& a, const Viewport& b)
{
    return
        a.position  == b.position   &&
        a.size      == b.size       &&
        a.minDepth  == b.minDepth   &&
        a.maxDepth  == b.maxDepth;
}

FORK_EXPORT bool operator != (const Viewport& a, const Viewport& b)
{
    return !(a == b);
}


} // /namespace Video

} // /namespace Fork



// ========================