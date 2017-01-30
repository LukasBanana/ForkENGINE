/*
 * Engine device header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Engine/EngineDevice.h"
#include "IO/InputDevice/Keyboard.h"
#include "IO/InputDevice/Mouse.h"
#include "Platform/Core/DynamicLibraryOpenException.h"
#include "Core/Exception/InvalidArgumentException.h"


namespace Fork
{

namespace Engine
{


EngineDevice::EngineDevice() :
    globalTimer_    { Platform::Timer::Create() },
    temporaryTimer_ { Platform::Timer::Create() }
{
}

/* --- Render System --- */

Video::RenderSystemPtr EngineDevice::CreateRenderSystem(const std::wstring& libraryName, bool createProfilerSystem)
{
    RenderSystemLibrary renderSysLib;
    {
        /* Open render system library and create render system */
        renderSysLib.library        = Platform::DynamicLibrary::Open(libraryName);
        renderSysLib.renderSystem   = Video::RenderSystem::Create(renderSysLib.library, createProfilerSystem);
    }
    renderSystemLibraries_.insert({ renderSysLib.renderSystem.get(), renderSysLib });

    return renderSysLib.renderSystem;
}

Video::RenderSystemPtr EngineDevice::CreateRenderSystemFromList(const std::vector<std::wstring>& libraryNames, bool createProfilerSystem)
{
    if (libraryNames.empty())
        throw InvalidArgumentException(__FUNCTION__, "libraryNames", "Library name list must not be empty");

    auto it = libraryNames.begin();

    while (true)
    {
        try
        {
            /* Create render system with current library name */
            return CreateRenderSystem(*it, createProfilerSystem);
        }
        catch (const DynamicLibraryOpenException& err)
        {
            /* Try next library name */
            ++it;
            if (it == libraryNames.end())
            {
                /* End reached -> pass exception to caller */
                throw err;
            }
        }
    }
}

void EngineDevice::ReleaseRenderSystem(Video::RenderSystemPtr& renderSystem)
{
    auto it = renderSystemLibraries_.find(renderSystem.get());
    if (it != renderSystemLibraries_.end())
    {
        renderSystemLibraries_.erase(it);
        renderSystem = nullptr;
    }
}

Video::PrimitiveRendererPtr EngineDevice::QueryPrimitiveRenderer(Video::RenderSystem* renderSystem)
{
    /* Find render system library */
    auto it = renderSystemLibraries_.find(renderSystem);
    if (it == renderSystemLibraries_.end())
        return nullptr;

    /* Create primitive renderer */
    if (!it->second.primRenderer)
        it->second.primRenderer = std::make_shared<Video::PrimitiveRenderer>(renderSystem);

    return it->second.primRenderer;
}

/* --- Sound System --- */

Audio::SoundSystemPtr EngineDevice::CreateSoundSystem(const std::wstring& libraryName)
{
    SoundSystemLibrary soundSystemLib;
    {
        /* Open sound system library and create sound system */
        soundSystemLib.library      = Platform::DynamicLibrary::Open(libraryName);
        soundSystemLib.soundSystem  = Audio::SoundSystem::Create(soundSystemLib.library);
    }
    soundSystemLibraries_.insert({ soundSystemLib.soundSystem.get(), soundSystemLib });

    return soundSystemLib.soundSystem;
}

void EngineDevice::ReleaseSoundSystem(Audio::SoundSystemPtr& soundSystem)
{
    auto it = soundSystemLibraries_.find(soundSystem.get());
    if (it != soundSystemLibraries_.end())
    {
        soundSystemLibraries_.erase(it);
        soundSystem = nullptr;
    }
}

/* --- Physics System --- */

Physics::PhysicsSystemPtr EngineDevice::CreatePhysicsSystem(const std::wstring& libraryName)
{
    PhysicsSystemLibrary physicsSysLib;
    {
        /* Open physics system library and create physics system */
        physicsSysLib.library       = Platform::DynamicLibrary::Open(libraryName);
        physicsSysLib.physicsSystem = Physics::PhysicsSystem::Create(physicsSysLib.library);
    }
    physicsSystemLibraries_.insert({ physicsSysLib.physicsSystem.get(), physicsSysLib });

    return physicsSysLib.physicsSystem;
}

void EngineDevice::ReleasePhysicsSystem(Physics::PhysicsSystemPtr& physicsSystem)
{
    auto it = physicsSystemLibraries_.find(physicsSystem.get());
    if (it != physicsSystemLibraries_.end())
    {
        physicsSystemLibraries_.erase(it);
        physicsSystem = nullptr;
    }
}

/* --- Scripting --- */

Script::ScriptInterpreterPtr EngineDevice::CreateScriptInterpreter(
    const std::wstring& libraryName, const Script::ScriptInterpreter::EnvironmentDescription& environmentDesc)
{
    ScriptInterpreterLibrary scriptInterpLib;
    {
        /* Open scripting library and create script interpreter */
        scriptInterpLib.library             = Platform::DynamicLibrary::Open(libraryName);
        scriptInterpLib.scriptInterpreter   = Script::ScriptInterpreter::Create(scriptInterpLib.library, environmentDesc);
    }
    scriptInterpreterLibraries_.insert({ scriptInterpLib.scriptInterpreter.get(), scriptInterpLib });

    return scriptInterpLib.scriptInterpreter;
}

void EngineDevice::ReleaseScriptInterpreter(Script::ScriptInterpreterPtr& scriptInterpreter)
{
    auto it = scriptInterpreterLibraries_.find(scriptInterpreter.get());
    if (it != scriptInterpreterLibraries_.end())
    {
        scriptInterpreterLibraries_.erase(it);
        scriptInterpreter = nullptr;
    }
}

/* --- Misc --- */

void EngineDevice::UpdateFrameStates()
{
    IO::Keyboard::Instance()->ResetStates();
    IO::Mouse   ::Instance()->ResetStates();
    GetGlobalTimer()->MeasureFrame();
}


} // /namespace Engine

} // /namespace Fork



// ========================