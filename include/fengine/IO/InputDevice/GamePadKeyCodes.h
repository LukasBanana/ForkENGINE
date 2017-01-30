/*
 * Game pad key codes header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_GAME_PAD_KEY_CODES_H__
#define __FORK_IO_GAME_PAD_KEY_CODES_H__


namespace Fork
{

namespace IO
{


//! Game pad (XBox 360 controller) key codes. Only supported under MS/Windows.
enum class GamePadKeyCodes
{
    ButtonDPadUp,
    ButtonDPadDown,
    ButtonDPadLeft,
    ButtonDPadRight,
    ButtonStart,
    ButtonBack,
    ButtonLeftJoystick,
    ButtonRightJoystick,
    ButtonLeftShoulder,
    ButtonRightShoulder,
    ButtonA,
    ButtonB,
    ButtonX,
    ButtonY,
};


} // /namespace IO

} // /namespace Fork


#endif



// ========================