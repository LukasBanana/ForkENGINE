/*
 * Editor UI screen frame file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "UIScreenFrame.h"
#include "IO/InputDevice/Mouse.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "../WxInputEventMapping.h"
#include "../Devices.h"
#include "../WxHelper.h"
#include "Core/StaticConfig.h"
#include "IO/Core/Log.h"
#include "Platform/Core/OS.h"

#ifdef FORK_WINDOWS_PLATFORM

#include <Windows.h>

#ifndef HID_USAGE_PAGE_GENERIC
#   define HID_USAGE_PAGE_GENERIC   ((USHORT)0x01)
#endif
#ifndef HID_USAGE_GENERIC_MOUSE
#   define HID_USAGE_GENERIC_MOUSE  ((USHORT)0x02)
#endif

#endif


namespace Fork
{

namespace Editor
{


UIScreenFrame::UIScreenFrame(
    wxWindow* parent, const wxPoint& position, const wxSize& size) :
    wxWindow
    {
        parent, wxID_ANY, position, size,
        /*
        'wxTRANSPARENT_WINDOW' flag must NOT be set!
        Otherwise flickering effects will occur.
        */
        (wxBORDER_NONE | wxCLIP_CHILDREN)
    },
    abstractFrame_{ this }
{
    AdjustWindowStyle();
    SetupInputCallbacks();

    #ifdef FORK_WINDOWS_PLATFORM
    InitRawInputDevices();
    #endif
}
UIScreenFrame::~UIScreenFrame()
{
}

void UIScreenFrame::OnResize(wxSizeEvent& event)
{
    auto clientAreaSize = GetClientSize();

    ResizeScreen(
        static_cast<unsigned int>(clientAreaSize.GetWidth()),
        static_cast<unsigned int>(clientAreaSize.GetHeight())
    );

    if (resizeCallback_)
        resizeCallback_({ clientAreaSize.GetWidth(), clientAreaSize.GetHeight() });
}

void UIScreenFrame::OnMouseDblClick(wxMouseEvent& event)
{
    SetFocus();
    MouseCapture();
    WxInputEventMapping::OnMouseDblClick(event, GetAbstractFrame());
}

void UIScreenFrame::OnMouseDown(wxMouseEvent& event)
{
    SetFocus();
    MouseCapture();
    WxInputEventMapping::OnMouseDown(event, GetAbstractFrame());
}

void UIScreenFrame::OnMouseUp(wxMouseEvent& event)
{
    MouseRelease();
    WxInputEventMapping::OnMouseUp(event, GetAbstractFrame());
}

void UIScreenFrame::OnMotion(wxMouseEvent& event)
{
    auto pos = event.GetPosition();
    IO::Mouse::Instance()->PostEventLocalMotion({ pos.x, pos.y }, GetAbstractFrame());
}

void UIScreenFrame::OnMouseWheel(wxMouseEvent& event)
{
    WxInputEventMapping::OnMouseWheel(event, GetAbstractFrame());
}

void UIScreenFrame::OnMouseCaptureLost(wxMouseCaptureLostEvent& event)
{
    //IO::Mouse::Instance()->PostEventLostCapture();
    mouseCaptureCounter_ = 0;
}

#ifdef FORK_WINDOWS_PLATFORM

/*
This function extends the mouse events from wxWidgets.
This is a partial copy of a "Win32FrameCallback" function in "Platform/Core/Win32FrameCallback.cpp".
*/
WXLRESULT UIScreenFrame::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
    switch (message)
    {
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
    }
    return wxWindow::MSWWindowProc(message, wParam, lParam);
}

#endif

void UIScreenFrame::Present()
{
    renderContext_->Present();
}

void UIScreenFrame::ResizeScreen(unsigned int width, unsigned int height)
{
    if (renderContext_)
        renderContext_->ResizeResolution({ width, height });
}

Video::Viewport UIScreenFrame::FullViewport() const
{
    auto clientAreaSize = GetClientSize();
    return { { 0, 0 }, { clientAreaSize.GetWidth(), clientAreaSize.GetHeight() }, 0.0f, 1.0f };
}


/*
 * ======= Protected: =======
 */

/*
!TODO! -> make this messy code a little less ugly!!!
*/
bool UIScreenFrame::CreateRenderContext(UIScreenFrame*& screenFrame)
{
    if (!screenFrame)
        return false;
    if (screenFrame->renderContext_)
        return true;

    /* Setup frame recreation callback */
    Video::RenderContext::SetFrameReCreationCallback(
        [](void* userData, void* parentWindow)
        {
            UIScreenFrame** screenFrameRef = reinterpret_cast<UIScreenFrame**>(userData);
            if (!screenFrameRef)
                return;

            UIScreenFrame* screenFrame = *screenFrameRef;
            if (!screenFrame)
                return;
            
            auto parent     = screenFrame->GetParent();
            auto position   = screenFrame->GetPosition();
            auto size       = screenFrame->GetSize();

            /* Push user data of abstract frame */
            auto userDataAF = screenFrame->GetAbstractFrame()->userData;

            /* ReCreate frame */
            screenFrame->Destroy();
            *screenFrameRef = MakeWxObject<UIScreenFrame>(parent, position, size);
            screenFrame = *screenFrameRef;

            /* Pop user data of abstract frame */
            screenFrame->GetAbstractFrame()->userData = userDataAF;

            *reinterpret_cast<WXHWND*>(parentWindow) = screenFrame->GetHWND();
        },
        &screenFrame
    );

    /* Create render context */
    screenFrame->renderContext_ = Devices::CreateRenderContext(screenFrame);

    Video::RenderContext::SetFrameReCreationCallback(nullptr, nullptr);
    
    return screenFrame->renderContext_ != nullptr;
}


/*
 * ======= Private: =======
 */

void UIScreenFrame::AdjustWindowStyle()
{
    #ifdef FORK_WINDOWS_PLATFORM

    auto nativeHandle = GetHWND();
    
    auto windowStyle = GetWindowLongPtr(nativeHandle, GWL_STYLE);
    {
        /* Append GL required window styles */
        windowStyle |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
    }
    SetWindowLongPtr(nativeHandle, GWL_STYLE, windowStyle);

    #endif
}

void UIScreenFrame::SetupInputCallbacks()
{
    Bind( wxEVT_SIZE,               &UIScreenFrame::OnResize,           this );
    Bind( wxEVT_MOUSE_CAPTURE_LOST, &UIScreenFrame::OnMouseCaptureLost, this );
    
    /* Default key binding */
    WxInputEventMapping::BindEventHandler(
        this,
        true, true, true,
        false, false, false, false
    );

    /* Extended key binding */
    Bind( wxEVT_LEFT_DCLICK,   &UIScreenFrame::OnMouseDblClick, this );
    Bind( wxEVT_MIDDLE_DCLICK, &UIScreenFrame::OnMouseDblClick, this );
    Bind( wxEVT_RIGHT_DCLICK,  &UIScreenFrame::OnMouseDblClick, this );
    Bind( wxEVT_AUX1_DCLICK,   &UIScreenFrame::OnMouseDblClick, this );
    Bind( wxEVT_AUX2_DCLICK,   &UIScreenFrame::OnMouseDblClick, this );
    
    Bind( wxEVT_LEFT_DOWN,     &UIScreenFrame::OnMouseDown,     this );
    Bind( wxEVT_MIDDLE_DOWN,   &UIScreenFrame::OnMouseDown,     this );
    Bind( wxEVT_RIGHT_DOWN,    &UIScreenFrame::OnMouseDown,     this );
    Bind( wxEVT_AUX1_DOWN,     &UIScreenFrame::OnMouseDown,     this );
    Bind( wxEVT_AUX2_DOWN,     &UIScreenFrame::OnMouseDown,     this );

    Bind( wxEVT_LEFT_UP,       &UIScreenFrame::OnMouseUp,       this );
    Bind( wxEVT_MIDDLE_UP,     &UIScreenFrame::OnMouseUp,       this );
    Bind( wxEVT_RIGHT_UP,      &UIScreenFrame::OnMouseUp,       this );
    Bind( wxEVT_AUX1_UP,       &UIScreenFrame::OnMouseUp,       this );
    Bind( wxEVT_AUX2_UP,       &UIScreenFrame::OnMouseUp,       this );

    Bind( wxEVT_MOTION,        &UIScreenFrame::OnMotion,        this );

    Bind( wxEVT_MOUSEWHEEL,    &UIScreenFrame::OnMouseWheel,    this );
}

void UIScreenFrame::MouseCapture()
{
    if (!mouseCaptureCounter_)
        CaptureMouse();
    ++mouseCaptureCounter_;
}

void UIScreenFrame::MouseRelease()
{
    --mouseCaptureCounter_;
    if (!mouseCaptureCounter_)
        ReleaseMouse();

    #ifdef FORK_DEBUG
    if (mouseCaptureCounter_ < 0)
    {
        /* This may never happen! */
        IO::Log::Debug("[UIScreenFrame] MouseCaptureCounter < 0");
        mouseCaptureCounter_ = 0;
        //FORK_DEBUG_BREAK;
    }
    #endif
}

#ifdef FORK_WINDOWS_PLATFORM

void UIScreenFrame::InitRawInputDevices()
{
    /* Register raw input device to capture high-resolution mouse motion events */
    RAWINPUTDEVICE inputDevice[1];

    inputDevice[0].usUsagePage  = HID_USAGE_PAGE_GENERIC;
    inputDevice[0].usUsage      = HID_USAGE_GENERIC_MOUSE;
    inputDevice[0].dwFlags      = RIDEV_INPUTSINK;
    inputDevice[0].hwndTarget   = reinterpret_cast<HWND>(GetHWND());

    RegisterRawInputDevices(inputDevice, 1, sizeof(inputDevice[0]));
}

#endif


} // /namespace Editor

} // /namespace Fork



// ========================