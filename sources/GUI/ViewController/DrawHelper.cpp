/*
 * GUI draw helper file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "DrawHelper.h"
#include "GUI/Core/DrawContext.h"


namespace Fork
{

namespace GUI
{

namespace DrawHelper
{


void Draw3DFrame(DrawContext* context, Rect rect, const ColorScheme::Border& borderColors)
{
    context->DrawFrame(rect, borderColors.outer);
    rect.ResizeLinear(-1);
    context->DrawFrame(rect, borderColors.innerTop, borderColors.innerTop, borderColors.innerBottom, borderColors.innerBottom);
}


} // /namespace DrawHelper

} // /namespace GUI

} // /namespace Fork



// ========================