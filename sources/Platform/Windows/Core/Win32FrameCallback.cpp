/*
 * WIN32: Win32 frame callback file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Win32Frame.h"
#include "Platform/Core/KeyCodeMapping.h"
#include "IO/InputDevice/Keyboard.h"
#include "IO/InputDevice/Mouse.h"
#include "Core/StaticConfig.h"

#ifdef FORK_DEBUG
#   include "IO/Core/Log.h"
#endif

#include <Windows.h>
#include <windowsx.h>


#ifndef HID_USAGE_PAGE_GENERIC
#   define HID_USAGE_PAGE_GENERIC   ((USHORT)0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#   define HID_USAGE_GENERIC_MOUSE  ((USHORT)0x02)
#endif


namespace Fork
{

namespace Platform
{


static inline Frame* GetFrameFromUserData(HWND windowHandle)
{
    return reinterpret_cast<Frame*>(GetWindowLongPtr(windowHandle, GWLP_USERDATA));
}

/* --- Keyboard event functions --- */

static void PostKeyEvent(const IO::KeyCodes keyCode, bool isDown, const Frame* frame)
{
    if (isDown)
        IO::Keyboard::Instance()->PostEventKeyDown(keyCode, frame);
    else
        IO::Keyboard::Instance()->PostEventKeyUp(keyCode, frame);
}

static void PostKeyEvent(WPARAM wParam, LPARAM lParam, bool isDown, const Frame* frame)
{
    /* Extract key code */
    unsigned int keyCodeSys = static_cast<unsigned int>(wParam);
    unsigned int keyCodeOEM = static_cast<unsigned int>(lParam & (0xff << 16)) >> 16;
    
    /* Get first key code mapping */
    const IO::KeyCodes keyCode = KeyCodeMapping::MapKeyCode(keyCodeSys);
    
    /* Check for extended keys */
    switch (keyCode)
    {
        case IO::KeyCodes::KeyShift:
        {
            if (keyCodeOEM == 0x36)
                PostKeyEvent(IO::KeyCodes::KeyRShift, isDown, frame);
            else if (keyCodeOEM == 0x2a)
                PostKeyEvent(IO::KeyCodes::KeyLShift, isDown, frame);
        }
        break;
        
        case IO::KeyCodes::KeyControl:
        {
            if ( ( ( static_cast<unsigned int>(lParam) >> 24 ) & 0x00000001 ) != 0 )
                PostKeyEvent(IO::KeyCodes::KeyRControl, isDown, frame);
            else
                PostKeyEvent(IO::KeyCodes::KeyLControl, isDown, frame);
        }
        break;
    }

    /* Post base key event */
    PostKeyEvent(keyCode, isDown, frame);
}


/* --- Mouse event functions --- */

static int mouseCaptureCounter = 0;

static void CaptureMouseButton(const IO::MouseKeyCodes mouseButton, HWND windowHandle, const Frame* frame)
{
    IO::Mouse::Instance()->PostEventButtonDown(mouseButton, frame);

    if (++mouseCaptureCounter == 1)
        SetCapture(windowHandle);
}

static void ReleaseMouseButton(const IO::MouseKeyCodes mouseButton, const Frame* frame)
{
    IO::Mouse::Instance()->PostEventButtonUp(mouseButton, frame);

    if (--mouseCaptureCounter == 0)
        ReleaseCapture();
    
    if (mouseCaptureCounter < 0)
    {
        #ifdef FORK_DEBUG
        IO::Log::Debug("[ReleaseMouseButton] \"MouseCaptureCount\" should never be less than zero");
        #endif
        mouseCaptureCounter = 0;
    }
}


/* --- Main window callback --- */

LRESULT CALLBACK Win32FrameCallback(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        /* --- Common window events --- */

        case WM_CREATE:
        {
            /* Register raw input device to capture high-resolution mouse motion events */
            RAWINPUTDEVICE inputDevice[1];

            inputDevice[0].usUsagePage  = HID_USAGE_PAGE_GENERIC;
            inputDevice[0].usUsage      = HID_USAGE_GENERIC_MOUSE;
            inputDevice[0].dwFlags      = RIDEV_INPUTSINK;
            inputDevice[0].hwndTarget   = windowHandle;

            RegisterRawInputDevices(inputDevice, 1, sizeof(inputDevice[0]));
        }
        break;

        case WM_CLOSE:
        {
            /* Drop window handle in Win32 frame object */
            auto frame = GetFrameFromUserData(windowHandle);

            if (frame)
            {
                auto frameWin32 = dynamic_cast<Win32Frame*>(frame);

                if (frameWin32)
                    frameWin32->windowHandle_ = nullptr;
            }

            /* Destroy window handle by posting a quit message */
            PostQuitMessage(0);
        }
        break;

        case WM_DROPFILES:
        {
            auto frame = GetFrameFromUserData(windowHandle);

            if (frame && frame->GetDesc().acceptDropFiles)
            {
                WCHAR filename[MAX_PATH] = { 0 };
                HDROP dropHandle = reinterpret_cast<HDROP>(wParam);

                /* Query number of droped files */
                UINT numFiles = DragQueryFile(dropHandle, 0xffffffff, 0, 0);

                /* Register filename from each droped file */
                for (UINT i = 0; i < numFiles; ++i)
                {
                    if (DragQueryFile(dropHandle, i, filename, MAX_PATH))
                        frame->PostEventDropFile(std::wstring(filename), i, numFiles);
                }

                DragFinish(dropHandle);
            }
        }
        return 0;

        case WM_SIZE:
        {
            auto frame = GetFrameFromUserData(windowHandle);

            if (frame)
            {
                WORD width = LOWORD(lParam);
                WORD height = HIWORD(lParam);

                frame->PostEventResize({ width, height });
            }
        }
        return 0;

        case WM_SYSCOMMAND:
        {
            switch (wParam & 0xfff0)
            {
                case SC_SCREENSAVE:
                case SC_MONITORPOWER:
                {
                    auto frame = GetFrameFromUserData(windowHandle);

                    if (frame && frame->GetDesc().preventForPowerSafe)
                    {
                        /* Prevent for a powersave mode of monitor or the screensaver */
                        return 0;
                    }
                }
                break;

                /*case SC_KEYMENU:
                {
                    // (F10 was pressed or ALT + any key was pressed)
                    return 0;
                }
                break;*/
            }
        }
        break;

        case WM_KILLFOCUS:
        {
            //clear keyboard and mouse input states ...
            ReleaseCapture();
        }
        return 0;

        /* --- Keyboard events --- */

        case WM_KEYDOWN:
        {
            auto frame = GetFrameFromUserData(windowHandle);
            PostKeyEvent(wParam, lParam, true, frame);
        }
        return 0;

        case WM_KEYUP:
        {
            auto frame = GetFrameFromUserData(windowHandle);
            PostKeyEvent(wParam, lParam, false, frame);
        }
        return 0;

        case WM_CHAR:
        {
            auto frame = GetFrameFromUserData(windowHandle);
            IO::Keyboard::Instance()->PostEventTextEnter(static_cast<wchar_t>(wParam), frame);
        }
        return 0;

        /* --- Left mouse button events --- */

        case WM_LBUTTONDOWN:
        {
            auto frame = GetFrameFromUserData(windowHandle);
            CaptureMouseButton(IO::MouseKeyCodes::MouseLeft, windowHandle, frame);
        }
        return 0;

        case WM_LBUTTONUP:
        {
            auto frame = GetFrameFromUserData(windowHandle);
            ReleaseMouseButton(IO::MouseKeyCodes::MouseLeft, frame);
        }
        return 0;

        case WM_LBUTTONDBLCLK:
        {
            auto frame = GetFrameFromUserData(windowHandle);
            CaptureMouseButton(IO::MouseKeyCodes::MouseLeft, windowHandle, frame);
            IO::Mouse::Instance()->PostEventButtonDoubleClicked(IO::MouseKeyCodes::MouseLeft, frame);
        }
        return 0;

        /* --- Right mouse button events --- */

        case WM_RBUTTONDOWN:
        {
            auto frame = GetFrameFromUserData(windowHandle);
            CaptureMouseButton(IO::MouseKeyCodes::MouseRight, windowHandle, frame);
        }
        return 0;

        case WM_RBUTTONUP:
        {
            auto frame = GetFrameFromUserData(windowHandle);
            ReleaseMouseButton(IO::MouseKeyCodes::MouseRight, frame);
        }
        return 0;

        case WM_RBUTTONDBLCLK:
        {
            auto frame = GetFrameFromUserData(windowHandle);
            CaptureMouseButton(IO::MouseKeyCodes::MouseRight, windowHandle, frame);
            IO::Mouse::Instance()->PostEventButtonDoubleClicked(IO::MouseKeyCodes::MouseRight, frame);
        }
        return 0;

        /* --- Middle mouse button events --- */

        case WM_MBUTTONDOWN:
        {
            auto frame = GetFrameFromUserData(windowHandle);
            CaptureMouseButton(IO::MouseKeyCodes::MouseMiddle, windowHandle, frame);
        }
        return 0;

        case WM_MBUTTONUP:
        {
            auto frame = GetFrameFromUserData(windowHandle);
            ReleaseMouseButton(IO::MouseKeyCodes::MouseMiddle, frame);
        }
        return 0;

        case WM_MBUTTONDBLCLK:
        {
            auto frame = GetFrameFromUserData(windowHandle);
            CaptureMouseButton(IO::MouseKeyCodes::MouseMiddle, windowHandle, frame);
            IO::Mouse::Instance()->PostEventButtonDoubleClicked(IO::MouseKeyCodes::MouseMiddle, frame);
        }
        return 0;

        /* --- X mouse button events --- */

        case WM_XBUTTONDOWN:
        {
            auto frame = GetFrameFromUserData(windowHandle);

            const short xButton = HIWORD(wParam);
            
            if (xButton == XBUTTON1)
                CaptureMouseButton(IO::MouseKeyCodes::MouseXButton1, windowHandle, frame);
            else if (xButton == XBUTTON2)
                CaptureMouseButton(IO::MouseKeyCodes::MouseXButton2, windowHandle, frame);
        }
        break;

        case WM_XBUTTONUP:
        {
            auto frame = GetFrameFromUserData(windowHandle);

            const short xButton = HIWORD(wParam);
            
            if (xButton == XBUTTON1)
                ReleaseMouseButton(IO::MouseKeyCodes::MouseXButton1, frame);
            else if (xButton == XBUTTON2)
                ReleaseMouseButton(IO::MouseKeyCodes::MouseXButton2, frame);
        }
        break;

        case WM_XBUTTONDBLCLK:
        {
            auto frame = GetFrameFromUserData(windowHandle);

            auto mouse = IO::Mouse::Instance();
            const short xButton = HIWORD(wParam);
            
            if (xButton == XBUTTON1)
            {
                CaptureMouseButton(IO::MouseKeyCodes::MouseXButton1, windowHandle, frame);
                IO::Mouse::Instance()->PostEventButtonDoubleClicked(IO::MouseKeyCodes::MouseXButton1);
            }
            else if (xButton == XBUTTON2)
            {
                CaptureMouseButton(IO::MouseKeyCodes::MouseXButton2, windowHandle, frame);
                IO::Mouse::Instance()->PostEventButtonDoubleClicked(IO::MouseKeyCodes::MouseXButton2);
            }
        }
        break;

        /* --- Mouse wheel events --- */

        case WM_MOUSEWHEEL:
        {
            auto frame = GetFrameFromUserData(windowHandle);
            IO::Mouse::Instance()->PostEventWheelMotion(
                GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA, frame
            );
        }
        return 0;

        /* --- High-resolution HID input events --- */

        case WM_INPUT:
        {
            UINT dataSize = 40;
            static BYTE data[40];

            GetRawInputData(
                reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT,
                data, &dataSize, sizeof(RAWINPUTHEADER)
            );

            RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(data);

            if (raw->header.dwType == RIM_TYPEMOUSE)
            {
                /* Post global mouse motion event */
                int x = raw->data.mouse.lLastX;
                int y = raw->data.mouse.lLastY;

                IO::Mouse::Instance()->PostEventGlobalMotion({ x, y });
            }
        }
        return 0;

        case WM_MOUSEMOVE:
        {
            /* Extract mouse position from event parameter */
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);

            /* Post local mouse motion event */
            auto frame = GetFrameFromUserData(windowHandle);
            IO::Mouse::Instance()->PostEventLocalMotion({ x, y }, frame);
        }
        return 0;
    }

    return DefWindowProc(windowHandle, message, wParam, lParam);
}


} // /namespace Platform

} // /namespace Fork



// ========================