/*
 * Engine simple app header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_ENGINE_SIMPLE_APP_H__
#define __FORK_ENGINE_SIMPLE_APP_H__


#include "Engine/App.h"
#include "Engine/Input.h"
#include "Scene/Manager/SceneManager.h"
#include "Scene/Node/CameraNode.h"
#include "Scene/Renderer/SimpleSceneRenderer.h"
#include "Utility/Terminal/Terminal.h"


namespace Fork
{

namespace Engine
{


/*
Simple engine app base class.
This class provides the default objects for a simple game application.
\see App
*/
class FORK_EXPORT SimpleApp : public App
{
    
    public:
        
        SimpleApp(const SimpleApp&) = delete;
        SimpleApp& operator = (const SimpleApp&) = delete;

        SimpleApp();

        /**
        This updates all events of the main frame and calls "App::Update".
        \see App::OnMain
        \see Platform::Frame::ReceiveEvents
        */
        void OnMain() override;

        /**
        Default render callback. Only draw the scene graph with the main scene renderer.
        \see sceneGraph
        \see mainCamera
        */
        void OnRender() override;

        /**
        Default clean up callback.
        \see App::OnCleanUp
        */
        void OnCleanUp() override;

    protected:
        
        friend class FrameEventHandler;

        /* === Structures === */

        /**
        Engine configuration structure with detailed options.
        \remarks Only extended options are stored here.
        Basic engine configurations (such as resolution) can be set
        with its respective parameter in the "InitEngine" function.
        \see InitEngine
        */
        struct FORK_EXPORT EngineConfiguration
        {
            bool            enableDebugDump         = false;
            bool            useNormalMapping        = false;
            std::wstring    renderSystemLibrary     = L"ForkRendererGL";
            std::wstring    physicsSystemLibrary    = L"ForkPhysicsNw";
            std::wstring    soundSystemLibrary      = L"ForkAudioXA2";
        };

        /* === Functions === */

        //! Returns true if the escape key was pressed.
        bool IsQuit() override;

        /**
        This function creates all standard objects for this simple engine application.
        It creates the render system, main render context, primitive renderer, terminal util
        and initializes the camera projection and viewport (with near plane of 0.1 and far plane of 1000.0).
        \see EngineConfiguration
        */
        void InitEngine(
            const std::wstring&         frameTitle      = L"ForkENGINE Application",
            const Math::Size2ui         screenSize      = { 800, 600 },
            bool                        isFullscreen    = false,
            unsigned int                multiSamples    = 8,
            bool                        isResizable     = true,
            const EngineConfiguration&  config          = {}
        );

        /**
        Callback function when a file was droped onto the main frame.
        \see Platform::Frame::EventHandler::OnFileDrop
        */
        virtual void OnFrameFileDrop(const std::wstring& filename, unsigned int index, unsigned int numFiles);
        /**
        Callback function when the main frame was resized.
        This function will update the viewport of the main camera and the render context
        and changes the render context's video mode.
        \see Platform::Frame::EventHandler::OnResize
        \see RenderContext::ChangeVideoMode
        */
        virtual void OnFrameResize(const Math::Size2i& clientAreaSize);

        /**
        Draws the current frame. This is the order of events omitted by this function:
        - Clear buffes of render context
        - Call "OnRender"
        - Draw terminal overlay (if enabled)
        - Present final frame onto the screen
        \remarks This function will be called automatically in the "OnMain" function and when the frame is resized.
        */
        virtual void OnDrawCurrentFrame();

        /**
        Sets the default render states. This is equivalent to the following code:
        \code
        renderContext->ChangeState(renderSystem->GetDefaultBlendState());
        renderContext->ChangeState(renderSystem->GetDefaultDepthStencilState());
        renderContext->ChangeState(renderSystem->GetDefaultRasterizerState());
        \endcode
        \see Video::RenderContext::ChangeState(const Video::BlendState*)
        \see Video::RenderContext::ChangeState(const Video::DepthStencilState*)
        \see Video::RenderContext::ChangeState(const Video::RasterizerState*)
        \see Video::RenderSystem::GetDefaultBlendState
        \see Video::RenderSystem::GetDefaultDepthStencilState
        \see Video::RenderSystem::GetDefaultRasterizerState
        */
        void SetupDefaultRenderStates();

        /**
        Adds the following extended commands to the terminal utility:
        - scene.hierarchy
        \remarks This is automatically called from the "InitEngine" function.
        \see InitEngine
        */
        void AddExtendedTerminalCommands();

        //! Returns a raw pointer to the terminal util.
        inline Utility::Terminal* GetTerminal() const
        {
            return terminal_.get();
        }
        //! Returns a raw pointer to the scene renderer.
        inline Scene::SimpleSceneRenderer* GetSceneRenderer() const
        {
            return sceneRenderer_.get();
        }

        /* === Members === */

        Video::RenderSystemPtr      renderSystem;
        Video::RenderContextPtr     renderContext;
        Video::PrimitiveRendererPtr primitiveRenderer;

        Physics::PhysicsSystemPtr   physicsSystem;
        Physics::WorldPtr           physicsWorld;

        Audio::SoundSystemPtr       soundSystem;

        Platform::Frame*            frame               = nullptr;

        IO::Keyboard* const         keyboard;
        IO::Mouse* const            mouse;

        Scene::SceneManager         sceneManager;
        Scene::SceneNode            sceneGraph;

        Scene::CameraNode           mainCamera;

        //! Device for input simplification when several and different key bindings are used.
        Input                       input;

    private:

        void DrawTerminalOverlay();

        std::unique_ptr<Utility::Terminal>                  terminal_;
        std::unique_ptr<Scene::SimpleSceneRenderer>         sceneRenderer_;

        std::shared_ptr<Utility::Terminal::LogEventHandler> terminalLogEventHandler_;

};


} // /namespace Engine

} // /namespace Fork


#endif



// ========================