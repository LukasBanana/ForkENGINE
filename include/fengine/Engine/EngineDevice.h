/*
 * Engine device header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_ENGINE_DEVICE_H__
#define __FORK_ENGINE_DEVICE_H__


#include "Core/Export.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Video/AdvancedRenderer/PrimitiveRenderer.h"
#include "Physics/PhysicsSystem.h"
#include "Script/ScriptInterpreter.h"
#include "Audio/SoundSystem/SoundSystem.h"
#include "Platform/Core/DynamicLibrary.h"
#include "Platform/Core/Timer.h"

#include "IO/Core/ScopedTimer.h"
#include "IO/Core/ScopedLogTimer.h"
#include "IO/Core/ScopedStringTimer.h"

#include <vector>


namespace Fork
{

namespace Engine
{


/**
Engine main device. This holds all sub-devices (e.g. render system, physics simulator, etc.).
\note This device is not required to develop applications with this engine. It's just a comfort class,
which holds references to all sub-devices.
*/
class FORK_EXPORT EngineDevice
{
    
    public:
        
        EngineDevice();

        /* --- Render System --- */

        /**
        Creates a render system from the specified dynamic library.
        \param[in] libraryName Specifies the library name from which the render system is to be created.
        The ForkENGINE supports OpenGL and Direct3D 11 render systems, thus there are two libraries:
        "ForkRendererGL" and "ForkRendererD3D11". This is used for modular programming.
        If you don't want dependencies to a renderer (e.g. Direct3D), just remove the respective library.
        If you write a custom render system, which implements the whole "Video::RenderSystem" interface,
        you can create this render system using this function, too. The file extension for the dynamic library
        (On MS/Windows "*.dll", on GNU/Linux "*.so") will be added automatically. If you want to create
        a render system without the engine device, use the following code snippet:
        \code
        // When "library" is deleted, the render system can no longer be used, since the dynamic library is no longer available!
        auto library = Platform::DynamicLibrary::Open(libraryName);
        auto renderSystem = Video::RenderSystem::Create(library);
        \endcode
        The default render system library is "ForkRendererGL".
        \param[in] createProfilerSystem Specifies whether a profiler- or a standard render system is to be created. By default false.
        \return Shared pointer to the new render system.
        \throws DynamicLibraryOpenException If opening the library (specified by 'libraryName') failed.
        \see Video::RenderSystem::Create
        \see Platform::DynamicLibrary::Open
        */
        Video::RenderSystemPtr CreateRenderSystem(
            const std::wstring& libraryName = L"ForkRendererGL", bool createProfilerSystem = false
        );

        /**
        Creates a render system with the first suitable library specified in the 'libraryNames' list.
        \param[in] libraryNames Specifies a list of libarary names to find a suitable render system.
        \throws DynamicLibraryOpenException If none of the specified libraries could be opened.
        \throws InvalidArgumentException If the list 'libraryNames' is empty.
        \see CreateRenderSystem(const std::wstring&, bool)
        */
        Video::RenderSystemPtr CreateRenderSystemFromList(
            const std::vector<std::wstring>& libraryNames, bool createProfilerSystem = false
        );

        /**
        Release the specified render system and sets the passed reference to null.
        All render contexts and other resources, attached to this render system, will be released, too.
        */
        void ReleaseRenderSystem(Video::RenderSystemPtr& renderSystem);

        /**
        Creates the primitive renderer for the specified render system.
        Only one instance for each render system can be created.
        \param[in] renderSystem Raw-pointer to the render system. This must have been created with this engine device.
        \return Shared-pointer to the primitive renderer object or null if no such render system has been created with this engine device.
        \see Video::PrimitiveRenderer
        */
        Video::PrimitiveRendererPtr QueryPrimitiveRenderer(Video::RenderSystem* renderSystem);

        /* --- Sound System --- */

        /**
        Creates a sound system from the specified dynamic library.
        \param[in] libraryName Specifies the library name from which the physics system is to be created.
        The ForkENGINE supports "OpenAL" and "XAudio2" sound systems, thus there are two libraries:
        "ForkAudioAL" and "ForkAudioXA2". This is used for modular programming.
        If you write a custom sound system, which implements the whole "Audio::SoundSystem" interface,
        you can create this sound system using this function, too. The file extension for the dynamic library
        (On MS/Windows "*.dll", on GNU/Linux "*.so") will be added automatically. If you want to create
        a sound system without the engine device, use the following code snippet:
        \code
        // When "library" is deleted, the physics system can no longer be used, since the dynamic library is no longer available!
        auto library = Platform::DynamicLibrary::Open(libraryName);
        auto soundSystem = Audio::SoundSystem::Create(library);
        \endcode
        \return Shared pointer to the new sound system.
        \see Audio::SoundSystem::Create
        \see Platform::DynamicLibrary::Open
        */
        Audio::SoundSystemPtr CreateSoundSystem(const std::wstring& libraryName = L"ForkAudioAL");

        /**
        Release the specified sound system and sets the passed reference to null.
        All sound objects, attached to this sound system, will be released, too.
        */
        void ReleaseSoundSystem(Audio::SoundSystemPtr& soundSystem);

        /* --- Physics System --- */

        /**
        Creates a physics system from the specified dynamic library.
        \param[in] libraryName Specifies the library name from which the physics system is to be created.
        The ForkENGINE supports "Newton Game Dynamics" and "NVIDIA PhysX" physics systems, thus there are two libraries:
        "ForkPhysicsNw" and "ForkPhysicsPx". This is used for modular programming.
        If you don't want dependencies to a physics engine (e.g. NVIDIA PhysX), just remove the respective library.
        If you write a custom physics system, which implements the whole "Physics::PhysicsSystem" interface,
        you can create this physics system using this function, too. The file extension for the dynamic library
        (On MS/Windows "*.dll", on GNU/Linux "*.so") will be added automatically. If you want to create
        a physics system without the engine device, use the following code snippet:
        \code
        // When "library" is deleted, the physics system can no longer be used, since the dynamic library is no longer available!
        auto library = Platform::DynamicLibrary::Open(libraryName);
        auto physicsSystem = Physics::PhysicsSystem::Create(library);
        \endcode
        \return Shared pointer to the new physics system.
        \see Physics::PhysicsSystem::Create
        \see Platform::DynamicLibrary::Open
        */
        Physics::PhysicsSystemPtr CreatePhysicsSystem(const std::wstring& libraryName);

        /**
        Release the specified physics system and sets the passed reference to null.
        All physics objects (such as rigid- and soft bodies), attached to this physics system, will be released, too.
        */
        void ReleasePhysicsSystem(Physics::PhysicsSystemPtr& physicsSystem);

        /* --- Scripting --- */

        /**
        Creates a script interpreter from the specified dynamic library.
        \param[in] libraryName Specifies the library name from which the physics system is to be created.
        The ForkENGINE supports "Python" script ("ForkScriptPy" library). This is used for modular programming.
        If you don't want dependencies to a script interprter (e.g. Python), just remove the respective library.
        If you write a custom script interpreter, which implements the whole "Script::ScriptInterpreter" interface,
        you can create this script interpreter using this function, too. The file extension for the dynamic library
        (On MS/Windows "*.dll", on GNU/Linux "*.so") will be added automatically. If you want to create
        a script interpreter without the engine device, use the following code snippet:
        \code
        // When "library" is deleted, the script interpreter can no longer be used, since the dynamic library is no longer available!
        Script::ScriptInterpreter::EnvironmentDescription environmentDesc;
        // environmentDesc ...
        auto library = Platform::DynamicLibrary::Open(libraryName);
        auto scriptInterpreter = Script::ScriptInterpreter::Create(library, environmentDesc);
        \endcode
        \param[in] environmentDesc Specifies the scripting environment description (seee 'Script::ScriptInterpreter::Create').
        \return Shared pointer to the new script interpreter.
        \see Script::ScriptInterpreter::Create
        \see Platform::DynamicLibrary::Open
        */
        Script::ScriptInterpreterPtr CreateScriptInterpreter(
            const std::wstring& libraryName,
            const Script::ScriptInterpreter::EnvironmentDescription& environmentDesc
        );

        /**
        Release the specified script interpreter and sets the passed reference to null.
        All scripting objects, attached to this script interpreter, will be released, too.
        */
        void ReleaseScriptInterpreter(Script::ScriptInterpreterPtr& scriptInterpreter);

        /* --- Misc --- */

        /**
        Updates the states for the current frame.
        \remarks This should be called once at the end of each frame.
        It restes the states of all input devices and measures the frame time of the global timer.
        \see IO::Keyboard::ResetStates
        \see IO::Mouse::ResetStates
        \see Platform::Timer::MeasureFrame
        \see GetGlobalTimer
        */
        void UpdateFrameStates();

        /**
        Makes a new scoped timer. This is a comfort function to easily measure time sections.
        \code
        // Usage example:
        double timerValue = 0;
        {
            auto timer = engine.MakeScopedTimer(timerValue);
            // Do something time intensive here ...
        }
        std::cout << "Elapsed Time: " << timerValue;
        \endcode
        \see IO::ScopedTimer
        \see Platform::Timer
        \see GetTemporaryTimer
        */
        inline IO::ScopedTimer MakeScopedTimer(double& value)
        {
            return { *GetTemporaryTimer(), value };
        }
        /**
        Makes a new scoped log timer. This is a comfort function to easily measure time sections and immediately output the result.
        \code
        // Usage example:
        {
            auto timer = engine.MakeScopedLogTimer();
            // Do something time intensive here ...
            // At the end of this scope, the duration is printed to the log
        }
        \endcode
        \see IO::ScopedLogTimer
        \see Platform::Timer
        \see GetTemporaryTimer
        */
        inline IO::ScopedLogTimer MakeScopedLogTimer(const std::string& info = "Duration: ")
        {
            return { *GetTemporaryTimer(), info };
        }
        /**
        Makes a new scoped string timer. This is a comfort function to easily measure time sections and convert it to a string.
        \code
        // Usage example:
        std::string timerValue;
        {
            auto timer = engine.MakeScopedStringTimer(timerValue);
            // Do something time intensive here ...
        }
        std::cout << "Elapsed Time: " << timerValue << "ms" << std::endl;
        \endcode
        \see IO::ScopedStringTimer
        \see Platform::Timer
        \see GetTemporaryTimer
        */
        inline IO::ScopedStringTimer MakeScopedStringTimer(
            std::string& value, const IO::ScopedStringTimer::Units unit = IO::ScopedStringTimer::Units::MilliSecs)
        {
            return { *GetTemporaryTimer(), value, unit };
        }

        /**
        Returns the global timer object. This timer can be used to query the current frame rate.
        \note Don't call 'ElapsedTime' or 'MeasureFrame' on this object.
        Otherwise the frame measurement of the engine device will be wrong.
        Use "GetTemporaryTimer" for this purpose instead.
        \see GetTemporaryTimer
        \see Platform::Timer
        \see UpdateFrameStates
        */
        inline Platform::Timer* GetGlobalTimer() const
        {
            return globalTimer_.get();
        }

        /**
        Returns the temporary timer object. This timer can be used for temporary time measurements.
        \see MakeLogScopedTimer
        \see MakeScopedTimer
        */
        inline Platform::Timer* GetTemporaryTimer() const
        {
            return temporaryTimer_.get();
        }

    private:
        
        struct RenderSystemLibrary
        {
            Platform::DynamicLibraryPtr library;
            Video::RenderSystemPtr      renderSystem;
            Video::PrimitiveRendererPtr primRenderer;
        };

        struct SoundSystemLibrary
        {
            Platform::DynamicLibraryPtr library;
            Audio::SoundSystemPtr       soundSystem;
        };

        struct PhysicsSystemLibrary
        {
            Platform::DynamicLibraryPtr library;
            Physics::PhysicsSystemPtr   physicsSystem;
        };

        struct ScriptInterpreterLibrary
        {
            Platform::DynamicLibraryPtr     library;
            Script::ScriptInterpreterPtr    scriptInterpreter;
        };

        std::map<const Video::RenderSystem*, RenderSystemLibrary>               renderSystemLibraries_;
        std::map<const Audio::SoundSystem*, SoundSystemLibrary>                 soundSystemLibraries_;
        std::map<const Physics::PhysicsSystem*, PhysicsSystemLibrary>           physicsSystemLibraries_;
        std::map<const Script::ScriptInterpreter*, ScriptInterpreterLibrary>    scriptInterpreterLibraries_;

        Platform::TimerPtr globalTimer_;
        Platform::TimerPtr temporaryTimer_;

};


} // /namespace Engine

} // /namespace Fork


#endif



// ========================