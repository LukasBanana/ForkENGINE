/*
 * Scissor file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/Core/Scissor.h"


namespace Fork
{

namespace Video
{


Scissor::Scissor(const Math::Point2i& scPosition, const Math::Size2i& scSize) :
    position{ scPosition },
    size    { scSize     }
{
}

FORK_EXPORT bool operator == (const Scissor& a, const Scissor& b)
{
    return a.position == b.position && a.size == b.size;
}

FORK_EXPORT bool operator != (const Scissor& a, const Scissor& b)
{
    return !(a == b);
}


} // /namespace Video

} // /namespace Fork



// ========================