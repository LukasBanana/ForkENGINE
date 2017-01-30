/*
 * GUI draw helper header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GUI_DRAW_HELPER_H__
#define __FORK_GUI_DRAW_HELPER_H__


#include "GUI/Core/ColorScheme.h"


namespace Fork
{

namespace GUI
{

class DrawContext;

namespace DrawHelper
{


void Draw3DFrame(DrawContext* context, Rect rect, const ColorScheme::Border& borderColors);


} // /namespace DrawHelper

} // /namespace GUI

} // /namespace Fork


#endif



// ========================