/*
 * Editor abstract frame header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_ABSTRACT_FRAME_H__
#define __FORK_EDITOR_ABSTRACT_FRAME_H__


#include "Platform/Core/Frame.h"

#include <wx/window.h>


namespace Fork
{

namespace Editor
{


/**
Abstract frame class.
This class is a wrapper around a wxWindow and is used as interface for some engine utils.
*/
class AbstractFrame : public Platform::Frame
{
    
    public:
        
        //! \throws NullPointerException If 'windowRef' is null.
        AbstractFrame(wxWindow* windowRef);

        AbstractFrame(const AbstractFrame&) = delete;
        AbstractFrame& operator = (const AbstractFrame&) = delete;

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
            const Platform::FrameDescription& frameDesc,
            const Math::Point2i& position,
            const Math::Size2i& clientAreaSize
        ) override;

        bool ReceiveEvents() override;

        bool IsOpen() const override;

        const void* GetNativeHandle() const override;
        const void* GetNativeDeviceContext() const override;

        //! User data raw pointer.
        void* userData = nullptr;

    private:
        
        //! Reference to the wxWidgets window.
        wxWindow* windowRef_ = nullptr;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================