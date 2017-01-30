
// ForkENGINE: Hi-Z Map Test
// 13/07/2014

#include <fengine/import.h>
#include <fengine/using.h>
#include <fengine/helper.h>

using namespace Fork;

Video::RenderSystem* renderer = nullptr;
Video::ConstantBuffer* constBuffer = nullptr;
Scene::CameraNode* cameraNode = nullptr;

int main()
{
    FORK_MATCH_SDK_VERSION

    Engine::EngineDevice engine;

    try
    {
        auto renderSystem = engine.CreateRenderSystem();

        renderer = renderSystem.get();

        Video::ContextDescription contextDesc { true, true, 8 };
        contextDesc.rendererProfileDesc.useExtProfile = true;
        contextDesc.rendererProfileDesc.useGLCoreProfile = true;

        const Math::Size2ui screenSize { 800, 600 };

        auto renderContext = renderSystem->CreateRenderContext(
            Video::VideoMode(screenSize), contextDesc, L"Hi-Z Map Test"
        );

        auto rendererSupport = renderContext->QueryRendererSupport();

        Video::ShaderProvider::ShaderPath() = "../../include/" + Video::ShaderProvider::ShaderPath();

        // Get input devices
        auto keyboard = IO::Keyboard::Instance();
        auto mouse = IO::Mouse::Instance();
        
        // Debug log
        class LogEventHandler : public IO::Log::EventHandler
        {
            public:
                void OnPrintLn(const std::string& indent, const std::string& message)
                {
                    file_.WriteLine(IO::Log::GetFullIndent() + message);
                }
                void OnPrint(const std::string& message)
                {
                    subMessageStack_ += message;
                }
                void OnEndLn()
                {
                    file_.WriteLine(IO::Log::GetFullIndent() + subMessageStack_);
                    subMessageStack_.clear();
                }
            private:
                IO::LogFile file_ { "debug.log" };
                std::string subMessageStack_;
        };

        IO::Log::AddEventHandler(std::make_shared<LogEventHandler>());

        // Primitive renderer
        auto primRenderer = engine.QueryPrimitiveRenderer(renderSystem.get());

        auto frame = renderContext->GetFrame();

        // Create scene
        Scene::SimpleSceneRenderer sceneRenderer;

        Scene::CameraNode camera;
        Scene::SceneNode sceneGraph;
        Scene::SceneManager sceneMngr;

        camera.projection.SetViewport({ {}, screenSize.Cast<int>() });
        camera.projection.SetPlanes(0.1f, 100.0f);
        cameraNode = &camera;

        Scene::FreeViewSceneNodeController cameraCtrl;

        auto cubeNode = sceneMngr.CreateGeometryNode(sceneMngr.GenerateCube());
        cubeNode->transform.SetPosition({ 0, 0, 4 });
        sceneGraph.AddChild(cubeNode);

        Scene::LightNode light;
        light.lightSource = std::make_shared<Scene::PointLightSource>();
        sceneRenderer.UpdateLightNodes({ &light });

        // Create shader
        auto shader = renderSystem->GetShaderManager()->LoadShaderComposition(
            { "DepthVertexShader.glsl", "DepthPixelShader.glsl" },
            Video::Simple3DVertex::Format()
        );

        auto vertexConstBuffer = shader->CreateAndAttachConstantBuffer("vertexParam");
        constBuffer = vertexConstBuffer.get();

        class shaderEventHandler : public Video::ShaderComposition::EventHandler
        {
            public:
                void OnUpdateConstantBuffers(const Video::ShaderComposition* shaderComposition, Video::RenderContext* context)
                {
                    struct
                    {
                        Math::Matrix4f wvpMatrix;
                        Math::Matrix4f worldMatrix;
                        Math::Vector4f cameraPos;
                    }
                    bufferData
                    {
                        context->WVPMatrix(),
                        context->GetRenderState().worldMatrix,
                        Math::Vector4f(cameraNode->transform.GetPosition())
                    };
                    renderer->UpdateBuffer(constBuffer, bufferData);
                }
        };

        shader->SetEventHandler(std::make_shared<shaderEventHandler>());

        cubeNode->shaderComposition = shader;

        // Create depth texture and render target
        auto depthTexture = renderSystem->CreateTexture2D(Video::TextureFormats::R32Float, screenSize.Cast<int>());
        renderContext->GenerateMIPMaps(depthTexture.get());

        auto depthRenderTarget = renderSystem->CreateRenderTarget({ depthTexture });

        // Create Hi-Z down sampler
        Video::CustomMIPMapsGenerator mipMapsGenerator;

        // Create sampler state
        Video::SamplerState::Description samplerStateDesc;
        samplerStateDesc.magFilter = Video::SamplerState::Filters::Nearest;
        auto samplerState = renderSystem->CreateSamplerState(samplerStateDesc);
        
        // Main loop
        while (frame->ReceiveEvents() && !keyboard->KeyDown(IO::KeyCodes::KeyEscape))
        {
            // Update input
            if (MOUSE_DOWN(Right))
            {
                cameraCtrl.Process(&camera);
                camera.UpdateView();
            }

            // Render depth texture
            renderContext->SetupViewport(camera.projection.GetViewport());
            renderContext->ChangeState(renderSystem->GetDefaultDepthStencilState());

            renderContext->BindRenderTarget(depthRenderTarget.get());
            {
                renderContext->ClearBuffers();
                sceneRenderer.RenderSceneFromCamera(&sceneGraph, camera);
            }
            renderContext->BindRenderTarget(renderContext->GetMainRenderTarget());

            #if 0
            renderContext->ClearBuffers();
            sceneRenderer.RenderSceneFromCamera(&sceneGraph, camera);
            #endif

            // Generate Hi-Z map
            static bool isMinimize = true;

            if (KEY_HIT(Return))
                isMinimize = !isMinimize;

            mipMapsGenerator.GenerateHiZMaps(
                depthTexture.get(),
                isMinimize ?
                    Video::CustomMIPMapsGenerator::HiZSampleModes::Minimize :
                    Video::CustomMIPMapsGenerator::HiZSampleModes::Maximize
            );

            // Draw scene
            renderContext->SetupViewport(camera.projection.GetViewport());
            renderContext->ClearBuffers();
            {
                // Draw result
                renderContext->BindSamplerState(samplerState.get());

                primRenderer->BeginDrawing2D();
                {
                    primRenderer->Setup2DProjection();
                    primRenderer->DrawImage(depthTexture.get(), {});
                }
                primRenderer->EndDrawing2D();
            }
            renderContext->Present();

            engine.UpdateFrameStates();
        }

        Platform::NetworkInterface::CleanUp();
    }
    catch (const DefaultException& err)
    {
        IO::Log::ClearEventHandlers();
        IO::Log::Error(err);
        IO::Console::Wait();
    }
    catch (const std::exception& err)
    {
        IO::Log::ClearEventHandlers();
        IO::Log::Error(err.what());
        IO::Console::Wait();
    }
    
    return 0;
}


