/*
 * WIN32: Win32 frame header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PLATFORM_WIN32FRAME_H__
#define __FORK_PLATFORM_WIN32FRAME_H__


#include "Platform/Core/Frame.h"

#include <Windows.h>


namespace Fork
{

namespace Platform
{


class Win32Frame : public Frame
{
    
    public:
        
        Win32Frame(
            const FrameDescription& frameDesc,
            const std::wstring& title, const Math::Size2i& clientAreaSize,
            const Math::Point2i& position, void* parentWindow
        );
        ~Win32Frame();

        Win32Frame(const Win32Frame&) = delete;
        Win32Frame& operator = (const Win32Frame&) = delete;

        void SetupPosition(const Math::Point2i& position) override;
        Math::Point2i Position() const override;

        void SetupSize(const Math::Size2i& size) override;
        Math::Size2i Size() const override;

        void SetupClientAreaSize(const Math::Size2i& size) override;
        Math::Size2i ClientAreaSize() const override;

        void SetupTitle(const std::wstring& title) override;
        std::wstring Title() const override;
        
        void Show(bool show = true) override;
        bool IsShown() const override;

        void Focus() override;
        bool HasFocus() const override;

        Math::Size2i BorderSize() const override;
        Math::Vector2i ClientAreaOffset() const override;
        
        void ChangeDesc(
            const FrameDescription& frameDesc,
            const Math::Point2i& position,
            const Math::Size2i& clientAreaSize
        ) override;

        bool ReceiveEvents() override;

        bool IsOpen() const override;

        const void* GetNativeHandle() const override;
        const void* GetNativeDeviceContext() const override;

    private:
        
        friend LRESULT CALLBACK Win32FrameCallback(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

        DWORD FrameStyleFlags() const;

        HWND CreateFrameWindow(
            const std::wstring& title, const Math::Size2i& clientAreaSize,
            const Math::Point2i& position, void* parentWindow
        );

        void SetupUserData(void* userData);

        /* === Members === */

        HWND windowHandle_;
        HDC deviceContext_;

};


} // /namespace Platform

} // /namespace Fork


#endif



// ========================