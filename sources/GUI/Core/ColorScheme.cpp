/*
 * GUI color scheme file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GUI/Core/ColorScheme.h"


namespace Fork
{

namespace GUI
{


static ColorScheme defaultColorScheme;
static const ColorScheme* activeColorScheme = &defaultColorScheme;

FORK_EXPORT void SetupActiveColorScheme(const ColorScheme* colorScheme)
{
    activeColorScheme = (colorScheme != nullptr ? colorScheme : &defaultColorScheme);
}

FORK_EXPORT const ColorScheme& GetActiveColorScheme()
{
    return *activeColorScheme;
}


} // /namespace GUI

} // /namespace Fork



// ========================