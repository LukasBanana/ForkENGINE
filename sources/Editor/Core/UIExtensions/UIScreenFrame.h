/*
 * Editor UI screen frame header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_UI_SCREEN_FRAME_H__
#define __FORK_EDITOR_UI_SCREEN_FRAME_H__


#include "Video/RenderSystem/RenderContext.h"
#include "Video/Core/Viewport.h"
#include "Platform/Core/OS.h"
#include "AbstractFrame.h"

#include <wx/window.h>

#include <functional>


namespace Fork
{

namespace Editor
{


class UIScreenFrame : public wxWindow
{
    
    public:
        
        typedef std::function<void (const Math::Size2i& clientAreaSize)> ResizeCallback;

        UIScreenFrame(
            wxWindow* parent, const wxPoint& position = wxDefaultPosition, const wxSize& size = wxDefaultSize
        );
        virtual ~UIScreenFrame();

        /* === Callback functions === */

        void OnResize       (wxSizeEvent&   event);

        void OnMouseDblClick(wxMouseEvent&  event);
        void OnMouseDown    (wxMouseEvent&  event);
        void OnMouseUp      (wxMouseEvent&  event);
        void OnMotion       (wxMouseEvent&  event);
        void OnMouseWheel   (wxMouseEvent&  event);

        void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);

        #ifdef FORK_WINDOWS_PLATFORM
        WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);
        #endif

        /* === Functions === */

        void Present();

        void ResizeScreen(unsigned int width, unsigned int height);

        //! Returns a viewport which is spaned over the entire render context.
        Video::Viewport FullViewport() const;

        static bool CreateRenderContext(UIScreenFrame*& screenFrame);

        /* === Inline functions === */

        inline Video::RenderContext* GetRenderContext() const
        {
            return renderContext_.get();
        }

        inline void SetResizeCallback(const ResizeCallback& resizeCallback)
        {
            resizeCallback_ = resizeCallback;
        }

        /**
        Returns the abstract frame which is connected to this wxWidgets frame.
        \remarks This can be used as interface between wxWidgets and some engine utils.
        */
        inline const AbstractFrame* GetAbstractFrame() const
        {
            return &abstractFrame_;
        }
        inline AbstractFrame* GetAbstractFrame()
        {
            return &abstractFrame_;
        }

    private:
        
        /* === Functions === */

        void AdjustWindowStyle();

        void SetupInputCallbacks();

        void MouseCapture();
        void MouseRelease();

        #ifdef FORK_WINDOWS_PLATFORM
        void InitRawInputDevices();
        #endif

        /* === Members === */

        Video::RenderContextPtr renderContext_;

        ResizeCallback resizeCallback_;

        int mouseCaptureCounter_ = 0;

        AbstractFrame abstractFrame_;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================