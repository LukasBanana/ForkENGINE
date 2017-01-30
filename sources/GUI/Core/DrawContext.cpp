/*
 * GUI draw context file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GUI/Core/DrawContext.h"


namespace Fork
{

namespace GUI
{


void DrawContext::PushScissor(Rect rect)
{
    /* Clamp new scissor rectangle, so it can not be larger than the previous one */
    if (!scissorStack_.empty())
        rect.ClampMin(scissorStack_.top());

    /* Push new scissor onto the stack */
    scissorStack_.push(rect);
    Scissor(rect);
}

void DrawContext::PopScissor()
{
    if (!scissorStack_.empty())
    {
        /* Pop previous scissor from the stack */
        scissorStack_.pop();
        if (scissorStack_.empty())
            Scissor();
        else
            Scissor(scissorStack_.top());
    }
}


} // /namespace GUI

} // /namespace Fork



// ========================