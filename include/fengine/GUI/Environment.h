/*
 * GUI environment header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GUI_ENVIRONMENT_H__
#define __FORK_GUI_ENVIRONMENT_H__


#include "Core/Export.h"
#include "GUI/Core/DrawContext.h"
#include "GUI/Core/BaseGUITypes.h"
#include "GUI/Cursor.h"

#include "GUI/ViewController/Frame.h"
#include "GUI/ViewController/Button.h"
#include "GUI/ViewController/ScrollBar.h"

#include "Video/AdvancedRenderer/PrimitiveRenderer.h"//!!!

#include <vector>
#include <memory>


namespace Fork
{

namespace GUI
{


class FORK_EXPORT Environment
{
    
    public:
        
        /**
        GUI environment contstructor.
        \param[in] context Specifies the draw context. If this is null, the default draw context will be created.
        */
        Environment(const Video::PrimitiveRendererPtr& renderer /*!!!*/, const DrawContextPtr& context = nullptr);

        Environment(const Environment&) = delete;
        Environment& operator = (const Environment&) = delete;

        /* --- Drawing and event handling --- */

        void Update();
        void Draw();

        //! Sets the new cursor position of this GUI environment.
        void SetupCursorPosition(Point position);

        //! Posts the mouse key down event to the GUI environment.
        void PostMouseKeyDown(const MouseKeyCodes keyCode);
        //! Posts the mouse key up event to the GUI environment.
        void PostMouseKeyUp(const MouseKeyCodes keyCode);
        //! Posts the mouse wheel motion event to the GUI environment.
        void PostWheelMotion(int wheelMotion);

        /* --- View controllers --- */

        //! Creates a new view controller and initializes it with the specified arguments.
        template <typename T, typename... Args> T* CreateViewController(Args&&... args)
        {
            auto ctrl = std::make_unique<T>(args...);
            auto ctrlRef = ctrl.get();
            viewControllers_.push_back(std::move(ctrl));
            return ctrlRef;
        }

        /**
        Deletes the specified view controller.
        \note Make sure you don't use this pointer anymore after this call!
        */
        void DeleteViewController(ViewController* viewCtrl);

        //! Creates a new frame.
        Frame* CreateFrame(
            const Point& position, const Size& size, const String& title = L"",
            ViewController* parent = nullptr, const Frame::Flags::DataType flags = Frame::Flags::TitleBar
        );
        //! Creates a new button.
        Button* CreateButton(
            const Point& position, const Size& size, const String& title = L"",
            ViewController* parent = nullptr, const Button::Flags::DataType flags = Button::Flags::DoubleFrame
        );
        //! Creates a new scroll bar.
        ScrollBar* CreateScrollBar(
            const Point& position, const Size& size, bool isVertical = false, ViewController* parent = nullptr,
            const ScrollBar::Flags::DataType flags = ScrollBar::Flags::SingleStepButtons
        );

        /* --- Misc --- */
        
        //! Returns the GUI environment cursor.
        inline const Cursor& GetCursor() const
        {
            return cursor_;
        }

        //! Returns the desktop view controller.
        inline ViewController* GetDesktop()
        {
            return &desktop_;
        }
        //! Returns the desktop view controller.
        inline const ViewController* GetDesktop() const
        {
            return &desktop_;
        }

        //! Global GUI environment client area for drawing operations.
        Rect clientArea;

    private:
        
        void InitViewController(
            ViewController* viewCtrl, const Point& position,
            const Size& size, const String& title, ViewController* parent
        );

        /* === Members === */

        DrawContextPtr                                  context_;
        
        ViewController                                  desktop_;
        std::vector<std::unique_ptr<ViewController>>    viewControllers_;

        Cursor                                          cursor_;

        ViewController*                                 prevFocusedViewController_ = nullptr;

};



} // /namespace GUI

} // /namespace Fork


#endif



// ========================