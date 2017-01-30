/*
 * WIN32: Win32 frame file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Win32Frame.h"
#include "Win32FrameClass.h"
#include "Platform/Core/FrameCreationException.h"
#include "IO/Core/Log.h"


namespace Fork
{

namespace Platform
{


Win32Frame::Win32Frame(
    const FrameDescription& frameDesc,
    const std::wstring& title, const Math::Size2i& clientAreaSize,
    const Math::Point2i& position, void* parentWindow) :
    Frame           { frameDesc, parentWindow                                          },
    windowHandle_   { CreateFrameWindow(title, clientAreaSize, position, parentWindow) },
    deviceContext_  { GetDC(windowHandle_)                                             }
{
    SetupUserData(this);
    SetupClientAreaSize(clientAreaSize);
}
Win32Frame::~Win32Frame()
{
    /*
    Release device context and destroy window
    (if this is not a parent's window handle)
    */
    if (windowHandle_ && !parentWindow_)
    {
        if (deviceContext_)
        {
            if (!ReleaseDC(windowHandle_, deviceContext_))
                IO::Log::Error("Releasing Win32 device context failed");
        }
        if (!DestroyWindow(windowHandle_))
            IO::Log::Error("Destroying Win32 frame failed");
    }
}

void Win32Frame::SetupPosition(const Math::Point2i& position)
{
    if (windowHandle_)
        SetWindowPos(windowHandle_, HWND_TOP, position.x, position.y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

Math::Point2i Win32Frame::Position() const
{
    if (windowHandle_)
    {
        RECT rc;
        GetWindowRect(windowHandle_, &rc);
        return { rc.left, rc.top };
    }
    return { 0, 0 };
}

void Win32Frame::SetupSize(const Math::Size2i& size)
{
    if (windowHandle_ && frameDesc_.isResizable)
        SetWindowPos(windowHandle_, HWND_TOP, 0, 0, size.width, size.height, SWP_NOMOVE | SWP_NOZORDER);
}

Math::Size2i Win32Frame::Size() const
{
    if (windowHandle_)
    {
        RECT rc;
        GetWindowRect(windowHandle_, &rc);

        return Math::Size2i(
            rc.right - rc.left,
            rc.bottom - rc.top
        );
    }
    return Math::Size2i();
}

// Queries window rectangular area by the specified client area size and window style.
static RECT QueryWindowArea(LONG width, LONG height, DWORD style)
{
    RECT rc;
    {
        rc.left     = 0;
        rc.top      = 0;
        rc.right    = width;
        rc.bottom   = height;
    }
    AdjustWindowRect(&rc, style, FALSE);
    return rc;
}

void Win32Frame::SetupClientAreaSize(const Math::Size2i& size)
{
    if (windowHandle_)
    {
        /* Get window size by the specified client area size (and frame style flags) */
        auto rc = QueryWindowArea(size.width, size.height, FrameStyleFlags());

        /* Set final window size */
        SetWindowPos(
            windowHandle_, HWND_TOP,
            0, 0,
            rc.right - rc.left,
            rc.bottom - rc.top,
            SWP_NOMOVE | SWP_NOZORDER
        );
    }
}

Math::Size2i Win32Frame::ClientAreaSize() const
{
    if (windowHandle_)
    {
        /*
        Only return the right and bottom corner, since the left and top values are alwas 0.
        see http://msdn.microsoft.com/de-de/library/windows/desktop/ms633503%28v=vs.85%29.aspx
        */
        RECT rc;
        GetClientRect(windowHandle_, &rc);
        return { rc.right, rc.bottom };
    }
    return Math::Size2i();
}

void Win32Frame::SetupTitle(const std::wstring& title)
{
    if (windowHandle_)
        SetWindowText(windowHandle_, title.c_str());
}

std::wstring Win32Frame::Title() const
{
    if (windowHandle_)
    {
        wchar_t title[MAX_PATH];
        GetWindowText(windowHandle_, title, MAX_PATH);
        return std::wstring(title);
    }
    return L"";

}

void Win32Frame::Show(bool show)
{
    if (windowHandle_)
        ShowWindow(windowHandle_, show ? SW_NORMAL : SW_HIDE);
}

bool Win32Frame::IsShown() const
{
    return windowHandle_ != nullptr && IsWindowVisible(windowHandle_) ? true : false;
}

void Win32Frame::Focus()
{
    SetForegroundWindow(windowHandle_);
    SetFocus(windowHandle_);
}

bool Win32Frame::HasFocus() const
{
    return GetForegroundWindow() == windowHandle_ && GetFocus() == windowHandle_;
}

Math::Size2i Win32Frame::BorderSize() const
{
    auto rc = QueryWindowArea(0, 0, FrameStyleFlags());
    return { rc.right - rc.left, rc.bottom - rc.top };
}

Math::Vector2i Win32Frame::ClientAreaOffset() const
{
    auto rc = QueryWindowArea(0, 0, FrameStyleFlags());
    return { -rc.left, -rc.top };
}

void Win32Frame::ChangeDesc(
    const FrameDescription& frameDesc, const Math::Point2i& position, const Math::Size2i& clientAreaSize)
{
    if (!windowHandle_ || Frame::IsFrameResizeProcessing())
        return;

    /* Temporarily remove window user data (to avoid recursive resize events) */
    SetupUserData(nullptr);
    
    /* Update window style */
    frameDesc_ = frameDesc;

    ShowWindow(windowHandle_, SW_HIDE);

    SetWindowLongPtr(windowHandle_, GWL_STYLE, FrameStyleFlags());
    SetWindowPos(windowHandle_, 0, 0, 0, 0, 0, SWP_FRAMECHANGED);

    UpdateWindow(windowHandle_);
    
    /* Setup refresh flags */
    UINT refreshFlags = SWP_NOZORDER;
    if (frameDesc.isVisible)
        refreshFlags |= SWP_SHOWWINDOW;

    /* Update new window dimension */
    SetWindowPos(
        windowHandle_,
        0,
        position.x,
        position.y,
        clientAreaSize.width,
        clientAreaSize.height,
        refreshFlags
    );
    
    /* Adjust client size */
    SetupClientAreaSize(clientAreaSize);

    /* Reset window user data */
    SetupUserData(this);
}

bool Win32Frame::ReceiveEvents()
{
    /* Peek all queued messages */
    MSG message;

    while (PeekMessage(&message, nullptr, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

    return IsOpen();
}

bool Win32Frame::IsOpen() const
{
    return windowHandle_ != nullptr;
}

const void* Win32Frame::GetNativeHandle() const
{
    return reinterpret_cast<const void*>(&windowHandle_);
}

const void* Win32Frame::GetNativeDeviceContext() const
{
    return reinterpret_cast<const void*>(&deviceContext_);
}


/*
 * ======= Private: =======
 */

DWORD Win32Frame::FrameStyleFlags() const
{
    DWORD Style = WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
    
    if (frameDesc_.isBorderless)
        Style |= WS_POPUP;
    else
        Style |= WS_SYSMENU | WS_MINIMIZEBOX | WS_CAPTION;

    if (!frameDesc_.isBorderless)
    {
        if (frameDesc_.acceptDropFiles)
            Style |= WM_DROPFILES;
        if (frameDesc_.isResizable)
            Style |= (WS_SIZEBOX | WS_MAXIMIZEBOX);
    }

    return Style;
}

HWND Win32Frame::CreateFrameWindow(
    const std::wstring& title, const Math::Size2i& clientAreaSize, const Math::Point2i& position, void* parentWindow)
{
    /* Get parent window handle */
    if (parentWindow)
        return *reinterpret_cast<HWND*>(parentWindow);

    /* Get window class */
    auto frameClass = Win32FrameClass::Instance();

    /* Setup final frame area */
    auto frameOffset = ClientAreaOffset();
    auto frameBorder = BorderSize();
    
    auto size = clientAreaSize + frameBorder;
    
    /* Create frame window object */
    HWND windowHandle = CreateWindow(
        frameClass->GetName(),
        title.c_str(),
        FrameStyleFlags(),
        position.x - frameOffset.x,
        position.y - frameOffset.y,
        size.width,
        size.height,
        HWND_DESKTOP,
        nullptr,
        GetModuleHandle(nullptr),
        nullptr
    );

    if (!windowHandle)
        throw FrameCreationException(frameDesc_, "Creating Win32 frame failed");

    /* Set additional flags */
    if (frameDesc_.acceptDropFiles)
        DragAcceptFiles(windowHandle, TRUE);

    return windowHandle;
}

void Win32Frame::SetupUserData(void* userData)
{
    SetWindowLongPtr(windowHandle_, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(userData));
}


} // /namespace Platform

} // /namespace Fork



// ========================