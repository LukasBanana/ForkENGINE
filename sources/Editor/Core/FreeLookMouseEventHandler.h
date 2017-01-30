/*
 * Editor free-look mouse event handler header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_FREE_LOOK_MOUSE_EVENT_HANDLER_H__
#define __FORK_EDITOR_FREE_LOOK_MOUSE_EVENT_HANDLER_H__


#include "IO/InputDevice/Mouse.h"


namespace Fork
{

namespace Editor
{


class FreeLookMouseEventHandler : public IO::Mouse::EventHandler
{
    
    protected:
        
        FreeLookMouseEventHandler() = default;

        void LockFreeLook();
        void UnlockFreeLook();

        inline bool IsFreeLook() const
        {
            return isFreeLook_;
        }

    private:
        
        bool            isFreeLook_     = false;
        Math::Point2i   prevMousePos_;
        
};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================