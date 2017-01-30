
// ForkENGINE: Scene Test
// 12/03/2014

#include "../TestUtils.h"

#include <chrono>
#include <algorithm>

using namespace Fork;


// Global members

std::chrono::system_clock::time_point startTime, endTime;

Video::RenderSystem* renderSys = nullptr;
Video::RenderContext* renderCtx = nullptr;

Video::ConstantBuffer* vertexParam = nullptr;
Video::ConstantBuffer* pixelParam = nullptr;

Util::Projector* projectorRef = nullptr;

std::unique_ptr<Video::GaussianBlur> gaussianBlur;

Video::Texture2DPtr frameBufferTexture;
Video::RenderTargetPtr frameBufferTarget;

Scene::GeometryNode meshNode;
Scene::CameraNode camera;


// Static functions

static void BeginStopWatch()
{
    startTime = std::chrono::system_clock::now();
}

static void EndStopWatch(const std::string& desc)
{
    endTime = std::chrono::system_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
    IO::Log::Message("Stop Watch (" + desc + ") Duration: " + ToStr(duration) + " ms.");
}


// Classes

class CustomSceneRenderer : public Scene::ForwardSceneRenderer
{
    
    public:
        
        void VisitTexturedGeometry(Scene::TexturedGeometry* node)
        {
            for (size_t l = 0; l < node->textures.size(); ++l)
                renderCtx->BindTexture(node->textures[l].get(), l);
            
            if (pixelParam)
            {
                struct
                {
                    Video::ColorRGBAf color;
                    Math::Vector4i flags;
                }
                param;
                {
                    param.color = { 1, 1, 1, 1 };
                    param.flags.x = (node->textures.empty() ? 0 : 1);
                    param.flags.y = 1;//param.flags.x;
                }
                renderSys->UpdateBuffer(pixelParam, param);
            }

            if (node->actualGeometry)
                node->actualGeometry->Visit(this);

            for (size_t l = 0; l < node->textures.size(); ++l)
                renderCtx->UnbindTexture(node->textures[l].get(), l);
        }

        void VisitGeometryNode(Scene::GeometryNode* node)
        {
            renderCtx->SetupWorldMatrix(node->transform.GetMatrix());

            if (vertexParam)
            {
                struct
                {
                    Matrix4f wvpMatrix;
                    Matrix4f worldMatrix;
                }
                param;
                {
                    param.wvpMatrix = renderCtx->WVPMatrix();
                    param.worldMatrix = renderCtx->GetRenderState().worldMatrix;
                }
                renderSys->UpdateBuffer(vertexParam, param);
            }

            if (pixelParam)
            {
                struct
                {
                    Video::ColorRGBAf color;
                    Math::Vector4i flags;
                }
                param;
                {
                    param.color = { 1, 1, 1, 1 };
                    param.flags.x = 0;
                    param.flags.y = 1;
                }
                renderSys->UpdateBuffer(pixelParam, param);
            }

            if (node->geometry)
                node->geometry->Visit(this);
        }

};

const int projectorSize = 100;

static void UpdateViewport(const Math::Size2i& clientAreaSize)
{
    projectorRef->GetModel()->SetupViewportDefaultVisual(
        { { 0, 0 }, clientAreaSize },
        projectorSize
    );

    renderCtx->SetupViewport(
        projectorRef->GetModel()->GetProjection().GetViewport()
    );
}

class FrameEventHandler : public Platform::Frame::EventHandler
{
    
    public:
        
        void OnResize(const Math::Size2i& clientAreaSize) override
        {
            renderCtx->ChangeVideoMode({ clientAreaSize.Cast<unsigned int>() });
            UpdateViewport(clientAreaSize);
            if (gaussianBlur)
                gaussianBlur->ResizeResolution(clientAreaSize);
            
            frameBufferTarget->Clear();
            renderSys->WriteTexture(frameBufferTexture.get(), Video::TextureFormats::RGBA, clientAreaSize);
            frameBufferTarget->Attach(frameBufferTexture);
        }

};

static Scene::Simple3DMeshGeometryPtr CreateSphereGeometry(unsigned int segs)
{
    auto geometry = std::make_shared<Scene::Simple3DMeshGeometry>();
    Scene::GeometryGenerator::GenerateUVSphere(*geometry, { 1.0f, segs });
    geometry->SetupHardwareBuffer();
    return geometry;
}

static Scene::Simple3DMeshGeometryPtr CreateCapsuleGeometry(unsigned int segs)
{
    auto geometry = std::make_shared<Scene::Simple3DMeshGeometry>();
    Scene::GeometryGenerator::GenerateCylinder(*geometry, { 1.0f, 2.0f, segs });
    geometry->SetupHardwareBuffer();
    return geometry;
}

static Scene::Simple3DMeshGeometryPtr CreateCubeGeometry()
{
    auto geometry = std::make_shared<Scene::Simple3DMeshGeometry>();
    Scene::GeometryGenerator::GenerateCube(*geometry);
    geometry->SetupHardwareBuffer();
    return geometry;
}


// Main function

int main()
{
    FORK_MATCH_SDK_VERSION

    IO::Log::AddDefaultEventHandler();

    Engine::EngineDevice engine;

    meshNode.transform.SetPosition({ 0, 0, 5 });

    //CustomSceneRenderer sceneRenderer;

    try
    {
        auto renderSystem = engine.CreateRenderSystem();//L"ForkRendererD3D11");
        renderSys = renderSystem.get();

        Video::ContextDescription contextDesc(true, true, 8);
        contextDesc.rendererProfileDesc.useExtProfile = true;
        contextDesc.rendererProfileDesc.useGLCoreProfile = true;
        //contextDesc.rendererProfileDesc.versionGL = { 3, 2 };

        const Math::Size2ui screenSize(800, 600);

        auto renderContext = renderSystem->CreateRenderContext(
            Video::VideoMode(screenSize), contextDesc, L"Scene Test"
        );
        renderCtx = renderContext.get();

        renderContext->GetFrame()->ChangeDesc({ true, false, true });

        Engine::PrintEngineInfo(renderContext.get());

        // Some setup
        //auto mesh = CreateCapsuleGeometry(32);
        auto mesh = CreateCubeGeometry();
        
        meshNode.geometry = mesh;

        Scene::SceneManager sceneMngr;

        camera.projection.SetViewport({ { 0, 0 }, screenSize.Cast<int>() });
        camera.projection.SetNearPlane(0.1f);

        renderContext->SetupViewport(camera.projection.GetViewport());

        // Load models from file
        Video::TextureManager texMngr;

        const std::string mdlPath
        {
            "???"
        };
        const std::string mdlFile
        {
            mdlPath +
            //"crate01a.3ds"
            "House1.3ds"
        };

        auto mdlNode = sceneMngr.CreateGeometryNode();
        auto texturedGeometry = std::make_shared<Scene::TexturedGeometry>();
        mdlNode->geometry = texturedGeometry;

        texturedGeometry->actualGeometry = sceneMngr.LoadGeometry(mdlFile);

        texturedGeometry->textures.push_back(texMngr.CreateTexture2DFromFile(
            mdlPath +
            //"crate01a.jpg"
            "House1.jpg"
        ));

        texturedGeometry->ComputeBoundingVolume();

        mdlNode->transform.SetPosition({ 0, 0, 5 });

        Scene::SceneNode sceneGraph;
        sceneGraph.AddChild(mdlNode);

        auto obj1 = sceneMngr.CreateGeometryNode(sceneMngr.GenerateIcoSphere());
        sceneGraph.AddChild(obj1);

        // Get frame
        auto frame = renderContext->GetFrame();
        renderContext->SetupClearColor(Video::ColorRGBub(20, 40, 100).Cast<float>());

        frame->SetEventHandler(std::make_shared<FrameEventHandler>());

        // Get input devices
        auto keyboard = IO::Keyboard::Instance();
        auto mouse = IO::Mouse::Instance();

        // Create texture
        const std::string texPath = "../../../media/Textures/";
        const std::string texFile = texPath + "StonesColorMap.jpg";//"HDR Test Image 1.hdr";

        #if 1
        auto texA = texMngr.LoadTexture2D(texFile);//, true);
        #else
        auto texA = texMngr.CreateTexture2DArrayFromFiles(
            {
                texPath + "StonesColorMap.jpg",
                texPath + "StonesNormalMap.jpg",
                texPath + "StonesHeightMap.jpg",
                texPath + "Test_JPEG_Baseline.jpg",
                texPath + "Test_JPEG_Progressive.jpg"
            }
        );
        #endif

        //renderSystem->Bind(texA.get());

        // Create font
        auto primRenderer = engine.QueryPrimitiveRenderer(renderSystem.get());

        auto font = primRenderer->CreateTextureFont({ "arial", 20 });

        // Create shaders
        std::vector<std::string> shaderFiles;

        if (renderContext->ShadingLanguageFileExt() == "hlsl")
            shaderFiles = { "TestShader.hlsl", "TestShader.hlsl" };
        else
            shaderFiles = { "TestShader.glvert", "TestShader.glfrag" };

        auto shaderComposition = renderSystem->GetShaderManager()->LoadShaderComposition(
            shaderFiles, Video::Simple3DVertex::Format()
        );

        vertexParam = shaderComposition->CreateAndAttachConstantBuffer("vertexParam").get();
        pixelParam = shaderComposition->CreateAndAttachConstantBuffer("pixelParam").get();

        // Create standard depth-stencil state
        Video::DepthStencilState::Description depthStencilDesc;
        auto depthStencilState = renderSystem->CreateDepthStencilState(depthStencilDesc);
        renderContext->ChangeState(depthStencilState.get());

        Util::Terminal term(primRenderer);
        term.GetView()->groundColor.a = 0.8f;

        // Create selector
        class SelectNode : public Util::Selectable
        {
            public:
                SelectNode(Scene::DynamicSceneNode* sceneNode) :
                    sceneNode_(sceneNode)
                {
                }
                const Scene::Transform& LocalTransform() const override
                {
                    return sceneNode_->transform;
                }
                Math::Matrix4f GlobalTransform() const override
                {
                    return sceneNode_->GlobalTransform();
                }
            private:
                Scene::Transform& GetTransform() override
                {
                    return sceneNode_->transform;
                }
                Scene::DynamicSceneNode* sceneNode_;
        };

        SelectNode mdlNodeSel(mdlNode.get());
        SelectNode obj1Sel(obj1.get());

        Util::Selector selector(primRenderer, &sceneMngr);
        selector.GetModel()->AddNode(&mdlNodeSel);
        selector.GetModel()->AddNode(&obj1Sel);
        selector.GetModel()->transformMode = Util::SelectorModel::TransformModes::Rotate;
        //selector.GetModel()->raster.mode = Util::SelectorModel::Raster::Modes::Absolute;
        //selector.GetModel()->raster.moveFreeInRaster = false;

        // Create projector
        Util::Projector projector(primRenderer);

        projector.GetModel()->SetupProjection(camera.projection);
        projector.GetModel()->SetupViewportDefaultVisual(camera.projection.GetViewport(), projectorSize);

        projectorRef = &projector;

        // Some scene node controllers
        Scene::FreeViewSceneNodeController camCtrl;
        Scene::PresentSceneNodeController mdlCtrl;

        Scene::BoundingBoxSceneRenderer boundingBoxRenderer(&sceneMngr);

        //#define QUERY_TEST
        #ifdef QUERY_TEST
        // Occlusion query
        auto query = renderSystem->CreateQuery(Video::Query::Types::SamplesPassed);
        #endif

        // Gaussian blur test
        frameBufferTexture = renderSystem->CreateTexture2D(Video::TextureFormats::RGBA, screenSize.Cast<int>());
        frameBufferTarget = renderSystem->CreateRenderTarget({ frameBufferTexture });
        
        #if 1
        gaussianBlur = std::make_unique<Video::GaussianBlur>(screenSize.Cast<int>());
        gaussianBlur->Setup(frameBufferTexture.get());
        #endif

        #if 0
        auto sceneRenderer = std::make_unique<Scene::SimpleSceneRenderer>();
        Scene::LightNode lightNode;
        lightNode.lightSource = std::make_shared<Scene::PointLightSource>();
        lightNode.transform.SetPosition({ 2, 3, -5 });
        sceneRenderer->UpdateLightNodes({ &lightNode });
        #else
        CustomSceneRenderer customSceneRenderer;
        auto sceneRenderer = &customSceneRenderer;
        #endif

        // Main loop
        while (frame->ReceiveEvents() && !KEY_DOWN(Escape))
        {
            // Game logic
            if (KEY_HIT(F3))
                term.Enable(!term.IsEnabled());

            if (!term.IsEnabled())
            {
                /*if (keyboard->KeyHit(IO::KeyCodes::KeyReturn))
                {
                    if (renderContext->GetVideoMode().isFullscreen)
                        renderContext->ChangeVideoMode({ { 800, 600 }, 32, false });
                    else
                        renderContext->ChangeVideoMode({ { 1920, 1080 }, 32, true });
                    UpdateViewport(renderContext->GetVideoMode().resolution.Cast<int>());
                }*/

                if (KEY_DOWN(Shift))
                {
                    if (KEY_HIT(1))
                        selector.GetModel()->orientationAlignment = Utility::SelectorModel::OrientationAlignments::Global;
                    if (KEY_HIT(2))
                        selector.GetModel()->orientationAlignment = Utility::SelectorModel::OrientationAlignments::Local;
                    if (KEY_HIT(3))
                        selector.GetModel()->orientationAlignment = Utility::SelectorModel::OrientationAlignments::View;
                }
                else
                {
                    if (KEY_HIT(1))
                        selector.GetModel()->transformMode = Utility::SelectorModel::TransformModes::Translate;
                    if (KEY_HIT(2))
                        selector.GetModel()->transformMode = Utility::SelectorModel::TransformModes::Rotate;
                    if (KEY_HIT(3))
                        selector.GetModel()->transformMode = Utility::SelectorModel::TransformModes::Scale;
                }

                if (KEY_HIT(F1))
                {
                    Scene::LogSceneRenderer logScene;
                    logScene.RenderScene(&sceneGraph);
                }

                if (KEY_HIT(F5))
                    renderSystem->GetShaderManager()->ReloadShaderComposition(shaderComposition.get());

                //if (mouse->ButtonDown(IO::MouseKeyCodes::MouseLeft))
                //    mdlCtrl.ProcessRotation(&meshNode);

                if (mouse->ButtonDown(IO::MouseKeyCodes::MouseRight))
                    camCtrl.ProcessRotation(&camera);
                camCtrl.ProcessMovement(&camera);

                //mdlNode->transform = meshNode.transform;
            }

            camera.UpdateView();

            camera.projection = projector.GetModel()->GetProjection();

            #if 0 // PICKING TEST

            {
                auto point = IO::Mouse::Instance()->GetFrameMousePosition().position.Cast<float>();
                auto ray = camera.PickingRay(point);

                obj1->transform.SetScale(Math::Vector3f(0.25f));
                obj1->transform.SetPosition(ray.Lerp(5));
            }

            #endif

            // Rendering
            renderContext->ClearBuffers();
            {
                if (gaussianBlur)
                {
                    renderContext->BindRenderTarget(frameBufferTarget.get());
                    renderContext->ClearBuffers();
                }

                if (shaderComposition && shaderComposition->HasCompiledSuccessful())
                {
                    // Render scene
                    renderContext->ChangeState(renderSystem->GetDefaultRasterizerState());
                    renderContext->BindShader(shaderComposition.get());

                    #ifdef QUERY_TEST
                    renderContext->BeginQuery(query.get());
                    #endif
                    
                    sceneRenderer->RenderSceneFromCamera(&sceneGraph, camera);
                    
                    #ifdef QUERY_TEST
                    renderContext->EndQuery(query.get());
                    #endif
                }

                boundingBoxRenderer.RenderScene(&sceneGraph);

                // Draw selector
                selector.SetupView(camera);
                selector.Draw(0);

                // Draw 3D
                renderContext->ChangeState(renderSystem->GetDefaultRasterizerState());

                primRenderer->BeginDrawing3D(false);
                {
                    primRenderer->Setup3DProjection(camera.transform.GetMatrix().Inverse());

                    renderContext->SetupLineSize(1.0f);

                    const float originSize = 1.0f;

                    primRenderer->DrawLine(Math::Line4f{ { 0, 0, 0, 1 }, { originSize, 0, 0, 1 } }, { 1, 0, 0, 1 });
                    primRenderer->DrawLine(Math::Line4f{ { 0, 0, 0, 1 }, { 0, originSize, 0, 1 } }, { 0, 1, 0, 1 });
                    primRenderer->DrawLine(Math::Line4f{ { 0, 0, 0, 1 }, { 0, 0, originSize, 1 } }, { 0, 0, 1, 1 });
                }
                primRenderer->EndDrawing3D();

                // Draw projector
                projector.GetModel()->orientationSource = &(camera.transform);
                projector.DrawAndUpdate();

                // Draw gaussian blur
                if (gaussianBlur)
                {
                    #if 1
                    static int numSamples = 8;

                    if (mouse->GetWheelMotion())
                    {
                        numSamples += mouse->GetWheelMotion();
                        gaussianBlur->ChangeConfig(numSamples, Math::Lerp(1.0f, 3.0f, static_cast<float>(numSamples) / 64));//, 100.0f);
                        numSamples = gaussianBlur->GetConfig().numSamples;
                    }
                    #else
                    static float anim;
                    anim += 0.025f;
                    int numSamples = static_cast<int>(std::sin(anim) * 21) + 21;
                    gaussianBlur->ChangeConfig(numSamples, Math::Lerp(1.0f, 3.0f, static_cast<float>(numSamples) / 64));
                    #endif

                    renderContext->ChangeState(primRenderer->GetDepthStencilState());
                    gaussianBlur->Draw(renderContext->GetMainRenderTarget());
                    renderContext->ChangeState(renderSystem->GetDefaultDepthStencilState());
                }

                // Draw 2D
                renderContext->BindRenderTarget(renderContext->GetMainRenderTarget());

                primRenderer->BeginDrawing2D();
                {
                    #ifdef QUERY_TEST
                    auto numSamplesPassed = renderContext->QueryResult(query.get());
                    primRenderer->DrawText2D(font.get(), { 15, 15 }, "Samples Passed: " + ToStr(numSamplesPassed));
                    #endif

                    //primRenderer->DrawImage(frameBufferTexture.get(), Math::Point2f());//!!!

                    // Draw terminal
                    if (term.IsEnabled())
                        term.Draw();
                }
                primRenderer->EndDrawing2D();
            }
            renderContext->Present();

            engine.UpdateFrameStates();
        }
    }
    catch (const DefaultException& err)
    {
        IO::Log::Error(err);
        IO::Console::Wait();
    }
    catch (const std::exception& err)
    {
        IO::Log::Error(err.what());
        IO::Console::Wait();
    }

    frameBufferTexture = nullptr;
    frameBufferTarget = nullptr;
    gaussianBlur = nullptr;
    meshNode.geometry = nullptr;
    
    return 0;
}


