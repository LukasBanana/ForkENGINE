
// ForkENGINE: SSCT Test
// 05/10/2014

#include "../TestUtils.h"

using namespace Fork;

//#define BUILD_FOR_STANDALONE
//#define ENABLE_SPECULAR_MAP
//#define SPONZA_SCENE
//#define USE_DYNAMIC_CUBEMAP

Video::RenderSystem* renderer = nullptr;
Video::RenderContext* context = nullptr;

Scene::CameraNode* camera = nullptr;


class MainGBuffer : public Video::GBuffer
{

    public:
        
        MainGBuffer();

        void ShowTextures(Util::Terminal* terminal);

        void Resize(const TextureType::SizeType& size) override;

        inline Video::Texture2D* GetSpecularMap() const
        {
            return specularMap.get();
        }

    private:
        
        Video::Texture2DPtr specularMap;

};

class MainCubeGBuffer : public Video::CubeGBuffer
{
    
    public:
        
        MainCubeGBuffer();

        inline Video::TextureCube* GetSpecularMap() const
        {
            return specularMap.get();
        }

    private:
        
        Video::TextureCubePtr specularMap;

};

class MainSceneRenderer : public Scene::ForwardSceneRenderer
{
    
    public:
        
        MainSceneRenderer();

        void Render(Scene::SceneNode* graphRootSceneNode, const Scene::CameraNode& camera);
        void RenderCubeMap(Scene::SceneNode* sceneGraph, const Math::Point3f& cameraPosition);
        void RenderScene(Scene::SceneNode* graphRootSceneNode) override;

        inline MainGBuffer* GetGBuffer()
        {
            return &gbuffer;
        }
        inline const MainGBuffer* GetGBuffer() const
        {
            return &gbuffer;
        }

        inline MainCubeGBuffer* GetCubeGBuffer()
        {
            return &cubeGbuffer;
        }
        inline const MainCubeGBuffer* GetCubeGBuffer() const
        {
            return &cubeGbuffer;
        }

        inline const Video::ShaderCompositionPtr& GetSceneSkyBoxShader() const
        {
            return sceneSkyBoxShader;
        }

        float surfaceNormalDeviation = 0.0f;
        bool enableRenderTarget = true;

    private:
        
        class SceneShaderEventHandler : public Video::ShaderComposition::EventHandler
        {
            
            public:
                
                SceneShaderEventHandler(Video::ConstantBuffer* vertexParam);

                void OnUpdateConstantBuffers(
                    const Video::ShaderComposition* shaderComposition, Video::RenderContext* renderContext
                ) override;

            private:
                
                Video::ConstantBuffer* vertexParam = nullptr;

        };

        void VisitTexturedGeometry(Scene::TexturedGeometry* node) override;

        void UpdateSurfaceSettings(size_t numTextures, float normalDeviation);

        MainGBuffer gbuffer;
        MainCubeGBuffer cubeGbuffer;
        
        Scene::CubeMapRenderer cubeMapRenderer;

        Video::ShaderCompositionPtr sceneShader;
        Video::ShaderCompositionPtr sceneSkyBoxShader;
        Video::ConstantBufferPtr vertexParam, pixelParam;

};

class SSCTTest : public Engine::SimpleApp
{
    
    public:
        
        void OnInit() override;
        void OnUpdate() override;
        void OnRender() override;

    private:
        
        enum class Effects
        {
            None,
            SSCT,
            SSLR,
        };

        void OnFrameResize(const Math::Size2i& clientAreaSize) override;
        void OnDrawCurrentFrame() override;

        void CreateScene();
        void SetupTestSampler(int mipLodBias);

        void DrawScene();

        void PrintDebugKeyBinding();
        void DrawDebugging();
        
        void SaveSettings();
        void LoadSettings();

        void SaveScreenshot();

        void UpdateFrameTitle();

        std::unique_ptr<MainSceneRenderer> sceneRenderer;

        Scene::FreeViewSceneNodeController cameraCtrl;

        std::unique_ptr<Video::SSCT> ssctProcessor;
        std::unique_ptr<Video::SSLR> sslrProcessor;
        Effects activeEffect = Effects::SSCT;

        Video::QueryPtr timeQuery;

        Video::BlendStatePtr blendStatePostProcessor;
        Video::SamplerStatePtr testSampler;
        int testSamplerLodBias = 0;//3;

        bool isCameraAnim = false;
        Anim::NodeAnimation cameraTrackAnim;

        bool showFrameRates = true;

        #ifdef _DEBUG_SSCT_
        Video::BlendStatePtr blendState;
        #endif

};

FORK_IMPLEMENT_APP(SSCTTest);


// --- GBuffer --- //

MainGBuffer::MainGBuffer() :
    Video::GBuffer
    {
        context->Resolution().Cast<int>(),
        Video::TextureFormats::RGBA8UInt,
        Video::TextureFormats::RGB16Float,
        Video::TextureFormats::RG32Float
    }
{
    specularMap = renderer->CreateTexture2D(Video::TextureFormats::R8UInt, context->Resolution().Cast<int>());

    #ifdef ENABLE_SPECULAR_MAP
    // Recreate render target
    renderTarget->Clear();
    renderTarget = renderer->CreateRenderTarget({ colorMap, normalMap, depthMap, specularMap });
    #endif
}

void MainGBuffer::ShowTextures(Util::Terminal* terminal)
{
    terminal->Message("GBuffer Color Map:");
    terminal->Print(colorMap);

    terminal->Message("GBuffer Normal Map:");
    terminal->Print(normalMap);

    terminal->Message("GBuffer Depth Map:");
    terminal->Print(depthMap);
}

void MainGBuffer::Resize(const TextureType::SizeType& size)
{
    Video::GBuffer::Resize(size);

    renderer->WriteTexture(specularMap.get(), specularMap->GetFormat(), size);

    //renderTarget->Attach(specularMap, 3);
}


// --- CubeGBuffer --- //

MainCubeGBuffer::MainCubeGBuffer() :
    Video::CubeGBuffer
    {
        //context->Resolution().Cast<int>(),
        { 512, 512 },
        Video::TextureFormats::RGBA8UInt,
        Video::TextureFormats::RGB16Float,
        Video::TextureFormats::RG32Float
    }
{
    specularMap = renderer->CreateTextureCube(Video::TextureFormats::R8UInt, { 1, 1 });
}


// --- MainSceneRenderer --- //

MainSceneRenderer::MainSceneRenderer()
{
    // Create main scene shader
    #ifdef BUILD_FOR_STANDALONE
    const std::string shaderPath = "shaders/";
    #else
    const std::string shaderPath = "";
    #endif

    std::vector<std::string> filenames;

    if (context->ShadingLanguageFileExt() == "glsl")
        filenames = { shaderPath + "GBufferShader.glvert", shaderPath + "GBufferShader.glfrag" };
    else
        filenames = { shaderPath + "GBufferShader.hlsl", shaderPath + "GBufferShader.hlsl" };

    sceneShader = renderer->GetShaderManager()->LoadShaderComposition(
        filenames, Video::TangentSpaceVertex::Format()
    );
    
    vertexParam = sceneShader->CreateAndAttachConstantBuffer("vertexParam");
    pixelParam = sceneShader->CreateAndAttachConstantBuffer("pixelParam");

    // Create sky-box shader
    if (context->ShadingLanguageFileExt() == "glsl")
        filenames = { shaderPath + "GBufferSkyBoxShader.glvert", shaderPath + "GBufferSkyBoxShader.glfrag" };
    else
        filenames = { shaderPath + "GBufferSkyBoxShader.hlsl", shaderPath + "GBufferSkyBoxShader.hlsl" };

    sceneSkyBoxShader = renderer->GetShaderManager()->LoadShaderComposition(
        filenames, Video::Simple3DVertex::Format()
    );

    sceneSkyBoxShader->Attach(vertexParam, "vertexParam");

    // Setup event handler for the scene shaders
    auto shaderEvtHandler = std::make_shared<SceneShaderEventHandler>(vertexParam.get());

    sceneShader->SetEventHandler(shaderEvtHandler);
    sceneSkyBoxShader->SetEventHandler(shaderEvtHandler);

    // Setup cube-map renderer
    cubeMapRenderer.projection.SetPlanes(0.1f, 1000.0f);
}

void MainSceneRenderer::Render(Scene::SceneNode* graphRootSceneNode, const Scene::CameraNode& camera)
{
    // Render scene into g-buffer
    if (enableRenderTarget)
    {
        context->BindRenderTarget(gbuffer.GetRenderTarget());
        {
            context->ClearBuffers();
            RenderSceneFromCamera(graphRootSceneNode, camera);
        }
        context->BindRenderTarget(context->GetMainRenderTarget());
    }
    else
    {
        context->ClearBuffers();
        RenderSceneFromCamera(graphRootSceneNode, camera);
    }
}

void MainSceneRenderer::RenderCubeMap(Scene::SceneNode* sceneGraph, const Math::Point3f& cameraPosition)
{
    cubeMapRenderer.Begin();
    {
        cubeMapRenderer.RenderCubeMap(cubeGbuffer.GetCubeRenderTargetRefs(), this, sceneGraph, cameraPosition);
    }
    cubeMapRenderer.End();
    context->GenerateMIPMaps(cubeGbuffer.GetColorMap());
}

void MainSceneRenderer::RenderScene(Scene::SceneNode* graphRootSceneNode)
{
    #if 0//!!!
    context->SetupProjectionMatrix(camera->projection);
    context->SetupViewport(camera->projection.GetViewport());
    #endif

    // Draw scene with scene shader
    UpdateSurfaceSettings(1, 0.0f);
    context->BindShader(sceneShader.get());
    ForwardSceneRenderer::RenderScene(graphRootSceneNode);
}

void MainSceneRenderer::VisitTexturedGeometry(Scene::TexturedGeometry* node)
{
    UpdateSurfaceSettings(node->textures.size(), surfaceNormalDeviation);
    ForwardSceneRenderer::VisitTexturedGeometry(node);
}

void MainSceneRenderer::UpdateSurfaceSettings(size_t numTextures, float normalDeviation)
{
    // Update pixel shader constant buffer
    struct
    {
        int     numTextures;
        float   normalDeviation;
        int     __pad0[2];
    }
    buffer
    {
        static_cast<int>(numTextures),
        normalDeviation
    };
    renderer->UpdateBuffer(pixelParam.get(), buffer);
}


// --- SceneShaderEventHandler --- //

MainSceneRenderer::SceneShaderEventHandler::SceneShaderEventHandler(Video::ConstantBuffer* vertexParam) :
    vertexParam{ vertexParam }
{
}

void MainSceneRenderer::SceneShaderEventHandler::OnUpdateConstantBuffers(
    const Video::ShaderComposition* shaderComposition, Video::RenderContext* renderContext)
{
    struct
    {
        Math::Matrix4f  wvpMatrix;
        Math::Matrix4f  worldMatrix;
        Math::Point4f   viewPos;
    }
    param
    {
        renderContext->WVPMatrix(),
        renderContext->GetRenderState().worldMatrix,
        Math::Point4f(renderContext->GetRenderState().cameraMatrix.GetPosition())
    };
    renderer->UpdateBuffer(vertexParam, param);
}


// --- SSCTTest --- //

void SSCTTest::OnInit()
{
    IO::Log::AddEventHandler(std::make_shared<IO::LogFile::LogHTMLEventHandler>("DebugLog.html", "SSCT Test Log File"));

    #if 0
    const Math::Size2ui resolution { 512, 512 };
    #else
    //const Math::Size2ui resolution { 640, 480 };
    //const Math::Size2ui resolution { 800, 600 };
    const Math::Size2ui resolution { 960, 540 }; // resolution used in 'Hi-Z' chapter of GPUPro5 (0.7 - 0.8 ms)
    //const Math::Size2ui resolution { 1280, 720 };
    //const Math::Size2ui resolution { 1280, 768 };
    //const Math::Size2ui resolution { 1920, 1080 };
    #endif

    static const bool enableFrameResize = false;

    InitEngine(L"SSCT Test", resolution, (resolution == Math::Size2ui(1920, 1080)), 8u, enableFrameResize);

    UpdateFrameTitle();

    renderer = renderSystem.get();
    context = renderContext.get();
    camera = &mainCamera;

    context->EnableVSync(false);

    if (resolution == Math::Size2ui(1920, 1080) && !context->GetVideoMode().isFullscreen)
    {
        frame->ChangeDesc({ true, true });
        frame->SetupPosition({ 0, 0 });
        frame->SetupSize(resolution.Cast<int>());
    }

    SetupTestSampler(testSamplerLodBias);

    #ifdef BUILD_FOR_STANDALONE
    sceneManager.texturePathDict.AddSearchPath("textures/");
    renderer->GetShaderProvider()->ShaderPath() = "shaders/";
    #else
    sceneManager.texturePathDict.AddSearchPath("../../../media/Textures/");
    renderer->GetShaderProvider()->ShaderPath() = "../../shaders/";
    #endif

    // Create render states
    Video::BlendState::Description blendDescPostProcessor;
    {
        blendDescPostProcessor.isBlendEnabled = false;
    }
    blendStatePostProcessor = renderSystem->CreateBlendState(blendDescPostProcessor);

    // Create time query
    timeQuery = renderSystem->CreateQuery(Video::Query::Types::TimeStamp);

    // Create scene
    CreateScene();

    PrintDebugKeyBinding();

    GetTerminal()->GetCommandFactory()->AddCommand(std::make_shared<Util::StandardCommand>(
        "show.fps",
        "Shows the current frames per second (FPS)",
        [&](const IO::CommandLine::ArgumentListType& args, Util::Terminal* terminal, void* userData) -> bool
        {
            IO::Log::Message("Current FPS: " + ToStr(engine.GetGlobalTimer()->GetFrameRate()));
            return true;
        }
    ));

    GetTerminal()->GetCommandFactory()->AddCommand(std::make_shared<Util::StandardCommand>(
        "show.position",
        "Shows the current camera position",
        [&](const IO::CommandLine::ArgumentListType& args, Util::Terminal* terminal, void* userData) -> bool
        {
            IO::Log::Message("Camera Position: " + IO::Printer::Print(camera->transform.GetPosition()));
            return true;
        }
    ));

    #ifdef _DEBUG_SSCT_
    Video::BlendState::Description blendDesc;
    blendDesc.colorMask = Video::ColorRGBAb(false);
    blendState = renderSystem->CreateBlendState(blendDesc);
    #endif
}

void SSCTTest::OnUpdate()
{
    cameraCtrl.moveFactor = static_cast<float>(engine.GetGlobalTimer()->GetDeltaTime()) * 1.8f;

    if (MOUSE_DOWN(Left) || MOUSE_DOWN(Right))
    {
        cameraCtrl.ProcessRotation(&mainCamera);
        //cameraCtrl.pitch = 0.0f;//!!!
    }

    if (MOUSE_HIT(Middle))
    {
        static size_t animFrame;
        cameraTrackAnim.keyframeSequence.AddTransform(animFrame++, camera->transform);
        cameraTrackAnim.keyframeSequence.BuildKeyframes();
        IO::Log::Message("Added Camera Animation Keyframe #" + ToStr(animFrame));
    }

    if (KEY_HIT(Space))
    {
        isCameraAnim = !isCameraAnim;
        if (isCameraAnim)
            cameraTrackAnim.playback.Play(0, cameraTrackAnim.keyframeSequence.LastFrame(), 0.3, std::make_shared<Anim::DefaultPlayback::Loop>());
        else
            cameraTrackAnim.playback.Stop();
    }

    if (KEY_HIT(F9))
        showFrameRates = !showFrameRates;

    if (KEY_HIT(F11))
    {
        switch (activeEffect)
        {
            case Effects::None:
                activeEffect = Effects::SSCT;
                IO::Log::Message("Post-processor switched to <SSCT>");
                break;
            case Effects::SSCT:
                activeEffect = Effects::SSLR;
                IO::Log::Message("Post-processor switched to <SSLR>");
                break;
            case Effects::SSLR:
                activeEffect = Effects::None;
                IO::Log::Message("Post-processor switched to <None>");
                break;
        }
        sceneRenderer->enableRenderTarget = (activeEffect != Effects::None);
    }

    if (isCameraAnim)
        cameraTrackAnim.Update(engine.GetGlobalTimer()->GetDeltaTime());
    else
        cameraCtrl.ProcessMovement(&mainCamera);
}

void SSCTTest::OnRender()
{
    // Draw 3D
    mainCamera.UpdateView();

    SetupDefaultRenderStates();

    DrawScene();
}

void SSCTTest::OnFrameResize(const Math::Size2i& clientAreaSize)
{
    Engine::SimpleApp::OnFrameResize(clientAreaSize);
    sceneRenderer->GetGBuffer()->Resize(clientAreaSize);
    
    ssctProcessor->ResizeResolution(clientAreaSize);
    sslrProcessor->ResizeResolution(clientAreaSize);

    UpdateFrameTitle();
}

void SSCTTest::OnDrawCurrentFrame()
{
    if (frame->HasFocus())
        Engine::SimpleApp::OnDrawCurrentFrame();
}

void SSCTTest::CreateScene()
{
    sceneRenderer = std::make_unique<MainSceneRenderer>();

    cameraTrackAnim.transform = &(camera->transform);

    // Load skybox textures
    #ifdef BUILD_FOR_STANDALONE
    const std::string skyBoxPath = "textures/";
    #else
    const std::string skyBoxPath = "../../../media/Textures/SkyboxSunny/";
    #endif

    const Math::Cuboid<std::string> cubeMapFilenames
    {
        skyBoxPath + "TropicalSunnyDayLeft1024.png",
        skyBoxPath + "TropicalSunnyDayUp1024.png",
        skyBoxPath + "TropicalSunnyDayFront1024.png",
        skyBoxPath + "TropicalSunnyDayRight1024.png",
        skyBoxPath + "TropicalSunnyDayDown1024.png",
        skyBoxPath + "TropicalSunnyDayBack1024.png",
    };
    
    auto skyBoxCubeMap = renderSystem->GetTextureManager()->CreateTextureCubeFromFiles(cubeMapFilenames);

    // Create SSCT post processor
    auto gbuffer = sceneRenderer->GetGBuffer();
    auto cubeGbuffer = sceneRenderer->GetCubeGBuffer();

    ssctProcessor = std::make_unique<Video::SSCT>(context->Resolution().Cast<int>());
    ssctProcessor->Setup(
        gbuffer->GetColorMap(), gbuffer->GetNormalMap(),
        gbuffer->GetDepthMap(), gbuffer->GetSpecularMap(),
        #ifdef USE_DYNAMIC_CUBEMAP
        cubeGbuffer->GetColorMap()
        #else
        skyBoxCubeMap.get()
        #endif
    );
    ssctProcessor->SetupProjection(mainCamera.projection.GetMatrixLH());

    // Create SSLR post processor
    sslrProcessor = std::make_unique<Video::SSLR>(context->Resolution().Cast<int>());
    sslrProcessor->Setup(
        gbuffer->GetColorMap(), gbuffer->GetNormalMap(),
        gbuffer->GetDepthMap(), gbuffer->GetSpecularMap()
    );
    sslrProcessor->SetupProjection(mainCamera.projection.GetMatrixLH());
    
    // Add further terminal commands
    GetTerminal()->GetCommandFactory()->AddCommand(std::make_shared<Util::StandardCommand>(
        "gbuffer",
        "Shows all textures of the g-buffer",
        [&](const IO::CommandLine::ArgumentListType& args, Util::Terminal* terminal, void* userData) -> bool
        {
            sceneRenderer->GetGBuffer()->ShowTextures(terminal);
            return true;
        }
    ));

    // Load scene
    #ifdef SPONZA_SCENE

    #   ifdef BUILD_FOR_STANDALONE
    //const std::string modelPath = "models/";
    #   else
    const std::string modelPath = "../../../media/Projects/SSCT/Scene/sponza_obj/";
    #   endif

    const std::string modelFilename = modelPath + "sponza";

    auto worldNode = Scene::LoadMesh(
        sceneManager, sceneGraph, modelFilename + ".obj",
        (Scene::ModelReader::Flags::OptimizeGeometryGraph | Scene::ModelReader::Flags::GenerateTangentSpace)
    );

    worldNode->transform.SetScale(Math::Vector3f(0.01f));

    #else

    #   ifdef BUILD_FOR_STANDALONE
    const std::string modelPath = "models/";
    #   else
    const std::string modelPath = "";
    #   endif

    const std::string modelFilename = modelPath + "SSCTTestScene2";

    auto worldNode = Scene::LoadMesh(
        sceneManager, sceneGraph, modelFilename + ".obj",
        (Scene::ModelReader::Flags::OptimizeGeometryGraph | Scene::ModelReader::Flags::GenerateTangentSpace)
    );

    auto worldWindowsNode = Scene::LoadMesh(
        sceneManager, sceneGraph, modelFilename + "-Windows.obj"
    );

    worldWindowsNode->shaderComposition = sceneRenderer->GetSceneSkyBoxShader();

    // Create skybox texture geometry
    auto texGeom = std::make_shared<Scene::TexturedGeometry>();

    texGeom->actualGeometry = worldWindowsNode->geometry;
    texGeom->textures.push_back(skyBoxCubeMap);

    worldWindowsNode->geometry = texGeom;

    #endif
}

void SSCTTest::SetupTestSampler(int mipLodBias)
{
    Video::SamplerState::Description desc;
    {
        desc.minFilter = Video::SamplerState::Filters::Nearest;
        desc.mipLODBias = static_cast<float>(mipLodBias);
    }

    if (!testSampler)
        testSampler = renderer->CreateSamplerState(desc);
    else
        renderer->ChangeStateDesc(testSampler.get(), desc);
}

void SSCTTest::DrawScene()
{
    // Draw 3D
    #ifdef USE_DYNAMIC_CUBEMAP
    sceneRenderer->RenderCubeMap(&sceneGraph, mainCamera.transform.GetPosition());
    #endif
    sceneRenderer->Render(&sceneGraph, mainCamera);

    // Draw 2D
    static unsigned int timeStamp;

    bool tempDisableFrameRates = (keyboard->KeyDown(IO::KeyCodes::KeyBack));

    primitiveRenderer->BeginDrawing2D();
    {
        renderContext->ChangeState(blendStatePostProcessor.get());

        if (showFrameRates && !tempDisableFrameRates)
            renderContext->BeginQuery(timeQuery.get());

        switch (activeEffect)
        {
            case Effects::SSCT:
                // Draw SSCT effect
                ssctProcessor->SetupView(mainCamera.GetViewMatrix());
                ssctProcessor->Draw(context->GetMainRenderTarget());
                break;
            case Effects::SSLR:
                // Draw SSLR effect
                sslrProcessor->SetupView(mainCamera.GetViewMatrix());
                sslrProcessor->Draw(context->GetMainRenderTarget());
                break;
        }

        if (showFrameRates && !tempDisableFrameRates)
        {
            renderContext->EndQuery(timeQuery.get());
            timeStamp = renderContext->QueryResult(timeQuery.get());
        }

        renderContext->ChangeState(renderSystem->GetDefaultBlendState());
    }
    primitiveRenderer->EndDrawing2D();

    primitiveRenderer->BeginDrawing2D();
    {
        //primitiveRenderer->DrawImage(sceneRenderer->GetGBuffer()->GetColorMap(), Math::Point2f());
        DrawDebugging();
    }
    primitiveRenderer->EndDrawing2D();

    auto font = GetTerminal()->GetDefaultFont();

    #ifdef _DEBUG_SSCT_

    // Re-write depth buffer
    renderContext->ClearBuffers(Video::ClearBuffersFlags::Depth);
    renderContext->ChangeState(blendState.get());
    renderContext->ChangeState(renderSystem->GetDefaultDepthStencilState());
    renderContext->ChangeState(renderSystem->GetDefaultRasterizerState());
    {
        GetSceneRenderer()->RenderSceneFromCamera(&sceneGraph, mainCamera);
    }
    renderContext->ChangeState(renderSystem->GetDefaultBlendState());

    // Draw SSCT debug info
    ssctProcessor->_Deb_DrawDebugInfo(
        primitiveRenderer.get(), font,
        mainCamera.GetViewMatrix(), mainCamera.projection
    );

    #endif

    if (showFrameRates)
    {
        static const int digitPrec = 2;
        primitiveRenderer->BeginDrawing2D();
        {
            auto posY = static_cast<float>(renderContext->Resolution().height) - 20;
            primitiveRenderer->DrawText2D(
                font, { 5, posY },
                "Frame Rate: " + ToStr(engine.GetGlobalTimer()->GetElapsedTime() / 1000.0, digitPrec) +
                " ms ( " + ToStr(engine.GetGlobalTimer()->GetFrameRate(), digitPrec) + " Hz )"
            );
            primitiveRenderer->DrawText2D(
                font, { 5, posY - 20 },
                "Effect Timestamp: " + ToStr(static_cast<double>(timeStamp) / 1000000.0, digitPrec) + " ms"
            );
        }
        primitiveRenderer->EndDrawing2D();
    }

    if (KEY_HIT(F12))
        SaveScreenshot();
}

void SSCTTest::PrintDebugKeyBinding()
{
    IO::Log::Blank();
    IO::Log::Message("<------ SSCT Test Debugging Info ------>");
    IO::Log::Blank();
    IO::Log::Message("Moving:");
    IO::Log::Message("  W/A/S/D     -> Move Camera");
    IO::Log::Message("  Shift       -> Move with 3x Speed");
    IO::Log::Message("  Space       -> Camera Track Animation On/Off");
    IO::Log::Blank();
    IO::Log::Message("Common Debugging:");
    IO::Log::Message("  F1          -> Debug Ray Marching Info On/Off");
    IO::Log::Message("  F2          -> Debug Color-/ Visibility- /Hi-Z Map On/Off");
    IO::Log::Message("  F3          -> Open/Close Terminal");
    IO::Log::Message("  F4          -> Print This Info");
    IO::Log::Message("  F5          -> Reload Shaders");
    IO::Log::Message("  F7          -> Save Settings");
    IO::Log::Message("  F8          -> Load Settings");
    IO::Log::Message("  F9          -> Show/Hide Frame Rates");
    IO::Log::Message("  F11         -> Switch between SSCT and SSLR");
    IO::Log::Message("  F12         -> Save Screenshot");
    IO::Log::Blank();
    IO::Log::Message("  Page Up     -> Test Sampler MIP Level Up");
    IO::Log::Message("  Page Down   -> Test Sampler MIP Level Down");
    IO::Log::Message("  Enter       -> Safe Debug Map to Text file");
    IO::Log::Blank();
    IO::Log::Message("  B           -> SSCT Color Map Blur On/Off");
    IO::Log::Message("  L           -> Debug Ray Marching Levels On/Off");
    IO::Log::Message("  V           -> Reset Camera View Orientation");
    IO::Log::Blank();
    IO::Log::Message("  M           -> SSCT Debug Map Selection");
    IO::Log::Message("                  1st: Color Map");
    IO::Log::Message("                  2nd: Visibility Map");
    IO::Log::Message("                  3rd: Hi-Z Depth Map");
    IO::Log::Blank();
    IO::Log::Message("  Mouse Wheel -> SSCT Debug Map Selection");
    IO::Log::Message("                  E: Hi-Z Epsilon");
    IO::Log::Message("                  R: Roughness");
    IO::Log::Message("                  Y: Reflectivity");
    IO::Log::Message("                  N: Normal Deviation");
    IO::Log::Blank();
    IO::Log::Message("Buffer Visualization:");
    IO::Log::Message("  0 -> Off");
    IO::Log::Message("  1 -> View-Space Position");
    IO::Log::Message("  2 -> Linear Depth");
    IO::Log::Message("  3 -> Post-Projected Depth");
    IO::Log::Message("  4 -> View-Space Normal");
    IO::Log::Message("  5 -> View-Space Reflection");
    IO::Log::Message("  6 -> Screen-Space Reflection XY");
    IO::Log::Message("  7 -> Screen-Space Reflection Z");
    IO::Log::Message("  8 -> Ray Tracing Method Distribution");
    IO::Log::Message("  9 -> Ray Hit Point (Tex-Coord)");
    IO::Log::Blank();
}

void SSCTTest::DrawDebugging()
{
    static bool isDebugMap;
    static int debugMapIndex;
    
    if (!GetTerminal()->IsEnabled())
    {
        #ifdef _DEBUG_SSCT_
        if (KEY_HIT(F1))
            ssctProcessor->_Deb_RayMarching = !ssctProcessor->_Deb_RayMarching;
        #endif

        if (KEY_HIT(F2))
            isDebugMap = !isDebugMap;
        if (KEY_HIT(F4))
            PrintDebugKeyBinding();

        #ifdef _DEBUG_SSCT_
        if (KEY_HIT(F5))
        {
            if (ssctProcessor->_Deb_ReloadShaders())
                IO::Log::Message("Reloaded Shaders Successful");
            else
                IO::Log::Error("Reloading Shaders Failed");
        }
        #endif

        if (KEY_HIT(F7))
            SaveSettings();
        if (KEY_HIT(F8))
            LoadSettings();

        if (KEY_HIT(PageUp))
            SetupTestSampler(++testSamplerLodBias);
        if (KEY_HIT(PageDown) && testSamplerLodBias > 0)
            SetupTestSampler(--testSamplerLodBias);

        #ifdef _DEBUG_SSCT_
        if (KEY_HIT(B))
        {
            ssctProcessor->_Deb_UseBlur = !ssctProcessor->_Deb_UseBlur;
            IO::Log::Message(ssctProcessor->_Deb_UseBlur ? "Blur ON" : "Blur OFF");
        }
        #endif

        if (KEY_HIT(M))
        {
            if (++debugMapIndex > 2)
                debugMapIndex = 0;
        }
        if (KEY_HIT(V))
            cameraCtrl.pitch = cameraCtrl.yaw = 0.0f;

        #ifdef _DEBUG_SSCT_
        if (KEY_HIT(L))
            ssctProcessor->_Deb_ShowRayMarchLevels = !ssctProcessor->_Deb_ShowRayMarchLevels;
        #endif
    }

    #ifdef _DEBUG_SSCT_
    ssctProcessor->_Deb_SamplePos = mouse->GetFrameMousePosition().position;

    for (int i = 0; i <= 9; ++i)
    {
        if (keyboard->KeyHit(static_cast<IO::KeyCodes>(static_cast<int>(IO::KeyCodes::Key0) + i)))
            ssctProcessor->_Deb_DebugMap = i;
    }
    #endif

    if (!GetTerminal()->IsEnabled() && mouse->GetWheelMotion() != 0)
    {
        #ifdef _DEBUG_SSCT_
        if (KEY_DOWN(E))
        {
            static int track;
            static const int maxTrack = 100;
        
            track += mouse->GetWheelMotion();
            track = Math::Clamp(track, 0, maxTrack);

            ssctProcessor->_Deb_HiZEpsilon = Math::Lerp(0.02f, 0.001f, static_cast<float>(track) / maxTrack);
            IO::Log::Message("Hi-Z Epsilon = " + ToStr(ssctProcessor->_Deb_HiZEpsilon));
        }
        else
        #endif
        if (KEY_DOWN(R))
        {
            static int roughness;

            roughness += mouse->GetWheelMotion();
            roughness = Math::Clamp(roughness, 0, 100);

            ssctProcessor->roughness = static_cast<float>(roughness)/100.0f;
            sslrProcessor->roughness = static_cast<float>(roughness)/100.0f;

            IO::Log::Message("Roughness = " + ToStr(ssctProcessor->roughness));
        }
        else if (KEY_DOWN(Y))
        {
            static int reflectivity = 20;

            reflectivity += mouse->GetWheelMotion();
            reflectivity = Math::Clamp(reflectivity, 0, 20);

            float factor = static_cast<float>(reflectivity)/20.0f;
            
            ssctProcessor->reflectivity = factor;
            sslrProcessor->reflectivity = factor;

            IO::Log::Message("Reflectivity = " + ToStr(factor));
        }
        else if (KEY_DOWN(N))
        {
            static int deviation = 0;

            deviation += mouse->GetWheelMotion();
            deviation = Math::Clamp(deviation, 0, 20);

            float factor = static_cast<float>(deviation)/20.0f;
            sceneRenderer->surfaceNormalDeviation = factor;
            IO::Log::Message("Normal Deviation = " + ToStr(factor));
        }
    }

    // Select debug map
    Video::Texture2D* debugMapTex = nullptr;
    switch (debugMapIndex)
    {
        #ifdef _DEBUG_SSCT_
        case 0: debugMapTex = ssctProcessor->_Deb_BluredColorMap(); break;
        case 1: debugMapTex = ssctProcessor->_Deb_GetVisibilityMap(); break;
        #endif
        case 2: debugMapTex = sceneRenderer->GetGBuffer()->GetDepthMap(); break;
    }

    if (isDebugMap)
    {
        context->BindSamplerState(testSampler.get());
        {
            primitiveRenderer->DrawImage(debugMapTex, Math::Point2f());
        }
        context->UnbindSamplerState();
    }

    if (!GetTerminal()->IsEnabled() && KEY_HIT(Return))
    {
        #define READ_FLOAT

        #ifdef READ_FLOAT
        Video::ImageFloat image
        #else
        Video::ImageUByte image
        #endif
        (
            { 1, 1, 1 },
            //Video::ImageColorFormats::RGB
            Video::ImageColorFormats::Gray
        );

        renderer->ReadTexture(debugMapTex, image, static_cast<unsigned int>(testSamplerLodBias));

        const std::string filename = "ImageData.txt";
        std::ofstream file(filename);

        #ifdef READ_FLOAT
        auto WriteValue = [](float x) -> std::string
        {
            return ToStr(x, 2);
        };
        #else
        auto WriteValue = [](unsigned char x) -> std::string
        {
            if (x < 10)
                return "  " + std::to_string(x);
            if (x < 100)
                return " " + std::to_string(x);
            return std::to_string(x);
        };
        #endif

        for (size_t y = 0; y < image.GetSize().height; ++y)
        {
            for (size_t x = 0; x < image.GetSize().width; ++x)
            {
                auto color = image.ReadColor({ x, y });
                file << WriteValue(color.r) << "  ";
            }

            file << std::endl;
        }

        IO::Log::Message("Saved \"" + filename + "\"");
    }
}

static std::string settingsFilename = "ssct-settings";

void SSCTTest::SaveSettings()
{
    IO::PhysicalFile file(settingsFilename, IO::File::OpenFlags::Write);

    file.Write(mainCamera.transform.GetPosition());
    file.Write(mainCamera.transform.GetRotation());

    file.Write(ssctProcessor->reflectivity);
    file.Write(sceneRenderer->surfaceNormalDeviation);
    file.Write(ssctProcessor->roughness);

    cameraTrackAnim.keyframeSequence.WriteToFile(file);

    IO::Log::Message("Saved \"" + settingsFilename + "\"");
}

void SSCTTest::LoadSettings()
{
    IO::PhysicalFile file(settingsFilename, IO::File::OpenFlags::Read);

    if (file.IsOpen())
    {
        mainCamera.transform.SetPosition(file.ReadVec3<float>());
        mainCamera.transform.SetRotation(file.Read<Math::Quaternionf>());

        ssctProcessor->reflectivity             = file.Read<float>();
        sceneRenderer->surfaceNormalDeviation   = file.Read<float>();
        ssctProcessor->roughness                = file.Read<float>();

        sslrProcessor->reflectivity = ssctProcessor->reflectivity;
        sslrProcessor->roughness    = ssctProcessor->roughness;

        cameraTrackAnim.keyframeSequence.ReadFromFile(file);
        
        IO::Log::Message("Loaded \"" + settingsFilename + "\"");
    }
    else
        IO::Log::Error("Loading \"" + settingsFilename + "\" failed");
}

void SSCTTest::SaveScreenshot()
{
    /* Choose filename */
    static int index;
    const std::string path = "";
    auto filename = path + "SSCT-Screenshot-" + ToStr(++index) + ".png";

    /* Save image to file */
    if (renderContext->SaveScreenshot(filename))
        IO::Log::Message("Saved screenshot to file \"" + filename + "\"");
}

void SSCTTest::UpdateFrameTitle()
{
    auto res = renderContext->Resolution().Cast<int>();
    frame->SetupTitle(UTF8toUTF16("SSCT Test [ " + ToStr(res.width) + " x " + ToStr(res.height) + " ]"));
}

