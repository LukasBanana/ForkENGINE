/*
 * GUI view controller file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GUI/ViewController/ViewController.h"
#include "GUI/Core/DrawContext.h"
#include "GUI/Cursor.h"
#include "Core/STLHelper.h"
#include "Core/Container/IdentityFactory.h"
#include "Core/Exception/NullPointerException.h"
#include "Math/Core/BaseMath.h"
#include "../EventSystem/ImportEvents.h"
#include "../EventSystem/MouseLock.h"

#include <algorithm>
#include <limits>


namespace Fork
{

namespace GUI
{


/*
 * EventHandler class
 */

ViewController::EventHandler::~EventHandler()
{
}

void ViewController::EventHandler::OnGetFocus(FocusEvent& event)
{
    hasFocus_ = true;
}

void ViewController::EventHandler::OnLostFocus(FocusEvent& event)
{
    hasFocus_ = false;
}

void ViewController::EventHandler::OnResize(SizeEvent& event)
{
    // dummy
}

void ViewController::EventHandler::OnMove(MoveEvent& event)
{
    // dummy
}

void ViewController::EventHandler::OnMouseDown(MouseEvent& event)
{
    // dummy
}

void ViewController::EventHandler::OnMouseUp(MouseEvent& event)
{
    // dummy
}

void ViewController::EventHandler::OnMouseMove(MouseEvent& event)
{
    // dummy
}

void ViewController::EventHandler::OnWheelMotion(MouseEvent& event)
{
    // dummy
}

void ViewController::EventHandler::OnUpdate(UpdateEvent& event)
{
    // dummy
}

void ViewController::EventHandler::OnDraw(DrawEvent& event)
{
    // dummy
}

void ViewController::EventHandler::OnTrigger(TriggerEvent& event)
{
    // dummy
}

bool ViewController::EventHandler::LockMouseFocus(ViewController* owner)
{
    /* Check if event handler is already locked */
    if (owner)
    {
        if (!GetMouseLock().eventHandler)
        {
            /* Lock this event handler */
            GetMouseLock().eventHandler = this;
            GetMouseLock().owner = owner;
            OnMouseFocus();
            return true;
        }
        return HasMouseFocus();
    }
    return false;
}

bool ViewController::EventHandler::UnlockMouseFocus()
{
    /* Check if this event handler has the current mouse focus */
    if (HasMouseFocus())
    {
        /* Unlock this event handler */
        GetMouseLock().eventHandler = nullptr;
        OnLostMouseFocus();
        return true;
    }
    return false;
}

bool ViewController::EventHandler::HasMouseFocus() const
{
    return GetMouseLock().eventHandler == this;
}

void ViewController::EventHandler::ClampMouse(const Rect& rect)
{
    GetMouseLock().Clamp(rect);
}

void ViewController::EventHandler::UnclampMouse()
{
    GetMouseLock().Unclamp();
}

void ViewController::EventHandler::OnMouseFocus()
{
    // dummy
}

void ViewController::EventHandler::OnLostMouseFocus()
{
    // dummy
}


/*
 * ViewController class
 */

//! Internal GUI factory for view controller ID numbers.
static std::shared_ptr<IdentityFactory> viewControllerIDFactory = std::make_shared<IdentityFactory>();

ViewController::ViewController() :
    Identifiable{ viewControllerIDFactory         },
    maxSize_    { std::numeric_limits<int>::max() }
{
}
ViewController::~ViewController()
{
}

/* --- Drawing and event handling --- */

void ViewController::Update()
{
    if (IsEnabled())
    {
        /* Post update event to this view controller */
        UpdateEvent event(this);

        if (eventHandler_)
            eventHandler_->OnUpdate(event);

        if (!event.IsVeto())
        {
            /* Update all children */
            for (auto it = children_.rbegin(); it != children_.rend(); ++it)
                (*it)->Update();
        }
    }
}

void ViewController::Draw(DrawContext* context, Point parentPosition, const Rect& parentClientArea)
{
    ASSERT_POINTER(context);

    /* Omit drawing if this object is invisible */
    if (!IsVisible())
        return;

    /* Setup client area to draw this view controller */
    parentPosition += GetPosition();

    if (GetParent())
        parentPosition += GetParent()->GetScrollOffset();

    auto clientArea = parentClientArea;

    clientArea.ClampMin({
        parentPosition.x,
        parentPosition.y,
        parentPosition.x + GetSize().width,
        parentPosition.y + GetSize().height
    });

    if (clientArea.Width() > 0 && clientArea.Height() > 0)
    {
        context->PushScissor(clientArea);
        
        /* Post draw event to this view controller */
        DrawEvent event(this, context, clientArea);

        if (eventHandler_)
            eventHandler_->OnDraw(event);

        if (!children_.empty() && !event.IsVeto())
        {
            /* Setup new client area for the children view controllers */
            auto localClientArea = ClientArea();

            parentPosition += localClientArea.LTPoint();

            clientArea.ClampMin({
                parentPosition.x,
                parentPosition.y,
                parentPosition.x + localClientArea.Width(),
                parentPosition.y + localClientArea.Height()
            });

            if (clientArea.Width() > 0 && clientArea.Height() > 0)
            {
                /*
                Draw all children view controllers
                -> Draw from back to front, because top level element is the first but must be drawn at last
                */
                context->PushScissor(clientArea);
                
                for (auto it = children_.rbegin(); it != children_.rend(); ++it)
                    (*it)->Draw(context, parentPosition, clientArea);
                
                context->PopScissor();
            }
        }
        
        context->PopScissor();
    }
}

void ViewController::PostGetFocus(const ViewController* prevFocusedElement)
{
    if (eventHandler_)
    {
        FocusEvent event(this, prevFocusedElement);
        eventHandler_->OnGetFocus(event);
    }
}

void ViewController::PostLostFocus()
{
    if (eventHandler_)
    {
        FocusEvent event(this);
        eventHandler_->OnLostFocus(event);
    }
}

bool ViewController::PostMouseMovement(
    const Point& mousePosition, const Point& mouseMotion, const Point& parentPosition, ViewController*& focusedViewController)
{
    if (!IsEnabled() || !OverlapGlobalArea(mousePosition, parentPosition))
        return false;

    /* Get global client area from this view controller */
    auto clientArea = GlobalClientArea(parentPosition);

    /* Check for locked child */
    if (lockedChild_)
    {
        /* Post event to locked child */
        auto globalPosition = clientArea.LTPoint() + GetScrollOffset();
        lockedChild_->PostMouseMovement(mousePosition, mouseMotion, globalPosition, focusedViewController);
        return true;
    }
    
    /* Check if mouse position is inside the client area */
    if (clientArea.Overlap(mousePosition))
    {
        /* Post event to top level children */
        auto globalPosition = clientArea.LTPoint() + GetScrollOffset();
        for (auto& child : children_)
        {
            if (child->PostMouseMovement(mousePosition, mouseMotion, globalPosition, focusedViewController))
                return true;
        }
    }

    /* Post event to this view controller's event handler */
    focusedViewController = this;

    MouseEvent event(this, mousePosition, mousePosition - parentPosition - GetPosition(), mouseMotion);

    if (eventHandler_)
        eventHandler_->OnMouseMove(event);

    return !event.IsVeto();
}

bool ViewController::PostMouseKeyDown(const Point& mousePosition, const MouseKeyCodes keyCode, const Point& parentPosition)
{
    if (!IsEnabled() || !OverlapGlobalArea(mousePosition, parentPosition))
        return false;

    /* Get global client area from this view controller */
    auto clientArea = GlobalClientArea(parentPosition);

    /* Check for locked child */
    if (lockedChild_)
    {
        /* Post event to locked child */
        auto globalPosition = clientArea.LTPoint() + GetScrollOffset();
        lockedChild_->PostMouseKeyDown(mousePosition, keyCode, globalPosition);
        return true;
    }
    
    /* Make this view controller to top level */
    MakeTopLevel();

    /* Check if mouse position is inside the client area */
    if (clientArea.Overlap(mousePosition))
    {
        /* Post event to top level children */
        auto globalPosition = clientArea.LTPoint() + GetScrollOffset();
        for (auto& child : children_)
        {
            if (child->PostMouseKeyDown(mousePosition, keyCode, globalPosition))
                return true;
        }
    }

    /* Post event to this view controller's event handler */
    MouseEvent event(this, mousePosition, mousePosition - parentPosition - GetPosition(), keyCode);

    if (eventHandler_)
        eventHandler_->OnMouseDown(event);

    return !event.IsVeto();
}

bool ViewController::PostMouseKeyUp(const Point& mousePosition, const MouseKeyCodes keyCode, const Point& parentPosition)
{
    if (!IsEnabled() || !OverlapGlobalArea(mousePosition, parentPosition))
        return false;

    /* Get global client area from this view controller */
    auto clientArea = GlobalClientArea(parentPosition);

    /* Check for locked child */
    if (lockedChild_)
    {
        /* Post event to locked child */
        auto globalPosition = clientArea.LTPoint() + GetScrollOffset();
        lockedChild_->PostMouseKeyUp(mousePosition, keyCode, globalPosition);
        return true;
    }
    
    /* Check if mouse position is inside the client area */
    if (clientArea.Overlap(mousePosition))
    {
        /* Post event to top level children */
        auto globalPosition = clientArea.LTPoint() + GetScrollOffset();
        for (auto& child : children_)
        {
            if (child->PostMouseKeyUp(mousePosition, keyCode, globalPosition))
                return true;
        }
    }

    /* Post event to this view controller's event handler */
    MouseEvent event(this, mousePosition, mousePosition - parentPosition - GetPosition(), keyCode);

    if (eventHandler_)
        eventHandler_->OnMouseUp(event);

    return !event.IsVeto();
}

bool ViewController::PostWheelMotion(const Point& mousePosition, int wheelMotion, const Point& parentPosition)
{
    if (!IsEnabled() || !OverlapGlobalArea(mousePosition, parentPosition))
        return false;

    /* Get global client area from this view controller */
    auto clientArea = GlobalClientArea(parentPosition);

    /* Check for locked child */
    if (lockedChild_)
    {
        /* Post event to locked child */
        auto globalPosition = clientArea.LTPoint() + GetScrollOffset();
        lockedChild_->PostWheelMotion(mousePosition, wheelMotion, globalPosition);
        return true;
    }
    
    /* Check if mouse position is inside the client area */
    if (clientArea.Overlap(mousePosition))
    {
        /* Post event to top level children */
        auto globalPosition = clientArea.LTPoint() + GetScrollOffset();
        for (auto& child : children_)
        {
            if (child->PostWheelMotion(mousePosition, wheelMotion, globalPosition))
                return true;
        }
    }

    /* Post event to this view controller's event handler */
    MouseEvent event(this, mousePosition, mousePosition - parentPosition - GetPosition(), wheelMotion);

    if (eventHandler_)
        eventHandler_->OnWheelMotion(event);

    return !event.IsVeto();
}

void ViewController::PostTrigger(TriggerIDType id)
{
    if (eventHandler_)
    {
        TriggerEvent event(this, id);
        eventHandler_->OnTrigger(event);
    }
}

/* --- Location and size --- */

Point ViewController::GlobalPosition() const
{
    auto position = GetPosition();

    if (GetParent())
    {
        position += GetParent()->GlobalPosition();
        position += GetParent()->ClientArea().LTPoint();
        position += GetParent()->GetScrollOffset();
    }

    return position;
}

Rect ViewController::LocalArea() const
{
    return { GetPosition(), GetSize() };
}

Rect ViewController::GlobalArea() const
{
    return { GlobalPosition(), GetSize() };
}

Rect ViewController::ClientArea() const
{
    auto border = ClientAreaBorder();
    return { Point(border.left, border.top),
             GetSize() - Size(border.right + border.left, border.bottom + border.top) };
}

Rect ViewController::GlobalClientArea() const
{
    auto clientArea = ClientArea();
    clientArea.Move(GetPosition());
    
    if (GetParent())
    {
        auto parentClientArea = GetParent()->GlobalClientArea();
        clientArea.Move(parentClientArea.LTPoint());
    }

    return clientArea;
}

void ViewController::Resize(Size size)
{
    /* Clamp size to limits */
    size.width  = Math::Clamp(size.width,  GetMinSize().width,  GetMaxSize().width );
    size.height = Math::Clamp(size.height, GetMinSize().height, GetMaxSize().height);

    /* Only post an event if the size has changed */
    if (size_ != size)
    {
        /* Post size event */
        SizeEvent event(this, size);

        if (eventHandler_)
            eventHandler_->OnResize(event);

        if (!event.IsVeto())
            size_ = event.size;
    }
}

void ViewController::MinResize(const Size& minSize)
{
    minSize_ = minSize;
    ClampSizeRange();
}

void ViewController::MaxResize(const Size& maxSize)
{
    maxSize_ = maxSize;
    ClampSizeRange();
}

void ViewController::Locate(const Point& position)
{
    /* Only post an event if the position has changed */
    if (position_ != position)
    {
        /* Post move event */
        MoveEvent event(this, position);

        if (eventHandler_)
            eventHandler_->OnMove(event);

        if (!event.IsVeto())
            position_ = event.position;
    }
}

void ViewController::Move(const Point& direction)
{
    Locate(GetPosition() + direction);
}

void ViewController::MakeTopLevel()
{
    if (GetParent())
    {
        GetParent()->MakeTopLevel();
        if (!IsTopLevel())
            GetParent()->MakeChildTopLevel(this);
    }
}

bool ViewController::Overlap(const Rect& localArea, Point globalPoint) const
{
    globalPoint -= GlobalPosition();
    return localArea.Overlap(globalPoint);
}

/* --- Parenting --- */

bool ViewController::SetupParent(ViewController* parent)
{
    /* Use root parent if it's not null */
    if (parent && parent->GetRedirectParent())
        parent = parent->GetRedirectParent();

    if (parent_ != parent)
    {
        if (parent_)
        {
            /* Remove this object from the previous parent's children list and re-assign the children z layers */
            RemoveFromList(parent_->children_, this);
            ReAssignChildrenLayers();
        }

        /* Remove previous parent and clear z layer */
        parent_ = nullptr;
        zLayer_ = 0;

        /* Check if new parent is this parent, also check for cycles */
        if ( parent != nullptr && ( parent == this || parent->IsParent(this) ) )
            return false;
        
        /* Set new parnet */
        parent_ = parent;

        if (parent)
        {
            /* Set z layer to the last entry */
            if (!parent->children_.empty())
                zLayer_ = parent->children_.back()->GetZLayer() + 1;

            /* Add this object to the new parent's children list */
            parent->children_.push_back(this);
        }
    }
    return true;
}

bool ViewController::IsParent(const ViewController* object) const
{
    if (object)
    {
        /* Check if this view controller is a child of the specified object */
        for (const auto& child : object->GetChildren())
        {
            if (child == this)
                return true;
        }

        /* Check the same for the next parent */
        if (GetParent())
            return GetParent()->IsParent(object);
    }
    return false;
}

bool ViewController::LockChild(ViewController* child)
{
    if (child)
    {
        if (lockedChild_ == nullptr)
        {
            lockedChild_ = child;
            child->MakeTopLevel();
            return true;
        }
        return lockedChild_ == child;
    }
    return false;
}

bool ViewController::UnlockChild(ViewController* child)
{
    if (child && lockedChild_ == child)
    {
        lockedChild_ = nullptr;
        return true;
    }
    return false;
}

/* --- States --- */

void ViewController::WriteToFile(IO::File& file)
{
    file.Write(position_);
    file.Write(size_);
    file.Write(zLayer_);
    //file.WriteStringSized(title_);
    file.Write(isVisible_);
    file.Write(isEnabled_);
}

void ViewController::ReadFromFile(IO::File& file)
{
    position_   = file.Read<Point>();
    size_       = file.Read<Size>();
    zLayer_     = file.Read<unsigned int>();
    //title_      = file.ReadStringSized();
    isVisible_  = file.Read<bool>();
    isEnabled_  = file.Read<bool>();
}

void ViewController::SetupTitle(const String& title)
{
    title_ = title;
}

void ViewController::Show(bool show)
{
    isVisible_ = show;
}

bool ViewController::IsVisible(bool global) const
{
    return GetParent() && global ? GetParent()->IsVisible(true) && isVisible_ : isVisible_;
}

void ViewController::Enable(bool enable)
{
    isEnabled_ = enable;
}

bool ViewController::IsEnabled(bool global) const
{
    return GetParent() && global ? GetParent()->IsEnabled(true) && isEnabled_ : isEnabled_;
}


/*
 * ======= Protected: =======
 */

void ViewController::Finalize()
{
    ClampSizeRange();
}

Rect ViewController::ClientAreaBorder() const
{
    return {};
}


/*
 * ======= Private: =======
 */

void ViewController::SortChildrenZLayers()
{
    std::sort(
        children_.begin(), children_.end(),
        [&](const ViewController* lhs, const ViewController* rhs)
        {
            return lhs->GetZLayer() < rhs->GetZLayer();
        }
    );
}

void ViewController::ReAssignChildrenLayers()
{
    size_t zLayer = 0;
    for (auto& child : children_)
        child->zLayer_ = zLayer++;
}

//!TODO! -> optimize this!!!
void ViewController::MakeChildTopLevel(ViewController* topLevelChild)
{
    /* Increment the z layer of all children which are before the new top level child */
    for (auto& child : children_)
    {
        if (child == topLevelChild)
            break;
        else
            ++child->zLayer_;
    }

    /* Set top level child's z layer to 0 */
    topLevelChild->zLayer_ = 0;

    SortChildrenZLayers();//!!!(optimize this here)
}

bool ViewController::OverlapGlobalArea(const Point& point, const Point& parentPosition) const
{
    /* Check if mouse position is inside this view controller's global area */
    auto globalArea = LocalArea();
    globalArea.Move(parentPosition);
    return globalArea.Overlap(point);
}

Rect ViewController::GlobalClientArea(const Point& parentPosition) const
{
    auto clientArea = ClientArea();
    clientArea.Move(parentPosition + GetPosition());
    return clientArea;
}

void ViewController::ClampSizeRange()
{
    /* Get size limits by client area (and clamp it if the client area is invalid) */
    auto border = ClientAreaBorder();
    auto size = Size(border.left + border.right, border.top + border.bottom);

    size.width  = std::max(0, size.width );
    size.height = std::max(0, size.height);

    /* Clamp min/max sizes */
    minSize_.width  = std::max(size.width , minSize_.width );
    minSize_.height = std::max(size.height, minSize_.height);

    maxSize_.width  = std::max(minSize_.width , maxSize_.width );
    maxSize_.height = std::max(minSize_.height, maxSize_.height);
}


} // /namespace GUI

} // /namespace Fork



// ========================