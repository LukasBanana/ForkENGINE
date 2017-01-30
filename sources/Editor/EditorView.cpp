/*
 * Editor view file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "EditorView.h"
#include "EditorViewSceneRenderer.h"
#include "EditorSelectionSceneRenderer.h"

#include "Frame/Main/MainFrame.h"
#include "Frame/Console/ConsoleFrame.h"
#include "Entity/EntitySelector.h"
#include "Entity/Entity.h"
#include "Project/ProjectFolder.h"

#include "Core/UIExtensions/UIScreenFrame.h"
#include "Core/WxHelper.h"
#include "Core/Devices.h"
#include "Core/StaticLayoutConfig.h"
#include "Core/StringModifier.h"

#include "Video/RenderSystem/RenderContextException.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Utility/RendererProfilerOutput.h"
#include "Scene/Renderer/BoundingBoxSceneRenderer.h"


namespace Fork
{

namespace Editor
{


/*
 * Internal members
 */

using namespace Utility;
using namespace Devices;

static const float farPlaneProjector    = 0.1f;
static const float farPlaneSelector     = 0.2f;


/*
 * EditorView class
 */

EditorView::GlobalConfiguration EditorView::globalConfig;

EditorView::EditorView(wxWindow* parent)
{
    CreateScreenFrame(parent);

    /* Setup initial background color */
    ChangeBackgroundColor(defaultClearColor);

    /* Initialize global states */
    Utility::RendererProfilerOutput::InitInfoTextMaxWidth(defaultFont.get());

    InitCameraSceneNode();

    /*
    Create resources for this editor view:
    projector util, event handlers.
    */
    CreateVisualProjector();
    CreateEventHandlers();
}
EditorView::~EditorView()
{
    IO::Keyboard::Instance()->RemoveEventHandler( keyboardEventHandler_ );
    IO::Mouse   ::Instance()->RemoveEventHandler( mouseEventHandler_    );
}

void EditorView::ProcessUserInteraction()
{
    if (GetScreenFrame()->HasFocus())
    {
        cameraCtrl_.moveFactor = config.moveSpeed * TimeScale() * 2.5f;
        cameraCtrl_.ProcessMovement(GetCamera());
    }
}

void EditorView::Draw()
{
    if (!GetScreenFrame()->IsShown())
        return;

    auto renderContext = GetScreenFrame()->GetRenderContext();

    /*
    Clear buffers
    -> this will also make the render context to the currnet one
    */
    renderContext->ClearBuffers();
    
    /* Setup render states */
    renderContext->ChangeState( editorViewRasterizerState.get()             );
    renderContext->ChangeState( renderSystem->GetDefaultDepthStencilState() );
    renderContext->ChangeState( renderSystem->GetDefaultBlendState()        );

    UpdateProjectionAndView();

    /* Draw the scene and front menus */
    DrawScene();
    DrawSelectionHighlight();

    DrawFrontMenu3D();
    DrawFrontMenu2D();

    /* Show final result on the screen */
    renderContext->Present();
}

void EditorView::ChangeBackgroundColor(const Video::ColorRGBAf& color)
{
    /* Change the background color for the editor view's render context */
    GetScreenFrame()->GetRenderContext()->SetupClearColor(color);
}

void EditorView::ChangeProjectorTransition(unsigned long long duration)
{
    /* Transition duration of editor view's projector */
    projector_->GetModel()->defaultTransitionDuration = duration;
}

void EditorView::MoveView(const Math::Vector3f& direction)
{
    GetCamera()->transform.MoveLocal(direction * ViewMoveSpeed());
}

void EditorView::ZoomView(float direction)
{
    auto projectorModel = projector_->GetModel();

    if (projectorModel->GetProjection().GetOrtho())
    {
        /* Zoom range for orthogonal projections */
        static const float minZoom = 0.01f;
        static const float maxZoom = 100.0f;

        /* Change view size for orthogonal projection */
        auto projection = projectorModel->GetProjection();
        {
            /*
            Use exponential function to smoothly zoom in and out.
            Use natural logarithm to re-compute previous orthogonal size.
            */
            auto size = std::log(projection.GetOrthoSize().width);
            size += direction;
            size = std::exp(size);

            size = Math::Clamp(size, minZoom, maxZoom);

            projection.SetOrthoSize({ size, size});
        }
        projectorModel->CancelTransition();
        projectorModel->SetupProjection(projection);
    }
}

bool EditorView::IsFrameOwner(const Platform::Frame* frame) const
{
    return GetScreenFrame()->GetAbstractFrame() == frame;
}


/*
 * ======= Private: =======
 */

void EditorView::CreateScreenFrame(wxWindow* parent)
{
    /* Create screen frame and render context */
    screenFrame_ = MakeWxObject<UIScreenFrame>(parent, wxPoint(0, 0), wxSize(300, 200));
    screenFrame_->GetAbstractFrame()->userData = this;

    if (!UIScreenFrame::CreateRenderContext(screenFrame_))
    {
        screenFrame_->Destroy();
        throw RenderContextException("Render context creation for screen frame failed");
    }

    screenFrame_->SetResizeCallback([&](const Math::Size2i&){ Draw(); });
}

void EditorView::InitCameraSceneNode()
{
    /* Initialize scene camera location */
    static const float distanceToOrigin = 2.0f;

    camera_.transform.SetPosition({ distanceToOrigin, distanceToOrigin, -distanceToOrigin });
    camera_.transform.LookAt({});

    camera_.projection.SetPlanes(0.1f, 1000.0f);

    cameraCtrl_.SetupRotation(&camera_);
    cameraCtrl_.moveFactor = 0.025f;

    /* Initialize camera light node with global camera light source */
    cameraLight_.lightSource = cameraLightSource;
}

void EditorView::CreateVisualProjector()
{
    /* Create and initialize the visual projector util */
    projector_ = std::make_unique<EditorViewProjector>(*this, cameraCtrl_);

    projector_->GetModel()->SetupViewportDefaultVisual(
        GetCamera()->projection.GetViewport(), 100,
        0.0f, farPlaneProjector,
        farPlaneSelector, 1.0f
    );
    projector_->GetModel()->SetupProjection(GetCamera()->projection);
}

void EditorView::CreateEventHandlers()
{
    keyboardEventHandler_ = std::make_shared<KeyboardEventHandler>(this);
    IO::Keyboard::Instance()->AddEventHandler(keyboardEventHandler_);

    mouseEventHandler_ = std::make_shared<MouseEventHandler>(this);
    IO::Mouse::Instance()->AddEventHandler(mouseEventHandler_);
}

void EditorView::UpdateProjectionAndView()
{
    auto renderContext = GetScreenFrame()->GetRenderContext();

    /*
    Update projector viewport
    -> depending on the render context resolution
    */
    projector_->GetModel()->SetupViewportDefaultVisual(
        { { 0, 0 }, renderContext->GetVideoMode().resolution.Cast<int>() },
        EditorView::globalConfig.projectorVisualSize,
        0.0f, farPlaneProjector,
        farPlaneSelector, 1.0f
    );

    /* Update camera view and projection */
    GetCamera()->projection = projector_->GetModel()->GetProjection();
    GetCamera()->UpdateView();

    renderContext->SetupViewport(
        projector_->GetModel()->GetProjection().GetViewport()
    );
}

void EditorView::DrawScene()
{
    /* Update light sources */
    if (config.toggleCameraLight)
    {
        cameraLight_.transform = GetCamera()->transform;
        simpleSceneRenderer->UpdateLightNodes({ &cameraLight_ });
    }

    /* Draw scene from editor camera view */
    viewSceneRenderer->RenderSceneFromEditorView(*GetCamera(), config);

    auto project = ProjectFolder::Active();
    if (project)
    {
        auto sceneGraph = project->ActiveSceneGraph();
        simpleSceneRenderer->RenderSceneFromCamera(sceneGraph, *GetCamera());
        //boundingBoxSceneRenderer->RenderSceneFromCamera(sceneGraph, *GetCamera());//!!!
    }
}

//TODO -> Optimize this function!!!
void EditorView::DrawSelectionHighlight()
{
    const auto& selectedNodes = entitySelector->GetModel()->GetSelectedNodes();
    if (!selectedNodes.empty())
    {
        std::vector<Scene::DynamicSceneNode*> selectedSceneNodes;

        size_t i = 0;
        for (auto node : selectedNodes)
        {
            auto entity = dynamic_cast<Entity*>(node);
            entity->ForEachComponentWithSceneNode(
                [&](Engine::Component* component, const Scene::DynamicSceneNodePtr& sceneNode)
                {
                    selectedSceneNodes.push_back(sceneNode.get());
                }
            );
        }

        selectionSceneRenderer->RenderSelectionHighlights(selectedSceneNodes, *GetCamera());
    }
}

void EditorView::DrawFrontMenu3D()
{
    if (entitySelector->IsVisible())
        DrawSelector();
    DrawProjector();
}

void EditorView::DrawFrontMenu2D()
{
    primitiveRenderer->BeginDrawing2D();
    {
        if (config.showProfiler)
            DrawProfilerOutput(renderSystem->GetProfilerModel());
    }
    primitiveRenderer->EndDrawing2D();
}

void EditorView::DrawSelector()
{
    auto renderContext = GetScreenFrame()->GetRenderContext();
    
    /* Setup viewport for entity selector */
    auto viewport = GetCamera()->projection.GetViewport();
    {
        viewport.minDepth = farPlaneProjector;
        viewport.maxDepth = farPlaneSelector;
    }
    renderContext->SetupViewport(viewport);

    /* Draw entity selector */
    entitySelector->SetupView(*GetCamera());
    entitySelector->DrawSelector();
}

void EditorView::DrawProjector()
{
    /* Draw and update projector */
    projector_->DrawAndUpdate();
}

void EditorView::DrawProfilerOutput(const Video::RendererProfilerModel& profilerModel)
{
    Utility::RendererProfilerOutput::DrawInfo(
        profilerModel,
        engine->GetGlobalTimer(),
        primitiveRenderer.get(),
        defaultFont.get(),
        StaticLayout::EditorViewLayout::profilerTextBorder
    );
}

void EditorView::PickEntity(const Math::Point2i& mousePos)
{
    static const float pickLength = 1000.0f;

    /* Get picking ray by camera view */
    auto pickingRay = GetCamera()->ViewRay(mousePos.Cast<float>());

    /* Make intersection test with the physics world */
    Physics::World::RayCastIntersection intersect;
    if (physicsWorld->RayCast(pickingRay.ToLine(pickLength), intersect) && intersect.body && intersect.body->userData)
    {
        /* Select picked entity */
        auto entity = reinterpret_cast<Entity*>(intersect.body->userData);
        entitySelector->SelectWithModifier(entity);
    }
    else
        entitySelector->SelectWithModifier(nullptr);
}

float EditorView::ViewMoveSpeed() const
{
    auto projectorModel = projector_->GetModel();

    if (projectorModel->GetProjection().GetOrtho())
    {
        static const float baseMoveSpeed = 0.003f;
        auto size = projectorModel->GetProjection().GetOrthoSize().width;
        return baseMoveSpeed * size;
    }

    return config.moveSpeed;
}


/*
 * KeyboardEventHandler class
 */

EditorView::KeyboardEventHandler::KeyboardEventHandler(EditorView* view) :
    view_{ view }
{
}

static void ChangeSelectorAlignmentOrMode(
    const SelectorModel::TransformModes mode,
    const SelectorModel::OrientationAlignments orientationAlignment,
    const SelectorModel::PivotAlignments pivotAlignment)
{
    if (IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyShift))
        entitySelector->SetupOrientationAlignment(orientationAlignment);
    else if (IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyControl))
        entitySelector->SetupPivotAlignment(pivotAlignment);
    else
        entitySelector->SetupTransformMode(mode);
}

bool EditorView::KeyboardEventHandler::OnKeyDown(const IO::KeyCodes keyCode, const Platform::Frame* frame)
{
    if (!view_->GetScreenFrame()->HasFocus())
        return true;

    auto& config = view_->config;

    switch (keyCode)
    {
        case IO::KeyCodes::KeyF1:
        {
            config.showProfiler = !config.showProfiler;
        }
        break;

        case IO::KeyCodes::KeyF3:
        {
            auto consoleFrame = MainFrame::Instance()->GetConsoleFrame();
            consoleFrame->Show(!consoleFrame->IsShown());
        }
        break;

        case IO::KeyCodes::KeyNumPad1:
        case IO::KeyCodes::Key1:
        {
            ChangeSelectorAlignmentOrMode(
                SelectorModel::TransformModes::Translate,
                SelectorModel::OrientationAlignments::Global,
                SelectorModel::PivotAlignments::MedianPoint
            );
        }
        break;

        case IO::KeyCodes::KeyNumPad2:
        case IO::KeyCodes::Key2:
        {
            ChangeSelectorAlignmentOrMode(
                SelectorModel::TransformModes::Rotate,
                SelectorModel::OrientationAlignments::Local,
                SelectorModel::PivotAlignments::IndividualOrigins
            );
        }
        break;

        case IO::KeyCodes::KeyNumPad3:
        case IO::KeyCodes::Key3:
        {
            ChangeSelectorAlignmentOrMode(
                SelectorModel::TransformModes::Scale,
                SelectorModel::OrientationAlignments::View,
                SelectorModel::PivotAlignments::ActiveElement
            );
        }
        break;

        case IO::KeyCodes::KeyNumPad4:
        case IO::KeyCodes::Key4:
        {
            if (IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyControl))
                entitySelector->SetupPivotAlignment(SelectorModel::PivotAlignments::BoundingBox);
        }
        break;

        case IO::KeyCodes::KeyNumPad5:
        case IO::KeyCodes::Key5:
        {
            if (IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyControl))
                entitySelector->SetupPivotAlignment(SelectorModel::PivotAlignments::Minimum);
        }
        break;

        case IO::KeyCodes::KeyNumPad6:
        case IO::KeyCodes::Key6:
        {
            if (IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyControl))
                entitySelector->SetupPivotAlignment(SelectorModel::PivotAlignments::Maximum);
        }
        break;

        case IO::KeyCodes::KeyNumPad7:
        case IO::KeyCodes::Key7:
        {
            if (IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyControl))
                entitySelector->SetupPivotAlignment(SelectorModel::PivotAlignments::GlobalCursor);
        }
        break;

        case IO::KeyCodes::KeyR:
        {
            entitySelector->SwitchRaster();
        }
        break;

        case IO::KeyCodes::KeyDelete:
        {
            entitySelector->DeleteSelectedEntities();
        }
        break;

        case IO::KeyCodes::KeyX:
        {
            if (IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyControl))
                entitySelector->CutSelectedEntities();
        }
        break;

        case IO::KeyCodes::KeyC:
        {
            if (IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyControl))
                entitySelector->CopySelectedEntities();
        }
        break;

        case IO::KeyCodes::KeyV:
        {
            if (IO::Keyboard::Instance()->KeyDown(IO::KeyCodes::KeyControl))
                entitySelector->PasteSelectedEntities();
        }
        break;

        default:
        break;
    }

    return true;
}


/*
 * MouseEventHandler class
 */

EditorView::MouseEventHandler::MouseEventHandler(EditorView* view) :
    view_{ view }
{
}

bool EditorView::MouseEventHandler::OnButtonDown(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame)
{
    if (view_->IsFrameOwner(frame))
    {
        switch (keyCode)
        {
            case IO::MouseKeyCodes::MouseLeft:
                /* Process picking only if no axis is selected */
                if (entitySelector->GetModel()->GetSelectionState() == Utility::SelectorModel::SelectionStates::None)
                    view_->PickEntity(mousePos_);
                break;
            case IO::MouseKeyCodes::MouseRight:
                lookMode_ = FreeLookModes::FreeLook;
                LockFreeLook();
                break;
            case IO::MouseKeyCodes::MouseMiddle:
                lookMode_ = FreeLookModes::MoveXY;
                LockFreeLook();
                break;
        }
    }
    return true;
}

bool EditorView::MouseEventHandler::OnButtonDoubleClicked(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame)
{
    return OnButtonDown(keyCode, frame);
}

bool EditorView::MouseEventHandler::OnButtonUp(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame)
{
    if (view_->IsFrameOwner(frame))
    {
        switch (keyCode)
        {
            case IO::MouseKeyCodes::MouseMiddle:
            case IO::MouseKeyCodes::MouseRight:
                UnlockFreeLook();
                break;
        }
    }
    return true;
}

bool EditorView::MouseEventHandler::OnWheelMotion(int motion, const Platform::Frame* frame)
{
    if (view_->IsFrameOwner(frame))
        view_->ZoomView(static_cast<float>(-motion) * 0.1f);
    return true;
}

bool EditorView::MouseEventHandler::OnLocalMotion(const Math::Point2i& position, const Platform::Frame* frame)
{
    mousePos_ = position;
    return true;
}

bool EditorView::MouseEventHandler::OnGlobalMotion(const Math::Vector2i& motion)
{
    if (IsFreeLook())
    {
        switch (lookMode_)
        {
            case FreeLookModes::FreeLook:
            {
                view_->cameraCtrl_.ProcessRotation(
                    view_->GetCamera(), motion.Cast<float>() * (Math::deg2rad * 0.5f)
                );
            }
            break;

            case FreeLookModes::MoveXY:
            {
                const auto motionDir = motion.Cast<float>();
                view_->MoveView({ motionDir.x, -motionDir.y, 0 });
            }
            break;
        }
    }
    return true;
}


} // /namespace Editor

} // /namespace Fork



// ========================