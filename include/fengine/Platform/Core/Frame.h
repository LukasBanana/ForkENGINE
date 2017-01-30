/*
 * Frame header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PLATFORM_FRAME_H__
#define __FORK_PLATFORM_FRAME_H__


#include "Math/Core/Vector2.h"
#include "Math/Core/Size2.h"
#include "Core/Export.h"

#include <memory>
#include <string>
#include <functional>


namespace Fork
{

namespace Platform
{


/**
Frame (or rather the graphics window) description structure.
\todo Rename to "Description" and move into "Frame" class.
*/
struct FORK_EXPORT FrameDescription
{
    FrameDescription(
        bool frVisible = true, bool frBorderless = false, bool frResizable = false,
        bool frAcceptDropFiles = false, bool frPreventForPowerSafe = true) :
            isVisible           { frVisible             },
            isBorderless        { frBorderless          },
            isResizable         { frResizable           },
            acceptDropFiles     { frAcceptDropFiles     },
            preventForPowerSafe { frPreventForPowerSafe }
    {
    }

    bool operator == (const FrameDescription& other) const
    {
        return
            isVisible           == other.isVisible          &&
            isBorderless        == other.isBorderless       &&
            isResizable         == other.isResizable        &&
            acceptDropFiles     == other.acceptDropFiles    &&
            preventForPowerSafe == other.preventForPowerSafe;
    }
    bool operator != (const FrameDescription& other) const
    {
        return !(*this == other);
    }

    bool isVisible              = true;     //!< Specifies whether the frame is visible or hidden at startup. By default visible.
    bool isBorderless           = false;    //!< Specifies whether the frame is borderless or not. By default false.
    bool isResizable            = false;    //!< Specifies whether the frame is resizable or fixed sized. By default fixed. This can not be used together with "borderless"!
    bool acceptDropFiles        = false;    //!< Specifies whether the frame accepts droped files or not. By default disabled. This can not be used together with "borderless"!
    bool preventForPowerSafe    = true;     //!< Specifies whether the frame shall prevent the system for power safe mode. By default true.
};


class Frame;

typedef std::shared_ptr<Frame> FramePtr;

//! This is the frame (or rather window) interface.
class FORK_EXPORT Frame
{
    
    public:
        
        /* === Classes === */

        //! Frame event handler interface.
        class FORK_EXPORT EventHandler
        {
            public:
                
                virtual ~EventHandler()
                {
                }

                /**
                Receives the 'file drop' event.
                \param[in] filename Specifies the filename (in UNICODE) of the drop file.
                \param[in] index Specifies the index of the file list.
                \param[in] numFiles Specifies the total number of files.
                */
                virtual void OnFileDrop(const std::wstring& filename, unsigned int index, unsigned int numFiles)
                {
                }

                /**
                Receives the 'resize' event.
                \param[in] clientAreaSize Specifies the new client area size.
                This can be used to update the screen resolution.
                \note Inside this function, the frame description can not be changed (using "ChangeDesc")!
                \see ChangeDesc
                */
                virtual void OnResize(const Math::Size2i& clientAreaSize)
                {
                }

            protected:
                
                EventHandler()
                {
                }

        };

        typedef std::shared_ptr<EventHandler> EventHandlerPtr;

        /* === Functions === */

        virtual ~Frame();

        /**
        Creates a new frame.
        \throws FrameCreationException if the frame creation failed.
        */
        static FramePtr Create(
            const FrameDescription& frameDesc,
            const std::wstring& title, const Math::Size2i& clientAreaSize,
            const Math::Point2i& position, void* parentWindow = nullptr
        );

        /**
        Posts a drop file event.
        \see SetEventHandler
        */
        void PostEventDropFile(const std::wstring& filename, unsigned int index, unsigned int numFiles);

        /**
        Posts a resize event.
        \see SetEventHandler
        */
        void PostEventResize(const Math::Size2i& clientAreaSize);

        //! Sets the frame position relative to its parent.
        virtual void SetupPosition(const Math::Point2i& position) = 0;
        //! Returns the frame position relative to the screen.
        virtual Math::Point2i Position() const = 0;

        /**
        Sets the frame size (inclusive border and title caption).
        \see SetClientSize
        */
        virtual void SetupSize(const Math::Size2i& size) = 0;
        virtual Math::Size2i Size() const = 0;

        /**
        Sets the frame size so that the specified size is equal to the client area size.
        \remarks The client area is the area where something can be drawn inside the frame.
        In a borderless frame the client area size is equal to the frame size.
        \see SetSize
        */
        virtual void SetupClientAreaSize(const Math::Size2i& size) = 0;
        /**
        Returns the client area size.
        \see SetupClientAreaSize
        */
        virtual Math::Size2i ClientAreaSize() const = 0;

        virtual void SetupTitle(const std::wstring& title) = 0;
        virtual std::wstring Title() const = 0;
        
        virtual void Show(bool show = true) = 0;
        virtual bool IsShown() const = 0;

        //! Set the focus to this frame and bring it to the top.
        virtual void Focus() = 0;
        //! Returns true if this frame has the system focus and is on the top.
        virtual bool HasFocus() const = 0;

        //! Returns the frame border size (inclusice title caption). This is (0, 0) if the frame is borderless.
        virtual Math::Size2i BorderSize() const = 0;
        //! Returns the frame client area offset position. This is (0, 0) if the frame is borderless.
        virtual Math::Vector2i ClientAreaOffset() const = 0;

        /**
        Changes the specified frame description. Since the frame appearance will change,
        this is not a simple 'setter' function.
        \param[in] frameDesc Specifies the new frame description.
        \param[in] position Specifies the new frame position.
        \param[in] clientAreaSize Specifies the new frame client area size.
        \see ChangeDesc(const FrameDescription&)
        */
        virtual void ChangeDesc(
            const FrameDescription& frameDesc,
            const Math::Point2i& position,
            const Math::Size2i& clientAreaSize
        ) = 0;

        //! Only changes the frame description without relocating the frame.
        void ChangeDesc(const FrameDescription& frameDesc);

        //! Receives all events affecting this frame and returns false if the frame has been closed.
        virtual bool ReceiveEvents() = 0;

        //! Returns true if the frame is open. Otherwise the frame has been closed.
        virtual bool IsOpen() const = 0;

        /**
        Returns the native frame (or rather window) handle. On MS/Windows this is
        from the type 'const HWND*' and on GNU/Linux this is from the type 'const Window*'.
        */
        virtual const void* GetNativeHandle() const = 0;

        /**
        Returns the native device context handle. On MS/Windows this is from the type
        'const HDC*' and on GNU/Linux this is a null pointer, since there is no device context.
        */
        virtual const void* GetNativeDeviceContext() const = 0;

        //! Returns the frame description.
        inline const FrameDescription& GetDesc() const
        {
            return frameDesc_;
        }
        
        /**
        Sets the event handler for this frame.
        \see EventHandler
        \todo Maybe also change this to "AddEventHandler" to allow several event handlers.
        */
        inline void SetEventHandler(const EventHandlerPtr& eventHandler)
        {
            eventHandler_ = eventHandler;
        }

        /**
        Returns exactly the same raw-pointer of the parent window as passed to the "Frame::Create" function
        \see Frame::Create
        */
        inline void* GetParentWindow() const
        {
            return parentWindow_;
        }

    protected:

        Frame(const FrameDescription& frameDesc, void* parentWindow);

        static bool IsFrameResizeProcessing();

        /* === Members === */

        FrameDescription frameDesc_;

        EventHandlerPtr eventHandler_;

        void* parentWindow_ = nullptr;

    private:
        
        static bool isFrameResizeProcessing_;

};


} // /namespace Platform

} // /namespace Fork


#endif



// ========================