/*
 * Mouse key codes header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_MOUSE_KEY_CODES_H__
#define __FORK_IO_MOUSE_KEY_CODES_H__


namespace Fork
{

namespace IO
{


//! Mouse key codes.
enum class MouseKeyCodes
{
    MouseLeft,      //!< Left mouse button.
    MouseRight,     //!< Right mouse button.
    MouseMiddle,    //!< Mouse wheel button.
    MouseXButton1,  //!< X mouse button 1. Not every mouse has this button! Only supported under MS/Windows.
    MouseXButton2,  //!< X mouse button 2. Not every mouse has this button! Only supported under MS/Windows.
};


} // /namespace IO

} // /namespace Fork


#endif



// ========================