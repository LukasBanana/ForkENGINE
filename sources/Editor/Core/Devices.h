/*
 * Editor devices header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_DEVICES_H__
#define __FORK_EDITOR_DEVICES_H__


#include "Engine/EngineDevice.h"
#include "Scene/Node/SceneNode.h"
#include "Scene/LightSource/LightSource.h"
#include "Scene/Manager/SceneManager.h"
#include "Scene/Renderer/SimpleSceneRenderer.h"
#include "Scene/Renderer/BoundingBoxSceneRenderer.h"
#include "Video/AdvancedRenderer/CustomMIPMapsGenerator.h"
#include "ErrorReporter.h"

#include <wx/window.h>


namespace Fork
{

namespace Editor
{

class EditorViewSceneRenderer;
class EntitySelector;
class EditorSelectionSceneRenderer;

namespace Devices
{


/* --- Global members --- */

extern std::unique_ptr<Engine::EngineDevice>            engine;

extern Video::RenderSystemPtr                           renderSystem;
extern Physics::PhysicsSystemPtr                        physicsSystem;
extern Audio::SoundSystemPtr                            soundSystem;
extern Script::ScriptInterpreterPtr                     scriptInterpreter;

extern Video::PrimitiveRendererPtr                      primitiveRenderer;
extern Video::TextureFontPtr                            defaultFont;

extern std::unique_ptr<Scene::SceneManager>             sceneManager;
extern Physics::WorldPtr                                physicsWorld;

extern std::unique_ptr<EditorViewSceneRenderer>         viewSceneRenderer;
extern std::unique_ptr<EditorSelectionSceneRenderer>    selectionSceneRenderer;
extern std::unique_ptr<Scene::SimpleSceneRenderer>      simpleSceneRenderer;
extern std::unique_ptr<Scene::BoundingBoxSceneRenderer> boundingBoxSceneRenderer;
extern std::unique_ptr<Video::CustomMIPMapsGenerator>   customMIPMapsGenerator;
extern std::unique_ptr<EntitySelector>                  entitySelector;

extern Video::RasterizerStatePtr                        editorViewRasterizerState;
extern Scene::LightSourcePtr                            cameraLightSource;

extern ErrorReporter                                    errorReporter;

static const Video::ColorRGBAf                          defaultClearColor { 0.2f };


/* --- Global functions --- */

void CreateEngine();

Video::RenderContextPtr CreateRenderContext(wxWindow* parentFrame);

void CleanUpScene();
void CleanUpEngine();

/**
Returns the time scale relative to 60 Hz,
i.e. with 60 FPS the time scale is 1.0
and with 120 FPS the time scale is 0.5.
*/
float TimeScale();


} // /namespace Devices

} // /namespace Editor

} // /namespace Fork


#endif



// ========================