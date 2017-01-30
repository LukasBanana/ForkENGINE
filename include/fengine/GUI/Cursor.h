/*
 * GUI cursor header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GUI_CURSOR_H__
#define __FORK_GUI_CURSOR_H__


#include "Core/Export.h"
#include "GUI/Core/BaseGUITypes.h"


namespace Fork
{

namespace GUI
{


//! GUI environment cursor.
struct Cursor
{
    Point position; //!< Cursor position.
};


} // /namespace GUI

} // /namespace Fork


#endif



// ========================