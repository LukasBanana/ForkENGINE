/*
 * Engine app header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_ENGINE_APP_H__
#define __FORK_ENGINE_APP_H__


#include "Core/Export.h"
#include "Engine/EngineDevice.h"


namespace Fork
{

namespace Engine
{


/**
Engine application interface. This can optionally be used as the program's object-oriented main entry point.
\code
// Main class for our application
class MyApp : public Engine::App
{
    public:
        void OnUpdate()
        {
            // Update user input here ...
        }
        void OnRender()
        {
            // Render scene here ...
        }
};

// This implements the "main" function
FORK_IMPLEMENT_APP(MyApp);
\endcode
*/
class FORK_EXPORT App
{
    
    public:
        
        App(const App&) = delete;
        App& operator = (const App&) = delete;

        virtual ~App();

        /**
        Primary initialization function.
        This is the first called function when the main entry point is entered.
        This function calls the secondary initialization function and
        adds the default log event handler (IO::Log::AddDefaultEventHandler).
        \param[in] numArgs Specifies the number of program arguments.
        \param[in] args Specifies the program argument strings.
        \see IO::Log::AddDefaultEventHandler
        */
        virtual void OnInit(int numArgs, char** args);

        /**
        Secondary initialization function.
        \see Init(int, char**)
        */
        virtual void OnInit();

        /**
        This will call "Update", "Render" and "EngineDevice::UpdateFrameStates" in a loop.
        \see Update
        \see Render
        \see EngineDevice::UpdateFrameStates
        */
        virtual void OnMain();

        /**
        Updates the current frame (e.g. for user input). This is called from the "Main" function.
        \see Main
        */
        virtual void OnUpdate();

        /**
        Renders the current frame. This is called from the "Main" function.
        \see Main
        */
        virtual void OnRender();

        //! Cleans up the entire application.
        virtual void OnCleanUp();

        //! Prints the specified unhandled exception to the standard output.
        virtual void OnUnhandledException(const DefaultException& err);
        //! Prints the specified unhandled exception to the standard output.
        virtual void OnUnhandledException(const std::exception& err);

        //! Last call after clean-up or unhandled exceptions.
        virtual void OnExit();

        //! Runs the specified engine application. This is used by the "FORK_IMPLEMENT_APP" helper macro.
        static int RunApp(App* app, int numArgs, char** args);

        //! Returns the engine instance.
        static App* Instance();

    protected:
        
        App();

        //! Returns true if the application is about to quit. By default always true.
        virtual bool IsQuit();

        //! Main engine device.
        EngineDevice engine;

};


/**
Implements the main function.
\param[in] appName Specifies the application class name. This class must inherit from "App".
\see Fork::Engine::App
\remarks Example:
\code
class MyApp : Fork::Engine::App
{
    public:
        void OnUpdate() override
        {
            // update user interaction here ...
        }
        void OnRender() override
        {
            // render scene here ...
        }
};
FORK_IMPLEMENT_APP(MyApp);
\endcode
*/
#define FORK_IMPLEMENT_APP(appName)                                 \
    int main(int numArgs, char** args)                              \
    {                                                               \
        FORK_MATCH_SDK_VERSION                                      \
        auto app = std::make_unique<appName>();                     \
        return Fork::Engine::App::RunApp(app.get(), numArgs, args); \
    }

/**
Implements the main function and the application class with the "OnMain" function declaration only.
\see Fork::Engine::App::OnMain
\remarks Example:
\code
FORK_IMPLEMENT_APP_MAIN_ONLY(MyApp)
void MyApp::OnMain()
{
    // implement your main function here ...
}
\endcode
*/
#define FORK_IMPLEMENT_APP_MAIN_ONLY(appName)   \
    class appName : public Fork::Engine::App    \
    {                                           \
        public:                                 \
            void OnMain() override;             \
    };                                          \
    FORK_IMPLEMENT_APP(appName);


} // /namespace Engine

} // /namespace Fork


#endif



// ========================