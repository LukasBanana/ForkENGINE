/*
 * Projector file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/Projector/Projector.h"
#include "Core/Exception/NullPointerException.h"


namespace Fork
{

namespace Utility
{


Projector::Projector(
    const Video::PrimitiveRendererPtr& renderer, const Video::TextureFontPtr& labelFont,
    const ProjectorModelPtr& model, const ProjectorViewPtr& view) :
       renderer_{ renderer },
       model_   { model    },
       view_    { view     }
{
    ASSERT_POINTER(renderer);

    /* Setup projector model and view */
    if (!model_)
        model_ = std::make_shared<ProjectorModel>();
    if (!view_)
        view_ = std::make_shared<ProjectorView>(GetModel(), labelFont_.get());

    /* Add mouse event handler */
    mouseEventHandler_ = std::make_shared<MouseEventHandler>(this);
    IO::Mouse::Instance()->AddEventHandler(mouseEventHandler_);
}
Projector::~Projector()
{
    /* Remove event handlers */
    IO::Mouse::Instance()->RemoveEventHandler(mouseEventHandler_);
}

void Projector::Draw(const ProjectorView::DrawFlags::DataType flags)
{
    GetView()->Draw(GetRenderer(), flags);
}

void Projector::DrawAndUpdate(const ProjectorView::DrawFlags::DataType flags)
{
    GetModel()->Update();
    GetView()->Draw(GetRenderer(), flags);
}


/*
 * ======= Protected: =======
 */

void Projector::ExchangeMouseEventHandler(const std::shared_ptr<MouseEventHandler>& eventHandler)
{
    if (eventHandler)
    {
        IO::Mouse::Instance()->RemoveEventHandler(mouseEventHandler_);
        mouseEventHandler_ = eventHandler;
        IO::Mouse::Instance()->AddEventHandler(mouseEventHandler_);
    }
}


/*
 * MouseEventHandler class
 */

Projector::MouseEventHandler::MouseEventHandler(Projector* projector) :
    projector_{ projector }
{
    ASSERT_POINTER(projector);
}

bool Projector::MouseEventHandler::OnButtonDown(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame)
{
    if (keyCode == IO::MouseKeyCodes::MouseLeft)
        projector_->GetModel()->ProcessActivation(mousePos_);
    return true;
}

bool Projector::MouseEventHandler::OnButtonDoubleClicked(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame)
{
    return OnButtonDown(keyCode, frame);
}

bool Projector::MouseEventHandler::OnLocalMotion(const Math::Point2i& position, const Platform::Frame* frame)
{
    mousePos_ = position.Cast<float>();
    if (projector_->GetModel()->ProcessSelection(mousePos_))
        LockFocus();
    else if (HasFocus())
        UnlockFocus();
    return true;
}


} // /namespace Utility

} // /namespace Fork



// ========================