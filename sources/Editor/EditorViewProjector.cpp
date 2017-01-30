/*
 * Editor view projector file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "EditorViewProjector.h"
#include "EditorView.h"
#include "Core/Devices.h"
#include "Math/Core/Zero.h"
#include "Core/DefaultColors.h"
#include "Core/UIExtensions/AbstractFrame.h"


namespace Fork
{

namespace Editor
{


using namespace Utility;

/* 
 * ProjectorModelExt (internal) class
 */

class ProjectorModelExt : public ProjectorModel
{
        
    public:
            
        ProjectorModelExt(Scene::FreeViewSceneNodeController& cameraCtrl) :
            cameraCtrl_{ &cameraCtrl }
        {
        }

        void SetupOrientation(
            const Math::Quaternionf& orientation,
            unsigned long long transitionDuration = 0)
        {
            /*
            Setup new rotation for camera controller
            -> Examine special case when 'orientation.w' is nearly zero
            -> In this case, set 'pitch' and 'yaw' manually.
            */
            if (Math::Zero(orientation.w))
            {
                cameraCtrl_->pitch  = 0.0f;
                cameraCtrl_->yaw    = Math::pi;
            }
            else
                cameraCtrl_->SetupRotation(orientation);

            ProjectorModel::SetupOrientation(orientation, transitionDuration);
        }

    private:
            
        Scene::FreeViewSceneNodeController* cameraCtrl_ = nullptr;
            
};


/*
 * EditorViewProjector class
 */

EditorViewProjector::EditorViewProjector(EditorView& owner, Scene::FreeViewSceneNodeController& cameraCtrl) :
    Projector
    {
        Devices::primitiveRenderer,
        nullptr,
        std::make_shared<ProjectorModelExt>(cameraCtrl)
    },
    owner_{ &owner }
{
    /* Use specialized mouse event handler to only react to dedicated mouse events */
    ExchangeMouseEventHandler(std::make_shared<ProjectorMouseEventHandler>(this));

    /* Initialize projector orientation */
    GetModel()->orientationSource = &(owner_->GetCamera()->transform);

    /* Initialize color configuration */
    auto& projTriggerCfg = GetView()->config.triggers;

    projTriggerCfg.left  .normalColor = DefaultColors::xAxis;
    projTriggerCfg.top   .normalColor = DefaultColors::yAxis;
    projTriggerCfg.front .normalColor = DefaultColors::zAxis;
    projTriggerCfg.right .normalColor = DefaultColors::xAxis;
    projTriggerCfg.bottom.normalColor = DefaultColors::yAxis;
    projTriggerCfg.back  .normalColor = DefaultColors::zAxis;
}


/*
 * ProjectorMouseEventHandler class
 */

EditorViewProjector::ProjectorMouseEventHandler::ProjectorMouseEventHandler(Projector* projector) :
    MouseEventHandler{ projector }
{
}

bool EditorViewProjector::ProjectorMouseEventHandler::OnButtonDown(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame)
{
    return IsDedicatedEvent(frame) ? MouseEventHandler::OnButtonDown(keyCode, frame) : true;
}

bool EditorViewProjector::ProjectorMouseEventHandler::OnButtonDoubleClicked(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame)
{
    return IsDedicatedEvent(frame) ? MouseEventHandler::OnButtonDoubleClicked(keyCode, frame) : true;
}

bool EditorViewProjector::ProjectorMouseEventHandler::OnLocalMotion(const Math::Point2i& position, const Platform::Frame* frame)
{
    return IsDedicatedEvent(frame) ? MouseEventHandler::OnLocalMotion(position, frame) : true;
}

bool EditorViewProjector::ProjectorMouseEventHandler::IsDedicatedEvent(const Platform::Frame* frame) const
{
    /* Always accept this event if this event handler has the focus */
    if (HasFocus())
        return true;

    /* Check if the event originates from the frame where this projector belongs to */
    auto screenFrame = dynamic_cast<const AbstractFrame*>(frame);

    if (screenFrame)
    {
        auto editorView = reinterpret_cast<EditorView*>(screenFrame->userData);
        if (editorView)
            return editorView->GetProjector() == GetProjector();
    }

    return false;
}


} // /namespace Editor

} // /namespace Fork



// ========================