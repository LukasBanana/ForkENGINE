/*
 * Key codes header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_KEY_CODES_H__
#define __FORK_IO_KEY_CODES_H__


namespace Fork
{

namespace IO
{


/**
Keyboard key codes.
\note Some of them are only supported under MS/Windows (like "KeyXButton1").
*/
enum class KeyCodes
{
    KeyLButton,             //!< Left mouse button.
    KeyRButton,             //!< Right mouse button.
    KeyCancel,              //!< Control-break processing.
    KeyMButton,             //!< Middle mouse button (three-button mouse).
    KeyXButton1,            //!< Windows 2000/XP: X1 mouse button.
    KeyXButton2,            //!< Windows 2000/XP: X2 mouse button.

    KeyBack,                //!< BACKSPACE key.
    KeyTab,                 //!< TAB key.
    KeyClear,               //!< CLEAR key.
    KeyReturn,              //!< RETURN (or ENTER) key.
    KeyShift,               //!< SHIFT key.
    KeyControl,             //!< CTRL key.

    KeyMenu,                //!< ALT key.
    KeyPause,               //!< PAUSE key.
    KeyCapital,             //!< CAPS LOCK key.

    KeyEscape,
    KeySpace,
    KeyPageUp,
    KeyPageDown,
    KeyEnd,                 //!< END key.
    KeyHome,                //!< HOME (or POS1) key.
    
    KeyLeft,                //!< Left arrow key.
    KeyUp,                  //!< Up arrow key.
    KeyRight,               //!< Right arrow key.
    KeyDown,                //!< Down arrow key.

    KeySelect,
    KeyPrint,               //!< Print key.
    KeyExe,                 //!< Execute key.
    KeySnapshot,
    KeyInsert,
    KeyDelete,
    KeyHelp,

    Key0,
    Key1,
    Key2,
    Key3,
    Key4,
    Key5,
    Key6,
    Key7,
    Key8,
    Key9,

    KeyA,
    KeyB,
    KeyC,
    KeyD,
    KeyE,
    KeyF,
    KeyG,
    KeyH,
    KeyI,
    KeyJ,
    KeyK,
    KeyL,
    KeyM,
    KeyN,
    KeyO,
    KeyP,
    KeyQ,
    KeyR,
    KeyS,
    KeyT,
    KeyU,
    KeyV,
    KeyW,
    KeyX,
    KeyY,
    KeyZ,

    KeyWinLeft,
    KeyWinRight,
    KeyApps,
    KeySleep,

    KeyNumPad0,
    KeyNumPad1,
    KeyNumPad2,
    KeyNumPad3,
    KeyNumPad4,
    KeyNumPad5,
    KeyNumPad6,
    KeyNumPad7,
    KeyNumPad8,
    KeyNumPad9,

    KeyMultiply,
    KeyAdd,
    KeySeparator,
    KeySubtract,
    KeyDecimal,
    KeyDivide,

    KeyF1,
    KeyF2,
    KeyF3,
    KeyF4,
    KeyF5,
    KeyF6,
    KeyF7,
    KeyF8,
    KeyF9,
    KeyF10,
    KeyF11,
    KeyF12,
    KeyF13,
    KeyF14,
    KeyF15,
    KeyF16,
    KeyF17,
    KeyF18,
    KeyF19,
    KeyF20,
    KeyF21,
    KeyF22,
    KeyF23,
    KeyF24,

    KeyNumLock,
    KeyScroll,

    KeyLShift,
    KeyRShift,
    KeyLControl,
    KeyRControl,
    KeyLMenu,
    KeyRMenu,

    KeyBrowserBack,
    KeyBrowserForward,
    KeyBrowserRefresh,
    KeyBrowserStop,
    KeyBrowserSearch,
    KeyBrowserFavorits,
    KeyBrowserHome,

    KeyVolumeMute,
    KeyVolumeDown,
    KeyVolumeUp,

    KeyMediaNextTrack,
    KeyMediaPrevTrack,
    KeyMediaStop,
    KeyMediaPlayPause,

    KeyLaunchMail,
    KeyLaunchMediaSelect,
    KeyLaunchApp1,
    KeyLaunchApp2,

    KeyPlus,                //!< '+'
    KeyComma,               //!< ','
    KeyMinus,               //!< '-'
    KeyPeriod,              //!< '.'

    KeyExponent,            //!< '^'

    KeyAttn,
    KeyCrSel,
    KeyExSel,
    KeyErEOF,
    KeyPlay,
    KeyZoom,
    KeyNoName,
    KeyPA1,
    KeyOEMClear,
};


} // /namespace IO

} // /namespace Fork


#endif



// ========================