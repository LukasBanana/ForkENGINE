/*
 * Editor devices file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Devices.h"
#include "Core/Exception/NullPointerException.h"
#include "Video/RenderSystem/RenderSystemException.h"
#include "Scene/LightSource/PointLightSource.h"
#include "Core/Exception/DefaultException.h"
#include "Core/Exception/NullPointerException.h"
#include "DefaultMessageBoxes.h"
#include "../EditorViewSceneRenderer.h"
#include "../EditorSelectionSceneRenderer.h"
#include "../Entity/EntitySelector.h"
#include "../Project/ProjectFolder.h"
#include "../Project/ProjectFactory.h"
#include "Config.h"
#include "IO/Core/Log.h"
#include "Engine/EngineInfo.h"


namespace Fork
{

namespace Editor
{

namespace Devices
{


/* --- Global members --- */

std::unique_ptr<Engine::EngineDevice>               engine;

Video::RenderSystemPtr                              renderSystem;
Physics::PhysicsSystemPtr                           physicsSystem;
Audio::SoundSystemPtr                               soundSystem;
Script::ScriptInterpreterPtr                        scriptInterpreter;

Video::PrimitiveRendererPtr                         primitiveRenderer;
Video::TextureFontPtr                               defaultFont;

std::unique_ptr<Scene::SceneManager>                sceneManager;
Physics::WorldPtr                                   physicsWorld;

std::unique_ptr<EditorViewSceneRenderer>            viewSceneRenderer;
std::unique_ptr<EditorSelectionSceneRenderer>       selectionSceneRenderer;
std::unique_ptr<Scene::SimpleSceneRenderer>         simpleSceneRenderer;
std::unique_ptr<Scene::BoundingBoxSceneRenderer>    boundingBoxSceneRenderer;
std::unique_ptr<Video::CustomMIPMapsGenerator>      customMIPMapsGenerator;
std::unique_ptr<EntitySelector>                     entitySelector;

Video::RasterizerStatePtr                           editorViewRasterizerState;
Scene::LightSourcePtr                               cameraLightSource;

ErrorReporter                                       errorReporter;


/* --- Internal functions --- */

static void CreateCommonDevicesOnce()
{
    /* Check if these common devices have already been created */
    static bool wasAlreadyCreated = false;

    if (wasAlreadyCreated)
        return;

    wasAlreadyCreated = true;

    /* Print engine information first */
    Engine::PrintEngineInfo(Video::RenderContext::Active());

    /* Create common primitive renderer */
    primitiveRenderer = engine->QueryPrimitiveRenderer(Video::RenderSystem::Active());

    /* Create font for profiler */
    defaultFont = primitiveRenderer->CreateTextureFont({ "Calibri", 20 });

    /* Create scene renderers */
    viewSceneRenderer           = std::make_unique< EditorViewSceneRenderer         >();
    selectionSceneRenderer      = std::make_unique< EditorSelectionSceneRenderer    >();
    simpleSceneRenderer         = std::make_unique< Scene::SimpleSceneRenderer      >(true);
    boundingBoxSceneRenderer    = std::make_unique< Scene::BoundingBoxSceneRenderer >(sceneManager.get());

    /* Create advanced renderers */
    customMIPMapsGenerator = std::make_unique<Video::CustomMIPMapsGenerator>();

    /* Create utils */
    entitySelector = std::make_unique<EntitySelector>();

    /* Create render states */
    Video::RasterizerState::Description rasterizerStateDesc;
    {
        rasterizerStateDesc.isDepthClipEnabled      = false;
        rasterizerStateDesc.isMultiSampleEnable     = true;
        rasterizerStateDesc.isAntiAliasedLineEnable = true;
    }
    editorViewRasterizerState = renderSystem->CreateRasterizerState(rasterizerStateDesc);

    /* Create editor's camera light source */
    cameraLightSource = std::make_shared<Scene::PointLightSource>();
}

static void CreateRenderSystem()
{
    try
    {
        /* Create profiler render system */
        renderSystem = engine->CreateRenderSystem(L"ForkRendererGL", true);
    }
    catch (const DefaultException& err)
    {
        FatalError(err.FullMessage());
    }
}

static void CreatePhysicsSystem()
{
    try
    {
        /* Create physics system and world */
        physicsSystem = engine->CreatePhysicsSystem(L"ForkPhysicsNw");
        physicsWorld = physicsSystem->CreateWorld();
    }
    catch (const DefaultException& err)
    {
        FatalError(err.FullMessage());
    }
}

static void CreateSoundSystem()
{
    try
    {
        /* Create sound system */
        soundSystem = engine->CreateSoundSystem(L"ForkAudioXA2");
    }
    catch (const DefaultException& err)
    {
        IO::Log::Error(err);
    }
}

static void CreateScriptInterpreter()
{
    try
    {
        /* Create "Python" script interpreter */
        Script::ScriptInterpreter::EnvironmentDescription envDesc;
        {
            envDesc.scriptEnginePath = L"Python-3.4.2rc1";
            envDesc.scriptModulePath = "scripts/python";
        }
        scriptInterpreter = engine->CreateScriptInterpreter(L"ForkScriptPy", envDesc);
    }
    catch (const DefaultException& err)
    {
        IO::Log::Error(err);
    }
}


/* --- Global functions --- */

void CreateEngine()
{
    if (!engine)
    {
        /* Create engine device */
        engine = std::make_unique<Engine::EngineDevice>();

        /* Create scene manager and main scene graph */
        sceneManager = std::make_unique<Scene::SceneManager>();

        /* Create sub systems */
        CreateRenderSystem();
        CreatePhysicsSystem();
        CreateSoundSystem();
        CreateScriptInterpreter();
    }
}

Video::RenderContextPtr CreateRenderContext(wxWindow* parentFrame)
{
    ASSERT_POINTER(parentFrame);

    /* Setup render context configuration */
    auto nativeHandle = parentFrame->GetHWND();

    const auto winSize = parentFrame->GetSize();

    const Math::Size2i frameSize { winSize.GetWidth(), winSize.GetHeight() };
    Video::VideoMode videoMode { frameSize.Cast<unsigned int>() };
    
    auto contextDesc = Config::Instance()->renderContextDesc;

    /*
    Only use GL core profile when all contexts shared the same GL hardware render context.
    Otherwise GL core profile can not be used, because "container objects"
    (such as "vertex attrib objects") are not shared between multiple render contexts.
    */
    //contextDesc.rendererProfileDesc.enableDebugDump     = true;
    contextDesc.rendererProfileDesc.useExtProfile       = true;
    contextDesc.rendererProfileDesc.useGLCoreProfile    = true;

    try
    {
        /* Create render context */
        auto renderContext = Video::RenderSystem::Active()->CreateRenderContext(
            videoMode, contextDesc, L"Render View", &nativeHandle
        );

        /* Create common devices */
        CreateCommonDevicesOnce();

        return renderContext;
    }
    catch (const DefaultException& err)
    {
        FatalError(err.FullMessage());
    }

    return nullptr;
}

void CleanUpScene()
{
    renderSystem->GetSharedRenderContext()->Activate();
    ProjectFactory::Instance()->RemoveAllProjectFolders();
}

void CleanUpEngine()
{
    renderSystem->GetSharedRenderContext()->Activate();

    /* Release all devices */
    entitySelector              .reset();

    editorViewRasterizerState   .reset();
    cameraLightSource           .reset();

    customMIPMapsGenerator      .reset();

    viewSceneRenderer           .reset();
    selectionSceneRenderer      .reset();
    simpleSceneRenderer         .reset();
    boundingBoxSceneRenderer    .reset();

    physicsWorld                .reset();
    sceneManager                .reset();
    
    primitiveRenderer           .reset();
    defaultFont                 .reset();

    scriptInterpreter           .reset();
    soundSystem                 .reset();
    physicsSystem               .reset();
    renderSystem                .reset();

    engine                      .reset();

    /* Print final log output */
    IO::Log::Blank();
    IO::Log::Message("ForkEd has terminated successful");
}

float TimeScale()
{
    static const double defaultFPS = 60.0;
    return static_cast<float>(engine->GetGlobalTimer()->GetDeltaTime() * defaultFPS);
}


} // /namespace Devices

} // /namespace Editor

} // /namespace Fork



// ========================