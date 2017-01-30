/*
 * Editor abstract frame file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "AbstractFrame.h"
#include "Core/Exception/NullPointerException.h"
#include "Platform/Core/OS.h"


namespace Fork
{

namespace Editor
{


AbstractFrame::AbstractFrame(wxWindow* windowRef) :
    Platform::Frame({}, nullptr),
    windowRef_(windowRef)
{
    ASSERT_POINTER(windowRef);
}

void AbstractFrame::SetupPosition(const Math::Point2i& position)
{
    windowRef_->SetPosition(wxPoint(position.x, position.y));
}

Math::Point2i AbstractFrame::Position() const
{
    auto position = windowRef_->GetScreenPosition();
    return { position.x, position.y };
}

void AbstractFrame::SetupSize(const Math::Size2i& size)
{
    windowRef_->SetSize(wxSize(size.width, size.height));
}

Math::Size2i AbstractFrame::Size() const
{
    auto size = windowRef_->GetSize();
    return { size.GetWidth(), size.GetHeight() };
}

void AbstractFrame::SetupClientAreaSize(const Math::Size2i& size)
{
    windowRef_->SetClientSize(wxSize(size.width, size.height));
}

Math::Size2i AbstractFrame::ClientAreaSize() const
{
    auto clientSize = windowRef_->GetClientSize();
    return { clientSize.GetWidth(), clientSize.GetHeight() };
}

void AbstractFrame::SetupTitle(const std::wstring& title)
{
    windowRef_->SetLabel(title);
}

std::wstring AbstractFrame::Title() const
{
    return windowRef_->GetLabel().ToStdWstring();
}
        
void AbstractFrame::Show(bool show)
{
    windowRef_->Show(show);
}

bool AbstractFrame::IsShown() const
{
    return windowRef_->IsShown();
}

void AbstractFrame::Focus()
{
    windowRef_->SetFocus();
}

bool AbstractFrame::HasFocus() const
{
    return windowRef_->HasFocus();
}

Math::Size2i AbstractFrame::BorderSize() const
{
    auto borderSize = windowRef_->GetWindowBorderSize();
    return { borderSize.GetWidth(), borderSize.GetHeight() };
}

Math::Vector2i AbstractFrame::ClientAreaOffset() const
{
    auto clientOrigin = windowRef_->GetClientAreaOrigin();
    return { clientOrigin.x, clientOrigin.y };
}
        
void AbstractFrame::ChangeDesc(
    const Platform::FrameDescription& frameDesc, const Math::Point2i& position, const Math::Size2i& clientAreaSize)
{
    // dummy
}

bool AbstractFrame::ReceiveEvents()
{
    return true; // dummy
}

bool AbstractFrame::IsOpen() const
{
    return windowRef_->IsShown();
}

const void* AbstractFrame::GetNativeHandle() const
{
    return nullptr; // dummy
}

const void* AbstractFrame::GetNativeDeviceContext() const
{
    return nullptr; // dummy
}


} // /namespace Editor

} // /namespace Fork



// ========================