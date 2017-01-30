/*
 * Selector file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Utility/Selector/Selector.h"
#include "Core/Exception/NullPointerException.h"
#include "Scene/Node/CameraNode.h"
#include "Platform/Core/Cursor.h"
#include "Platform/Core/Frame.h"


namespace Fork
{

namespace Utility
{


Selector::Selector(
    const Video::PrimitiveRendererPtr& renderer, Scene::SceneManager* sceneManager,
    const Video::TextureFontPtr& labelFont, const SelectorModelPtr& model, const SelectorViewPtr& view) :
        renderer_   { renderer },
        model_      { model    },
        view_       { view     }
{
    ASSERT_POINTER(renderer);

    if (!labelFont_)
    {
        /* Create default label font */
        Video::Font::Description fontDesc;
        
        fontDesc.name           = L"arial";
        fontDesc.size.height    = 15;
        fontDesc.firstGlyph     = L'X';
        fontDesc.lastGlyph      = L'Z';
        
        labelFont_ = renderer_->CreateTextureFont(fontDesc);
    }
    else
    {
        auto fontDesc = labelFont_->GetDesc();

        if (fontDesc.firstGlyph > 'X' || fontDesc.lastGlyph < 'Z')
        {
            /* Create new label font with description adjustment */
            fontDesc.firstGlyph = L'X';
            fontDesc.lastGlyph  = L'Z';
            
            labelFont_ = renderer_->CreateTextureFont(fontDesc);
        }
    }

    /* Setup selector model and view */
    if (!model_)
        model_ = std::make_shared<SelectorModel>();
    if (!view_)
        view_ = std::make_shared<SelectorView>(sceneManager, GetModel(), labelFont_.get());

    /* Add mouse event handler */
    mouseEventHandler_ = std::make_shared<MouseEventHandler>(this);
    IO::Mouse::Instance()->AddEventHandler(mouseEventHandler_);
}
Selector::~Selector()
{
    /* Remove event handlers */
    IO::Mouse::Instance()->RemoveEventHandler(mouseEventHandler_);
}

void Selector::SetupView(const Scene::CameraNode& view)
{
    GetModel()->SetupView(view);
}

void Selector::Draw(const SelectorView::DrawFlags::DataType flags)
{
    /* Update axis visibility */
    GetModel()->UpdateAxisVisibility();

    /* Draw selector at current transformation */
    GetView()->Draw(GetRenderer(), flags);

    if (GetModel()->IsActive() && (flags & SelectorView::DrawFlags::DrawPrevTransform) != 0)
    {
        /* Draw selector at previous transformation */
        Math::Vector3<SelectorModel::AxisState> axesStates;
        
        for (size_t i = 0; i < 3; ++i)
            axesStates[i].isActive = GetModel()->GetAxesState()[i].isActive;

        GetView()->Draw(GetRenderer(), GetModel()->GetPrevTransform(), axesStates, flags);
    }
}

void Selector::ProcessAbsoluteTransform(const Math::Point2f& pointerPosition)
{
    if (model_->isEnabled && model_->IsActive())
    {
        if ( model_->IsPlaneSelection() || ( model_->IsAxisSelection() && model_->IsPickingActive() ) )
            ProcessTransformAxisPicked(pointerPosition);
    }
}

void Selector::ProcessRelativeTransform(const Math::Vector2f& pointerMotion)
{
    if (model_->isEnabled && model_->IsActive())
    {
        if (model_->GetSelectionState() == SelectorModel::SelectionStates::Uniform)
            ProcessTransformUniform(pointerMotion);
        else if (model_->IsAxisSelection() && !model_->IsPickingActive())
            ProcessTransformAxis(pointerMotion);
    }
}


/*
 * ======= Protected: =======
 */

void Selector::ExchangeMouseEventHandler(const std::shared_ptr<MouseEventHandler>& eventHandler)
{
    if (eventHandler)
    {
        IO::Mouse::Instance()->RemoveEventHandler(mouseEventHandler_);
        mouseEventHandler_ = eventHandler;
        IO::Mouse::Instance()->AddEventHandler(mouseEventHandler_);
    }
}

Math::Vector3f Selector::ConvertAxisToVector(const SelectorModel::SelectionStates mode) const
{
    switch (mode)
    {
        case SelectorModel::SelectionStates::XAxis:
            return { 1, 0, 0 };
        case SelectorModel::SelectionStates::YAxis:
            return { 0, 1, 0 };
        case SelectorModel::SelectionStates::ZAxis:
            return { 0, 0, 1 };
    }
    return {};
}

Math::Quaternionf Selector::ConvertAxisToQuaternion(const SelectorModel::SelectionStates mode, float angle) const
{
    Math::Matrix3f rotation;

    switch (mode)
    {
        case SelectorModel::SelectionStates::XAxis:
            rotation.RotateX(angle);
            break;
        case SelectorModel::SelectionStates::YAxis:
            rotation.RotateY(angle);
            break;
        case SelectorModel::SelectionStates::ZAxis:
            rotation.RotateZ(angle);
            break;
    }

    return Math::Quaternionf(rotation);
}

float Selector::ArrowMotionSpeedByDirection(const Math::Vector2f& motion, float maxMotionSpeed) const
{
    /* Compute motion speed by the dot product between arrow- and motion direction */
    return Math::Dot(GetModel()->GetArrowDirection(), motion) * maxMotionSpeed;
}

float Selector::ArrowMotionSpeedByInvDirection(const Math::Vector2f& motion, float maxMotionSpeed) const
{
    /* Compute motion speed by the dot product between fliped arrow- and motion direction */
    return Math::Dot(GetModel()->GetArrowDirection().FlipCW(), motion) * maxMotionSpeed;
}

float Selector::ArrowMotionSpeedByDistance() const
{
    static const float distanceFactor = 0.05f;

    float distance = Math::Distance(
        GetModel()->GetCameraMatrix().GetPosition(),
        GetModel()->GetTransform().GetPosition()
    );

    return distance * distanceFactor;
}

void Selector::ProcessTransformAxis(const Math::Vector2f& motion)
{
    /* Setup transformation meta data */
    const auto& transform = model_->GetTransform();
    const auto& origRotation = transform.GetRotation();

    auto axisAlign = ConvertAxisToVector(model_->GetSelectionState());
    auto transformAlign = transform.GetRotation() * axisAlign;

    /* Transform all selected scene nodes */
    switch (model_->transformMode)
    {
        case SelectorModel::TransformModes::Translate:
        {
            auto motionSpeed = ArrowMotionSpeedByDirection(motion) * ArrowMotionSpeedByDistance();
            TranslateSelectedNodes(transformAlign * motionSpeed);
        }
        break;

        case SelectorModel::TransformModes::Rotate:
        {
            auto motionSpeed = ArrowMotionSpeedByInvDirection(motion) * 0.2f;
            RotateSelectedNodes(
                ( origRotation.Inverse() *
                  ConvertAxisToQuaternion(model_->GetSelectionState(), motionSpeed) *
                  origRotation )
            );
        }
        break;

        case SelectorModel::TransformModes::Scale:
        {
            auto motionSpeed = ArrowMotionSpeedByDirection(motion) * ArrowMotionSpeedByDistance();
            ScaleSelectedNodes(axisAlign * motionSpeed);
        }
        break;
    }
}

void Selector::ProcessTransformAxisPicked(const Math::Point2f& position)
{
    switch (model_->transformMode)
    {
        case SelectorModel::TransformModes::Translate:
            TranslateSelectedNodes(model_->PickOffset(position));
            break;

        default:
            break;
    }
}

void Selector::ProcessTransformUniform(const Math::Vector2f& motion)
{
    //...
}

/*
This functions translates all selected scene nodes in a raster (if it is enabled),
otherwise they are translated directly.
*/
void Selector::TranslateSelectedNodes(const Math::Vector3f& translation)
{
    /* Get final translation vector (when raster is enabled) */
    if (model_->raster.IsEnabled())
    {
        model_->raster.vector += { translation.x, translation.y, translation.z };
        TranslateSelectedNodesDirectly(model_->raster.RasterFlushVector());
    }
    else
        TranslateSelectedNodesDirectly(translation);
}

// This function translates all selected scene nodes directly (global and without any raster).
void Selector::TranslateSelectedNodesDirectly(const Math::Vector3f& translation)
{
    /* Translate all selected scene nodes */
    for (auto& node : model_->GetSelectedNodes())
    {
        node->Transform(
            [&](Scene::Transform& transform)
            {
                transform.MoveGlobal(translation);
            },
            false
        );
    }
}

void Selector::RotateSelectedNodes(const Math::Quaternionf& rotation)
{
    if (model_->IsPivotDistributed())
    {
        /* Rotate all selected scene nodes around the pivot point */
        const auto pivotPoint = model_->GetTransform().GetPosition();

        for (auto& node : model_->GetSelectedNodes())
        {
            node->Transform(
                [&](Scene::Transform& transform)
                {
                    transform.Turn(rotation, pivotPoint);
                },
                false
            );
        }
    }
    else
    {
        /* Rotate all selected scene nodes individually */
        for (auto& node : model_->GetSelectedNodes())
        {
            node->Transform(
                [&](Scene::Transform& transform)
                {
                    #if 1
                    //!!! (temporarily used because "Turn" function is not precisely)
                    transform.SetRotation(Math::Quaternionf(rotation.Mat3() * transform.GetRotation().Mat3()));
                    #else
                    transform.Turn(rotation);
                    #endif
                },
                false
            );
        }
    }
}

void Selector::ScaleSelectedNodes(const Math::Vector3f& scale)
{
    if (model_->IsPivotDistributed())
    {
        /* First find largest scale vector */
        Math::Vector3f maxScale(std::numeric_limits<float>::min());

        for (auto& node : model_->GetSelectedNodes())
        {
            auto nodeScale = node->LocalTransform().GetScale();
            maxScale.x = std::max(maxScale.x, nodeScale.x);
            maxScale.y = std::max(maxScale.y, nodeScale.y);
            maxScale.z = std::max(maxScale.z, nodeScale.z);
        }

        /* Rotate all selected scene nodes around the pivot point */
        const auto pivotPoint = model_->GetTransform().GetPosition();

        for (auto& node : model_->GetSelectedNodes())
        {
            /* Get final scale factor */
            auto nodeScale = scale * node->LocalTransform().GetScale() / maxScale;

            /* Scale node around the pivot */
            node->Transform(
                [&](Scene::Transform& transform)
                {
                    transform.Scale(nodeScale, pivotPoint);
                },
                false
            );
        }
    }
    else
    {
        /* Scale all selected scene nodes individually */
        for (auto& node : model_->GetSelectedNodes())
        {
            node->Transform(
                [&](Scene::Transform& transform)
                {
                    transform.Scale(scale);
                },
                false
            );
        }
    }
}


/*
 * ======= Private: =======
 */

void Selector::OnTransformBegin()
{
    for (auto node : GetModel()->GetSelectedNodes())
        node->OnTransformBegin();
}

void Selector::OnTransformEnd()
{
    for (auto node : GetModel()->GetSelectedNodes())
        node->OnTransformEnd();
}


/*
 * MouseEventHandler class
 */

Selector::MouseEventHandler::MouseEventHandler(Selector* selector) :
    selector_{ selector }
{
    ASSERT_POINTER(selector);
}

bool Selector::MouseEventHandler::OnButtonDown(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame)
{
    auto model = selector_->GetModel();

    switch (keyCode)
    {
        case IO::MouseKeyCodes::MouseLeft:
            if (model->AreAnyNodesSelected() && model->IsAnySelected())
            {
                LockFocus();
                selector_->OnTransformBegin();

                if (frame)
                {
                    auto frameOrigin = frame->Position();
                    model->ProcessActivation(mousePos_, true, &frameOrigin);
                }
                else
                    model->ProcessActivation(mousePos_, true);
            }
            break;

        case IO::MouseKeyCodes::MouseRight:
            if (model->IsActive())
            {
                model->CancelActivation();
                model->ProcessSelection(mousePos_);
                UnlockFocus();
            }
            break;
    }

    return true;
}

bool Selector::MouseEventHandler::OnButtonUp(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame)
{
    auto model = selector_->GetModel();

    if (keyCode == IO::MouseKeyCodes::MouseLeft && model->IsActive())
    {
        model->ProcessActivation(mousePos_, false);
        model->ProcessSelection(mousePos_);
        selector_->OnTransformEnd();
        UnlockFocus();
    }

    return true;
}

// Process arrow selection
bool Selector::MouseEventHandler::OnLocalMotion(const Math::Point2i& position, const Platform::Frame* frame)
{
    auto model = selector_->GetModel();

    if (model->AreAnyNodesSelected())
    {
        mousePos_ = position.Cast<float>();
        
        /* Process axis/plane selection */
        model->ProcessSelection(mousePos_);

        selector_->ProcessAbsoluteTransform(mousePos_);
    }

    return true;
}

// Process object movement
bool Selector::MouseEventHandler::OnGlobalMotion(const Math::Vector2i& motion)
{
    selector_->ProcessRelativeTransform(motion.Cast<float>());
    return true;
}

void Selector::MouseEventHandler::OnLostFocus()
{
    selector_->GetModel()->ResetSelection();
}


} // /namespace Utility

} // /namespace Fork



// ========================