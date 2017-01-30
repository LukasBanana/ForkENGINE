/*
 * WIN32: Win32 frame class file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Win32FrameClass.h"
#include "Core/StringModifier.h"
#include "IO/Core/Log.h"
#include "Core/STLHelper.h"

#include <Windows.h>
#include <string>


namespace Fork
{

namespace Platform
{


LRESULT CALLBACK Win32FrameCallback(HWND Window, UINT Message, WPARAM wParam, LPARAM lParam);

Win32FrameClass::Win32FrameClass()
{
    /* Setup window class information */
    WNDCLASS wc;
    InitRawMemory(wc);

    wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
    wc.hInstance        = GetModuleHandle(nullptr);
    wc.lpfnWndProc      = reinterpret_cast<WNDPROC>(Win32FrameCallback);
    wc.hIcon            = LoadIcon(0, IDI_APPLICATION);
    wc.hCursor          = LoadCursor(0, IDC_ARROW);
    wc.hbrBackground    = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.lpszMenuName     = nullptr;
    wc.lpszClassName    = GetName();

    /* Register window class */
    if (!RegisterClass(&wc))
        throw std::string("Registering frame class \"" + ToStr(GetName()) + "\" failed");
}
Win32FrameClass::Win32FrameClass(const Win32FrameClass&)
{
    /* Dummy */
}
Win32FrameClass::~Win32FrameClass()
{
    /* Unregister window class */
    if (!UnregisterClass(GetName(), GetModuleHandle(nullptr)))
        IO::Log::Error("Unregistering frame class \"" + ToStr(GetName()) + "\" failed");
}

Win32FrameClass* Win32FrameClass::Instance()
{
    static Win32FrameClass frameClass;
    return &frameClass;
}

const wchar_t* Win32FrameClass::GetName() const
{
    return L"__ForkENGINE__Win32_FrameClass__";
}


} // /namespace Platform

} // /namespace Fork



// ========================