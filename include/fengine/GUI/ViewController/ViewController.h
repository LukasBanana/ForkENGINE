/*
 * GUI view controller header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GUI_VIEW_CONTROLLER_H__
#define __FORK_GUI_VIEW_CONTROLLER_H__


#include "Core/Export.h"
#include "Core/Container/Identifiable.h"
#include "GUI/Core/BaseGUITypes.h"
#include "IO/FileSystem/File.h"

#include <vector>
#include <string>
#include <memory>
#include <functional>


namespace Fork
{

namespace GUI
{


class DrawContext;
class FocusEvent;
class SizeEvent;
class MoveEvent;
class MouseEvent;
class KeyEvent;
class UpdateEvent;
class DrawEvent;
class TriggerEvent;

/**
The view controller is the base class of all GUI elements.
There are several options every view controller has:
- Position
- Size
- Visibility State (Shown/Hidden)
- Activation State (Enabled/Disabled)
- Title
*/
class FORK_EXPORT ViewController : private Identifiable
{
    
    public:
        
        //! Base event handler for all GUI element events.
        class FORK_EXPORT EventHandler
        {
            
            public:
                
                virtual ~EventHandler();

                //! Callback when a view controller gets the focus. This occurs when the mouse is over the view controller.
                virtual void OnGetFocus     ( FocusEvent&   event );
                //! Callback when a view controller lost the focus. This occurs when the mouse was over the view controller.
                virtual void OnLostFocus    ( FocusEvent&   event );

                virtual void OnResize       ( SizeEvent&    event );
                virtual void OnMove         ( MoveEvent&    event );

                virtual void OnMouseDown    ( MouseEvent&   event );
                virtual void OnMouseUp      ( MouseEvent&   event );
                virtual void OnMouseMove    ( MouseEvent&   event );
                virtual void OnWheelMotion  ( MouseEvent&   event );

                virtual void OnUpdate       ( UpdateEvent&  event );
                virtual void OnDraw         ( DrawEvent&    event );
                
                virtual void OnTrigger      ( TriggerEvent& event );

            protected:
                
                EventHandler() = default;

                //! Locks the mouse focus.
                bool LockMouseFocus(ViewController* owner);
                //! Unlocks the mouse focus.
                bool UnlockMouseFocus();

                /**
                Returns true if this event handler has the mouse focus.
                \see LockMouseFocus
                */
                bool HasMouseFocus() const;

                //! Clamps the mouse area to the specified rectangle.
                void ClampMouse(const Rect& rect);
                //! Unclamps the mouse area.
                void UnclampMouse();

                virtual void OnMouseFocus();
                virtual void OnLostMouseFocus();

                //! Returns true if the button has the focus.
                inline bool HasFocus() const
                {
                    return hasFocus_;
                }

            private:

                bool hasFocus_ = false;

        };

        typedef std::shared_ptr<EventHandler> EventHandlerPtr;

        /* === Functions === */

        ViewController();
        virtual ~ViewController();

        /* --- Base functions --- */

        //! Returns the ID number of this view controller.
        inline Identifiable::IDType GetID() const
        {
            return Identifiable::GetID();
        }

        /**
        Sets the view controller event handler.
        \see EventHandler
        */
        inline void SetEventHandler(const EventHandlerPtr& eventHandler)
        {
            eventHandler_ = eventHandler;
        }

        /* --- Drawing and event handling --- */

        /**
        Updates internal states of this object and all its children if it's enabled.
        \see IsEnabled
        */
        virtual void Update();

        /**
        Draws this and all children view controllers.
        \param[in] context Specifies the draw context.
        \param[in] clientArea Specifies the global client area for drawing operations.
        \throws NullPointerException If 'context' is null.
        */
        virtual void Draw(DrawContext* context, Point parentPosition, const Rect& parentClientArea);

        /**
        Posts the get focus event.
        \see EventHandler::OnGetFocus
        */
        void PostGetFocus(const ViewController* prevFocusedElement);
        /**
        Posts the lost focus event.
        \see EventHandler::OnLostFocus
        */
        void PostLostFocus();

        /**
        Posts the mouse movement event.
        \param[out] focusedViewController Specifies the output view controller which is focused,
        i.e. the mouse is over this view controller. This will be set even if the function return value is false.
        \reamrks The focused view controller won't switch while the mouse focus is locked.
        \see EventHandler::OnMouseMove
        \see EventHandler::LockMouseFocus
        */
        bool PostMouseMovement(
            const Point& mousePosition, const Point& mouseMotion,
            const Point& parentPosition, ViewController*& focusedViewController
        );
        /**
        Posts the mouse key down event.
        \see EventHandler::OnMouseDown
        */
        bool PostMouseKeyDown(const Point& mousePosition, const MouseKeyCodes keyCode, const Point& parentPosition);
        /**
        Posts the mouse key up event.
        \see EventHandler::OnMouseUp
        */
        bool PostMouseKeyUp(const Point& mousePosition, const MouseKeyCodes keyCode, const Point& parentPosition);
        /**
        Posts the mouse wheel motion event.
        \see EventHandler::OnWheelMotion
        */
        bool PostWheelMotion(const Point& mousePosition, int wheelMotion, const Point& parentPosition);

        /**
        Post the trigger event. This will be call when a button is pushed for instance.
        \see EventHandler::OnTrigger
        */
        void PostTrigger(TriggerIDType id);

        /* --- Location and size --- */

        //! Returns the global position on the screen.
        Point GlobalPosition() const;
        //! Returns the local area relative to the view controller parent.
        Rect LocalArea() const;
        /**
        Returns the global area.
        \see GlobalPosition
        */
        Rect GlobalArea() const;

        /**
        Returns the client area relative to the view controller's position.
        By default the entire area of the view controller is returned.
        \see GlobalClientArea()
        \see ClientAreaBorder
        */
        Rect ClientArea() const;
        /**
        Returns the global client area.
        \see ClientArea
        */
        Rect GlobalClientArea() const;

        //! Resizes this GUI element.
        virtual void Resize(Size size);

        //! Returns the size of this GUI element.
        inline const Size& GetSize() const
        {
            return size_;
        }

        //! Sets the new minimal size. By default { 0, 0 }.
        void MinResize(const Size& minSize);

        //! Returns the minimal size of this GUI element.
        inline const Size& GetMinSize() const
        {
            return minSize_;
        }

        /**
        Sets the new maximal size. By default Size(std::numeric_limits<int>::max()).
        \remarks The minimal size is higher priority, i.e. if this is smaller than the minimal size it will be clamped.
        */
        void MaxResize(const Size& maxSize);

        //! Returns the maximal size of this GUI element.
        inline const Size& GetMaxSize() const
        {
            return maxSize_;
        }

        /**
        Sets the new position of this GUI element.
        \param[in] position Specifies the local position relative to the view controller parent.
        \see GetParent
        */
        virtual void Locate(const Point& position);
        /**
        Moves this GUI element.
        \param[in] direction Specifies the move direction.
        \see Locate
        */
        void Move(const Point& direction);

        //! Returns the local position of this GUI element.
        inline const Point& GetPosition() const
        {
            return position_;
        }

        /**
        Makes this to the new top level view controller.
        After this call, the z layer is 0.
        \see GetZLayer
        */
        void MakeTopLevel();

        /**
        Returns true if the global point overlaps the local area relative to this view controller.
        This can be used to easily check if the mouse position overlaps a specific area inside this view controller.
        */
        bool Overlap(const Rect& localArea, Point globalPoint) const;

        /* --- Parenting --- */

        /**
        Set the parent object of this view controller.
        \return True on success. Otherwise a parenting cycle was detected.
        \note This must never be called while iterating over the children list:
        \code
        // False example:
        for (auto child : viewCtrl->GetChildren())
            child->SetupParent(otherViewCtrl); // <-- ERROR
        
        // Correct example:
        auto children = viewCtrl->GetChildren(); // get a copy of the list
        for (auto child : children)
            child->SetupParent(otherViewCtrl); // no problems here
        \endcode
        */
        bool SetupParent(ViewController* parent);

        //! Returns the parent object of this view controller.
        inline ViewController* GetParent() const
        {
            return parent_;
        }

        //! Returns true if the specified object is the parent or any upper parent of this GUI element.
        bool IsParent(const ViewController* object) const;

        /**
        Returns the list of all children objects. The list is always sorted by the Z layer.
        The first element is the top level view controller (with Z layer 0).
        \see GetZLayer
        */
        inline const std::vector<ViewController*>& GetChildren() const
        {
            return children_;
        }

        /**
        Locks the specified child to this view controller. Only this child will receive input events.
        \param[in] child Specifies the child which is to be locked to this view controller.
        This doesn't need to be a child of this view controller. If this is null, the function has no effect and returns false.
        \return True if the child could be locked. Otherwise another child is already locked to this view controller.
        \remarks This function will also make the child a top level view controller (see "MakeTopLevel"),
        but only if the function returns true.
        \see UnlockChild
        \see MakeTopLevel
        */
        bool LockChild(ViewController* child);
        /**
        Unlocks the specified child from this view controller.
        \return True if the child could be unlcoked. Otherwise another child is already locked to this view controller.
        \see LockChild
        */
        bool UnlockChild(ViewController* child);

        /* --- States --- */

        //! Writes the entire state of this view controller to the specified file.
        virtual void WriteToFile(IO::File& file);
        //! Reads the entire state of this view controller to the specified file.
        virtual void ReadFromFile(IO::File& file);

        //! Sets the title of this GUI element.
        void SetupTitle(const String& title);

        //! Returns the title of this GUI element.
        inline const String& GetTitle() const
        {
            return title_;
        }

        //! Shows or hides this GUI element. By default visible.
        void Show(bool show = true);
        //! Returns true if this GUI element is visible. All parent GUI elements must also be visible if 'global' is true.
        bool IsVisible(bool global) const;

        //! Returns true if this GUI element is visible.
        inline bool IsVisible() const
        {
            return isVisible_;
        }

        //! Enables or disables this GUI element. By default enabled.
        void Enable(bool enable = true);
        //! Returns true if this GUI element is enabled. All parent GUI elements must also be enabled if 'global' is true.
        bool IsEnabled(bool global) const;

        //! Returns true if this GUI element is enabled.
        inline bool IsEnabled() const
        {
            return isEnabled_;
        }

        //! Returns the Z (also called "depth"-) layer. The top level view controller has the Z layer 0.
        inline unsigned int GetZLayer() const
        {
            return zLayer_;
        }

        /**
        Returns true if this is a top level view controller. This is the case when the Z layer is zero.
        \see GetZLayer
        */
        inline bool IsTopLevel() const
        {
            return GetZLayer() == 0;
        }

        /**
        Returns the view controller's scroll offset.
        This afffects the global position of all children view controllers.
        \remarks This member can only be modified by sub classes of "ViewController".
        */
        inline const Point& GetScrollOffset() const
        {
            return scrollOffset;
        }

        /**
        Returns the redirection parent view controller.
        This will be used when "SetupParent" is called. May also be null.
        \see SetupParent
        */
        inline ViewController* GetRedirectParent() const
        {
            return redirectParent;
        }

        /**
        Specifies the view controller's local scroll offset.
        This will affect the global position of all children view controllers. By default { 0, 0 }.
        */
        Point scrollOffset;//!TODO! -> should not be public!!!

    protected:
        
        /**
        Finalizes the construction of this view controller.
        This must be called as last function in the respective constructors of the view controller sub classes.
        */
        void Finalize();

        /**
        Returns the border of the client area. This is independent of the view controller's
        position and size and it should be a fixed value. By default { 0, 0, 0, 0 }.
        */
        virtual Rect ClientAreaBorder() const;

        //! Creates a new event handler.
        template <class EvtHandler, typename... Args> void MakeEventHandler(Args&&... args)
        {
            SetEventHandler(std::make_shared<EvtHandler>(args...));
        }

        /**
        Redirection parent view controller. This can be used to redirect
        this view controller (as a parent) to a child object, e.g. to a scroll area.
        If this is null, no redirection is done.
        \see SetupParent
        */
        ViewController* redirectParent = nullptr;

    private:
        
        /* === Functions === */

        void SortChildrenZLayers();
        void ReAssignChildrenLayers();
        void MakeChildTopLevel(ViewController* topLevelChild);

        bool OverlapGlobalArea(const Point& point, const Point& parentPosition) const;
        Rect GlobalClientArea(const Point& parentPosition) const;

        void ClampSizeRange();

        /* === Members === */

        EventHandlerPtr eventHandler_;

        Point           position_;
        Size            size_;
        unsigned int    zLayer_         = 0;

        String          title_;
        bool            isVisible_      = true;
        bool            isEnabled_      = true;

        Size            minSize_;
        Size            maxSize_;

        ViewController* parent_         = nullptr;
        ViewController* lockedChild_    = nullptr;

        std::vector<ViewController*> children_;

};



} // /namespace GUI

} // /namespace Fork


#endif



// ========================