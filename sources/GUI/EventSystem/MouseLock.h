/*
 * GUI mouse lock header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GUI_MOUSE_LOCK_H__
#define __FORK_GUI_MOUSE_LOCK_H__


#include "GUI/Core/BaseGUITypes.h"
#include "GUI/ViewController/ViewController.h"


namespace Fork
{

namespace GUI
{


//! Mouse lock structure.
struct MouseLock
{
    MouseLock();

    void Clamp(const Rect& rect);
    void Unclamp();
    void ClampPoint(Point& point);

    ViewController*                 owner           = nullptr;
    ViewController::EventHandler*   eventHandler    = nullptr;
    Rect                            clampRect;
};

//! Returns a reference to the mouse lock data.
MouseLock& GetMouseLock();


} // /namespace GUI

} // /namespace Fork


#endif



// ========================