/*
 * Editor wxWidgets input event mapping file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "WxInputEventMapping.h"
#include "IO/InputDevice/Keyboard.h"
#include "IO/InputDevice/Mouse.h"
#include "Platform/Core/KeyCodeMapping.h"

#include <wx/msgdlg.h>//!!!


namespace Fork
{

namespace Editor
{

namespace WxInputEventMapping
{


/* --- Internal functions --- */

static bool ConvertWxKeyCode(int keyCodeWx, IO::KeyCodes& keyCode)
{
    switch (keyCodeWx)
    {
        case WXK_RETURN:    keyCode = IO::KeyCodes::KeyReturn;      return true;
        case WXK_ESCAPE:    keyCode = IO::KeyCodes::KeyEscape;      return true;
        case WXK_SPACE:     keyCode = IO::KeyCodes::KeySpace;       return true;
        case WXK_SHIFT:     keyCode = IO::KeyCodes::KeyShift;       return true;
        case WXK_CONTROL:   keyCode = IO::KeyCodes::KeyControl;     return true;
        case WXK_DELETE:    keyCode = IO::KeyCodes::KeyDelete;      return true;
        case WXK_TAB:       keyCode = IO::KeyCodes::KeyTab;         return true;
        case WXK_BACK:      keyCode = IO::KeyCodes::KeyBack;        return true;
        case WXK_LEFT:      keyCode = IO::KeyCodes::KeyLeft;        return true;
        case WXK_UP:        keyCode = IO::KeyCodes::KeyUp;          return true;
        case WXK_RIGHT:     keyCode = IO::KeyCodes::KeyRight;       return true;
        case WXK_DOWN:      keyCode = IO::KeyCodes::KeyDown;        return true;
        case WXK_END:       keyCode = IO::KeyCodes::KeyEnd;         return true;
        case WXK_HOME:      keyCode = IO::KeyCodes::KeyHome;        return true;
        case WXK_INSERT:    keyCode = IO::KeyCodes::KeyInsert;      return true;
        case WXK_PAGEUP:    keyCode = IO::KeyCodes::KeyPageUp;      return true;
        case WXK_PAGEDOWN:  keyCode = IO::KeyCodes::KeyPageDown;    return true;
        case WXK_F1:        keyCode = IO::KeyCodes::KeyF1;          return true;
        case WXK_F2:        keyCode = IO::KeyCodes::KeyF2;          return true;
        case WXK_F3:        keyCode = IO::KeyCodes::KeyF3;          return true;
        case WXK_F4:        keyCode = IO::KeyCodes::KeyF4;          return true;
        case WXK_F5:        keyCode = IO::KeyCodes::KeyF5;          return true;
        case WXK_NUMPAD0:   keyCode = IO::KeyCodes::KeyNumPad0;     return true;
        case WXK_NUMPAD1:   keyCode = IO::KeyCodes::KeyNumPad1;     return true;
        case WXK_NUMPAD2:   keyCode = IO::KeyCodes::KeyNumPad2;     return true;
        case WXK_NUMPAD3:   keyCode = IO::KeyCodes::KeyNumPad3;     return true;
        case WXK_NUMPAD4:   keyCode = IO::KeyCodes::KeyNumPad4;     return true;
        case WXK_NUMPAD5:   keyCode = IO::KeyCodes::KeyNumPad5;     return true;
        case WXK_NUMPAD6:   keyCode = IO::KeyCodes::KeyNumPad6;     return true;
        case WXK_NUMPAD7:   keyCode = IO::KeyCodes::KeyNumPad7;     return true;
        case WXK_NUMPAD8:   keyCode = IO::KeyCodes::KeyNumPad8;     return true;
        case WXK_NUMPAD9:   keyCode = IO::KeyCodes::KeyNumPad9;     return true;

        default:
            if (keyCodeWx >= 0x30 && keyCodeWx < 0x5a)
            {
                keyCode = Platform::KeyCodeMapping::MapKeyCode(static_cast<unsigned char>(keyCodeWx));
                return true;
            }
            break;
    }

    return false;
}

static bool ConvertWxMouseButton(int buttonWx, IO::MouseKeyCodes& mouseKeyCode)
{
    switch (buttonWx)
    {
        case wxMOUSE_BTN_LEFT:      mouseKeyCode = IO::MouseKeyCodes::MouseLeft;        return true;
        case wxMOUSE_BTN_RIGHT:     mouseKeyCode = IO::MouseKeyCodes::MouseRight;       return true;
        case wxMOUSE_BTN_MIDDLE:    mouseKeyCode = IO::MouseKeyCodes::MouseMiddle;      return true;
        case wxMOUSE_BTN_AUX1:      mouseKeyCode = IO::MouseKeyCodes::MouseXButton1;    return true;
        case wxMOUSE_BTN_AUX2:      mouseKeyCode = IO::MouseKeyCodes::MouseXButton2;    return true;
    }
    return false;
}


/* --- Global functions --- */

void OnKeyDown(wxKeyEvent& event, const Platform::Frame* frame)
{
    int keyCodeWx = event.GetKeyCode();

    IO::KeyCodes keyCode = IO::KeyCodes::KeyPause;
    if (ConvertWxKeyCode(keyCodeWx, keyCode))
        IO::Keyboard::Instance()->PostEventKeyDown(keyCode, frame);

    event.Skip();
}

void OnKeyUp(wxKeyEvent& event, const Platform::Frame* frame)
{
    int keyCodeWx = event.GetKeyCode();

    IO::KeyCodes keyCode = IO::KeyCodes::KeyPause;
    if (ConvertWxKeyCode(keyCodeWx, keyCode))
        IO::Keyboard::Instance()->PostEventKeyUp(keyCode, frame);

    event.Skip();
}

void OnChar(wxKeyEvent& event, const Platform::Frame* frame)
{
    auto keyCodeASCII = event.GetUnicodeKey();

    /*
    Check if key code is greater than zero because wxWidgets will
    post this events also when a non-char key is pressed
    -> Then the key code is zero.
    */
    if (keyCodeASCII > 0 && keyCodeASCII < 0xff)
        IO::Keyboard::Instance()->PostEventTextEnter(static_cast<wchar_t>(keyCodeASCII), frame);

    event.Skip();
}

void OnMouseDblClick(wxMouseEvent& event, const Platform::Frame* frame)
{
    int buttonWx = event.GetButton();

    IO::MouseKeyCodes mouseKeyCode = IO::MouseKeyCodes::MouseXButton2;
    if (ConvertWxMouseButton(buttonWx, mouseKeyCode))
        IO::Mouse::Instance()->PostEventButtonDoubleClicked(mouseKeyCode, frame);

    event.Skip();
}

void OnMouseDown(wxMouseEvent& event, const Platform::Frame* frame)
{
    int buttonWx = event.GetButton();

    IO::MouseKeyCodes mouseKeyCode = IO::MouseKeyCodes::MouseXButton2;
    if (ConvertWxMouseButton(buttonWx, mouseKeyCode))
        IO::Mouse::Instance()->PostEventButtonDown(mouseKeyCode, frame);

    event.Skip();
}

void OnMouseUp(wxMouseEvent& event, const Platform::Frame* frame)
{
    int buttonWx = event.GetButton();

    IO::MouseKeyCodes mouseKeyCode = IO::MouseKeyCodes::MouseXButton2;
    if (ConvertWxMouseButton(buttonWx, mouseKeyCode))
        IO::Mouse::Instance()->PostEventButtonUp(mouseKeyCode, frame);

    event.Skip();
}

void OnMouseWheel(wxMouseEvent& event, const Platform::Frame* frame)
{
    IO::Mouse::Instance()->PostEventWheelMotion(event.GetWheelRotation() / event.GetWheelDelta(), frame);
    event.Skip();
}

/* --- Internal event callback --- */

static void OnWxKeyDown(wxKeyEvent& event)
{
    OnKeyDown(event, nullptr);
}

static void OnWxKeyUp(wxKeyEvent& event)
{
    OnKeyUp(event, nullptr);
}

static void OnWxChar(wxKeyEvent& event)
{
    OnChar(event, nullptr);
}

static void OnWxMouseDblClick(wxMouseEvent& event)
{
    OnMouseDblClick(event, nullptr);
}

static void OnWxMouseDown(wxMouseEvent& event)
{
    OnMouseDown(event, nullptr);
}

static void OnWxMouseUp(wxMouseEvent& event)
{
    OnMouseUp(event, nullptr);
}

static void OnWxMouseWheel(wxMouseEvent& event)
{
    OnMouseWheel(event, nullptr);
}

/* --- Event handler binding function --- */

void BindEventHandler(
    wxEvtHandler* eventHandler,
    bool bindKeyDown, bool bindKeyUp, bool bindChar,
    bool bindMouseDlbClick, bool bindMouseDown, bool bindMouseUp, bool bindMouseWheel)
{
    if (bindKeyDown)
        eventHandler->Bind( wxEVT_KEY_DOWN,      OnWxKeyDown       );
    if (bindKeyUp)
        eventHandler->Bind( wxEVT_KEY_UP,        OnWxKeyUp         );
    
    if (bindChar)
        eventHandler->Bind( wxEVT_CHAR,          OnWxChar          );

    if (bindMouseDlbClick)
    {
        eventHandler->Bind( wxEVT_LEFT_DCLICK,   OnWxMouseDblClick );
        eventHandler->Bind( wxEVT_MIDDLE_DCLICK, OnWxMouseDblClick );
        eventHandler->Bind( wxEVT_RIGHT_DCLICK,  OnWxMouseDblClick );
        eventHandler->Bind( wxEVT_AUX1_DCLICK,   OnWxMouseDblClick );
        eventHandler->Bind( wxEVT_AUX2_DCLICK,   OnWxMouseDblClick );
    }

    if (bindMouseDown)
    {
        eventHandler->Bind( wxEVT_LEFT_DOWN,     OnWxMouseDown     );
        eventHandler->Bind( wxEVT_MIDDLE_DOWN,   OnWxMouseDown     );
        eventHandler->Bind( wxEVT_RIGHT_DOWN,    OnWxMouseDown     );
        eventHandler->Bind( wxEVT_AUX1_DOWN,     OnWxMouseDown     );
        eventHandler->Bind( wxEVT_AUX2_DOWN,     OnWxMouseDown     );
    }

    if (bindMouseUp)
    {
        eventHandler->Bind( wxEVT_LEFT_UP,       OnWxMouseUp       );
        eventHandler->Bind( wxEVT_MIDDLE_UP,     OnWxMouseUp       );
        eventHandler->Bind( wxEVT_RIGHT_UP,      OnWxMouseUp       );
        eventHandler->Bind( wxEVT_AUX1_UP,       OnWxMouseUp       );
        eventHandler->Bind( wxEVT_AUX2_UP,       OnWxMouseUp       );
    }

    if (bindMouseWheel)
        eventHandler->Bind( wxEVT_MOUSEWHEEL,    OnWxMouseWheel    );
}


} // /namespace WxInputEventMapping

} // /namespace Editor

} // /namespace Fork



// ========================