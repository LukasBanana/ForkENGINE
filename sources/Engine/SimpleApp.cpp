/*
 * Engine simple app file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Engine/SimpleApp.h"
#include "Engine/EngineInfo.h"
#include "Utility/Terminal/StandardCommand.h"
#include "Scene/Renderer/LogSceneRenderer.h"


namespace Fork
{

namespace Engine
{


/*
 * Internal classes
 */

class FrameEventHandler : public Platform::Frame::EventHandler
{

    public:
        
        FrameEventHandler(SimpleApp* simpleApp) :
            simpleApp_{ simpleApp }
        {
        }

        void OnFileDrop(const std::wstring& filename, unsigned int index, unsigned int numFiles) override
        {
            simpleApp_->OnFrameFileDrop(filename, index, numFiles);
        }

        void OnResize(const Math::Size2i& clientAreaSize) override
        {
            simpleApp_->OnFrameResize(clientAreaSize);
        }

    private:

        SimpleApp* simpleApp_ = nullptr;

};


/*
 * SimpleApp class
 */

SimpleApp::SimpleApp() :
    keyboard{ IO::Keyboard::Instance() },
    mouse   { IO::Mouse::Instance()    }
{
}

void SimpleApp::OnMain()
{
    if (!frame || !renderSystem || !renderContext)
        return;

    while (frame->ReceiveEvents() && !IsQuit())
    {
        /* Update user input */
        if (keyboard->KeyHit(IO::KeyCodes::KeyF3))
            terminal_->Enable(!terminal_->IsEnabled());

        /* Update sound system */
        if (soundSystem)
            soundSystem->Update();

        /* Run "OnUpdate" callback */
        if (!terminal_->IsEnabled())
            OnUpdate();

        /* Draw current frame and update frame states */
        OnDrawCurrentFrame();

        engine.UpdateFrameStates();
    }
}

void SimpleApp::OnRender()
{
    /* Render scene graph with main scene renderer */
    mainCamera.UpdateView();
    sceneRenderer_->RenderSceneFromCamera(&sceneGraph, mainCamera);
}

void SimpleApp::OnCleanUp()
{
    IO::Log::RemoveEventHandler(terminalLogEventHandler_);
    terminal_.reset();
    sceneRenderer_.reset();
}


/*
 * ======= Protected: =======
 */

bool SimpleApp::IsQuit()
{
    return keyboard->KeyDown(IO::KeyCodes::KeyEscape);
}

void SimpleApp::InitEngine(
    const std::wstring& frameTitle, const Math::Size2ui screenSize, bool isFullscreen,
    unsigned int multiSamples, bool isResizable, const EngineConfiguration& config)
{
    /* Create render system */
    renderSystem = engine.CreateRenderSystem(config.renderSystemLibrary);

    /* Create render context */
    Video::ContextDescription contextDesc(true, (multiSamples > 1), multiSamples);

    contextDesc.rendererProfileDesc.enableDebugDump     = config.enableDebugDump;
    contextDesc.rendererProfileDesc.useExtProfile       = true;
    contextDesc.rendererProfileDesc.useGLCoreProfile    = true;

    renderContext = renderSystem->CreateRenderContext(
        Video::VideoMode(screenSize, 32, isFullscreen), contextDesc, frameTitle
    );

    /* Setup frame */
    frame = renderContext->GetFrame();
    frame->SetEventHandler(std::make_shared<FrameEventHandler>(this));

    if (isResizable && !isFullscreen)
        frame->ChangeDesc({ true, false, true });

    /* Create primitive renderer */
    primitiveRenderer = engine.QueryPrimitiveRenderer(renderSystem.get());

    /* Create terminal util */
    terminal_ = std::make_unique<Utility::Terminal>(primitiveRenderer);
    terminal_->GetView()->groundColor.a = 0.8f;

    AddExtendedTerminalCommands();

    terminalLogEventHandler_ = std::make_shared<Utility::Terminal::LogEventHandler>(terminal_.get());
    IO::Log::AddEventHandler(terminalLogEventHandler_);

    /* Setup camera projection */
    mainCamera.projection.SetPlanes(0.1f, 1000.0f);
    mainCamera.projection.SetViewport({ {}, screenSize.Cast<int>() });

    renderContext->SetupViewport(mainCamera.projection.GetViewport());

    /* Setup meta data */
    mainCamera.metaData.name = "mainCamera";
    sceneGraph.metaData.name = "sceneGraph";

    /* Print engine information at start-up */
    PrintEngineInfo(renderContext.get());

    /* Create physics system */
    if (!config.physicsSystemLibrary.empty())
    {
        try
        {
            physicsSystem = engine.CreatePhysicsSystem(config.physicsSystemLibrary);
            physicsWorld = physicsSystem->CreateWorld();
        }
        catch (const DynamicLibraryOpenException&)
        {
            IO::Log::Warning("Physics system is not available");
        }
    }

    /* Create sound system */
    if (!config.soundSystemLibrary.empty())
    {
        try
        {
            soundSystem = engine.CreateSoundSystem(config.soundSystemLibrary);
        }
        catch (const DynamicLibraryOpenException&)
        {
            IO::Log::Warning("Sound system is not available");
        }
    }

    /* Create the main scene renderer */
    sceneRenderer_ = std::make_unique<Scene::SimpleSceneRenderer>(config.useNormalMapping);
}

void SimpleApp::OnFrameFileDrop(const std::wstring& filename, unsigned int index, unsigned int numFiles)
{
    // dummy
}

void SimpleApp::OnFrameResize(const Math::Size2i& clientAreaSize)
{
    mainCamera.projection.SetViewport({ {}, clientAreaSize });

    renderContext->ResizeResolution(clientAreaSize.Cast<unsigned int>());
    renderContext->SetupViewport(mainCamera.projection.GetViewport());

    OnDrawCurrentFrame();
}

void SimpleApp::OnDrawCurrentFrame()
{
    renderContext->ClearBuffers();
    {
        SetupDefaultRenderStates();

        OnRender();

        if (terminal_->IsEnabled())
        {
            renderContext->BindRenderTarget(renderContext->GetMainRenderTarget());
            DrawTerminalOverlay();
        }
    }
    renderContext->Present();
}

void SimpleApp::SetupDefaultRenderStates()
{
    renderContext->ChangeState(renderSystem->GetDefaultBlendState());
    renderContext->ChangeState(renderSystem->GetDefaultDepthStencilState());
    renderContext->ChangeState(renderSystem->GetDefaultRasterizerState());
}

void SimpleApp::AddExtendedTerminalCommands()
{
    terminal_->GetCommandFactory()->AddCommand(
        std::make_shared<Utility::StandardCommand>(
            "scene.hierarchy",
            "Prints the node hierarchy of the entire scene graph",
            [&](const IO::CommandLine::ArgumentListType& args, Utility::Terminal* terminal, void* userData) -> bool
            {
                Scene::LogSceneRenderer().RenderScene(&sceneGraph);
                return true;
            }
        )
    );
}


/*
 * ======= Private: =======
 */

void SimpleApp::DrawTerminalOverlay()
{
    primitiveRenderer->BeginDrawing2D();
    {
        terminal_->Draw();
    }
    primitiveRenderer->EndDrawing2D();
}


} // /namespace Engine

} // /namespace Fork



// ========================