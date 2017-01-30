/*
 * WIN32: Frame file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Win32Frame.h"


namespace Fork
{

namespace Platform
{


bool Frame::isFrameResizeProcessing_ = false;

Frame::Frame(const FrameDescription& frameDesc, void* parentWindow) :
    frameDesc_      (frameDesc      ),
    parentWindow_   (parentWindow   )
{
}
Frame::~Frame()
{
}

FramePtr Frame::Create(
    const FrameDescription& frameDesc,
    const std::wstring& title, const Math::Size2i& clientAreaSize,
    const Math::Point2i& position, void* parentWindow)
{
    return std::make_shared<Win32Frame>(frameDesc, title, clientAreaSize, position, parentWindow);
}

void Frame::PostEventDropFile(const std::wstring& filename, unsigned int index, unsigned int numFiles)
{
    if (eventHandler_)
        eventHandler_->OnFileDrop(filename, index, numFiles);
}

void Frame::PostEventResize(const Math::Size2i& clientAreaSize)
{
    if (eventHandler_)
    {
        Frame::isFrameResizeProcessing_ = true;
        eventHandler_->OnResize(clientAreaSize);
        Frame::isFrameResizeProcessing_ = false;
    }
}

void Frame::ChangeDesc(const FrameDescription& frameDesc)
{
    ChangeDesc(frameDesc, Position(), ClientAreaSize());
}

bool Frame::IsFrameResizeProcessing()
{
    return isFrameResizeProcessing_;
}


} // /namespace Platform

} // /namespace Fork



// ========================