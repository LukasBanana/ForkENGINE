
// ForkENGINE: D3D11 Test
// 20/07/2014

#include <fengine/import.h>
#include <fengine/using.h>
#include <fengine/helper.h>

using namespace Fork;

FORK_IMPLEMENT_APP_MAIN_ONLY(D3D11Test);

class ShaderEventHandler : public Video::ShaderComposition::EventHandler
{

    public:

        void OnDisassemblyOutput(
            const Video::ShaderComposition* shaderComposition,
            const Video::Shader* shader, const std::string& disassemblyOutput)
        {
            IO::Log::Message(disassemblyOutput);
        }

        #ifndef SIMPLE_SHADER_TEST
        void OnUpdateConstantBuffers(const Video::ShaderComposition* shaderComposition, Video::RenderContext* renderContext)
        {
            struct
            {
                Math::Matrix4f wvpMatrix;
                Math::Matrix4f worldMatrix;
                Video::ColorRGBAf diffuseColor;
            }
            vertexParam
            {
                renderContext->WVPMatrix(),
                renderContext->GetRenderState().worldMatrix,
                { 1, 1, 1, 1 }
            };
            renderer->UpdateBuffer(cbufferVertexParam, vertexParam);
        }
        #endif

        Video::RenderSystem*    renderer            = nullptr;
        Video::ConstantBuffer*  cbufferVertexParam  = nullptr;

};

void D3D11Test::OnMain()
{
    auto renderSystem = engine.CreateRenderSystem(L"ForkRendererGL");//D3D11

    Video::ContextDescription contextDesc(true, true, 8);

    const Math::Size2ui screenSize(800, 600);

    auto renderContext = renderSystem->CreateRenderContext(
        Video::VideoMode(screenSize), contextDesc, L"D3D11 Test"
    );

    renderContext->SetupClearColor(Video::ColorRGBub(20, 40, 100).Cast<float>());

    // Setup frame
    auto frame = renderContext->GetFrame();

    // Create font
    auto primRenderer = engine.QueryPrimitiveRenderer(renderSystem.get());
    auto font = primRenderer->CreateTextureFont({ "arial", 32 });

    // Shader event handler
    //#define SIMPLE_SHADER_TEST

    typedef Video::Shader::CompilationFlags ShaderFlags;

    // Create shader
    const auto shaderFile = (
        #ifdef SIMPLE_SHADER_TEST
        "TestShader.hlsl"
        #else
        "TestMeshShader.hlsl"
        #endif
    );

    auto shader = renderSystem->GetShaderManager()->CreateShaderCompositionFromFiles(
        { shaderFile, shaderFile },
        Video::Simple3DVertex::Format()
    );

    auto shaderEvtRecv = std::make_shared<ShaderEventHandler>();
    shader->SetEventHandler(shaderEvtRecv);

    //shader->Compile(ShaderFlags::DisassemblyOutput | ShaderFlags::O3);
    //shader->Compile("-O3 -dis-asm -debug");
    shader->Compile();

    auto cbufferVertexParam = shader->CreateAndAttachConstantBuffer("vertexParam");

    shaderEvtRecv->renderer = renderSystem.get();
    shaderEvtRecv->cbufferVertexParam = cbufferVertexParam.get();

    #ifdef SIMPLE_SHADER_TEST

    Math::Vector4f vertexParam { 0, 1, 0, 1 };
    renderSystem->UpdateBuffer(cbufferVertexParam.get(), vertexParam);

    #endif

    // Create scene
    Scene::ForwardSceneRenderer sceneRenderer;
    Scene::FreeViewSceneNodeController cameraCtrl;

    Scene::SceneNode sceneGraph;
    Scene::SceneManager sceneMngr;
    Scene::CameraNode camera;

    camera.projection.SetViewport({ {}, screenSize.Cast<int>() });
    camera.projection.SetPlanes(0.1f, 100.0f);

    const std::string modelPath = "../../../media/Models/";

    auto meshNode = Scene::LoadMesh(sceneMngr, sceneGraph, modelPath + "House1.3ds");
    meshNode->transform.SetPosition({ 0, 0, 5 });

    auto texGeometry = std::make_shared<Scene::TexturedGeometry>();
    texGeometry->actualGeometry = meshNode->geometry;
    meshNode->geometry = texGeometry;

    // Create blend state
    Video::BlendState::Description blendDesc;
    {
        blendDesc.srcBlend = Video::BlendState::BlendOps::One;
        blendDesc.destBlend = Video::BlendState::BlendOps::Zero;
        blendDesc.srcBlendAlpha = Video::BlendState::BlendOps::One;
        blendDesc.destBlendAlpha = Video::BlendState::BlendOps::Zero;
        blendDesc.colorMask = { true, false, false, true };
    }
    auto blendState = renderSystem->CreateBlendState(blendDesc);

    // Create rasterizer state
    Video::RasterizerState::Description rasterDesc;
    {
        rasterDesc.fillMode = Video::RasterizerState::FillModes::Solid;
        rasterDesc.isAntiAliasedLineEnable = true;
        rasterDesc.isMultiSampleEnable = true;
    }
    auto rasterState = renderSystem->CreateRasterizerState(rasterDesc);

    // Create depth-stencil state
    Video::DepthStencilState::Description depthDesc;
    {
        depthDesc.isDepthTestEnabled = true;
    }
    auto depthState = renderSystem->CreateDepthStencilState(depthDesc);

    // Create sampler state
    Video::SamplerState::Description samplerDesc;
    {
        //samplerDesc.useMIPMaps = false;
        //samplerDesc.anisotropy = 16;
    }
    auto samplerState = renderSystem->CreateSamplerState(samplerDesc);
    renderContext->BindSamplerState(samplerState.get());

    // Load texture
    auto diffuseMap = renderSystem->GetTextureManager()->LoadTexture2D(modelPath + "House1.jpg");

    texGeometry->textures.push_back(diffuseMap);

    // Animation test
    #define ANIM_TEST
    #ifdef ANIM_TEST

    Anim::NodeAnimation anim;
    auto& keyframes = anim.keyframeSequence;

    keyframes.positionKeyframes.push_back({ 0, { 0, 0, 5 } });
    keyframes.positionKeyframes.push_back({ 1, { 2, 0, 5 } });
    keyframes.positionKeyframes.push_back({ 5, { 2, 0, 3 } });

    keyframes.rotationKeyframes.push_back({ 2, Math::Quaternionf() });
    keyframes.rotationKeyframes.push_back({ 3, Math::Quaternionf(Math::Vector3f(15.0f)) });
    keyframes.rotationKeyframes.push_back({ 4, Math::Quaternionf(Math::Vector3f(26.0f)) });
    keyframes.rotationKeyframes.push_back({ 5, Math::Quaternionf() });

    keyframes.BuildKeyframes();

    auto playbackEvtHandler = std::make_shared<Anim::DefaultPlayback::ListLoop>();
    playbackEvtHandler->frames.insert(playbackEvtHandler->frames.end(), { 0, 3, 2, 5, 0, 5, 3, 1, 4 });

    //anim.playback.Play(0, 5, 1.0f, std::make_shared<Anim::DefaultPlayback::Loop>());
    anim.playback.Play(0, 5, 1.0f, playbackEvtHandler);
    anim.transform = &(meshNode->transform);

    #endif

    // Terminal
    Util::Terminal term(primRenderer);
    term.GetView()->groundColor.a = 0.8f;

    // Main loop
    while (frame->ReceiveEvents() && !KEY_DOWN(Escape))
    {
        // Update scene
        if (KEY_HIT(F3))
            term.Enable(!term.IsEnabled());

        #ifdef ANIM_TEST

        //anim.Update();
        anim.playback.Update();
        anim.keyframeSequence.Interpolate(
            *anim.transform,
            anim.playback.frame,
            anim.playback.nextFrame,
            Math::SmootherStep(static_cast<float>(anim.playback.interpolator))
        );

        auto speedChange = IO::Mouse::Instance()->GetWheelMotion();
        if (speedChange)
        {
            anim.playback.speed += (0.1 * speedChange);
            IO::Log::Message("anim.playback.speed = " + ToStr(anim.playback.speed));
        }

        if (KEY_HIT(Return))
            anim.playback.speed = -anim.playback.speed;

        #endif

        // Draw scene
        renderContext->SetupViewport(camera.projection.GetViewport());

        renderContext->ChangeState(rasterState.get());
        renderContext->ChangeState(blendState.get());
        renderContext->ChangeState(depthState.get());

        renderContext->ClearBuffers();
        {
            #ifdef SIMPLE_SHADER_TEST
                
            renderContext->SetupDrawMode(Video::GeometryPrimitives::TriangleStrip);

            renderContext->BindShader(shader.get());
            renderContext->UnbindVertexBuffer();
            //renderContext->BindVertexBuffer(renderSystem->GetEmptyVertexBuffer());
            //renderContext->BindVertexBuffer(vertexBuffer.get());

            renderContext->Draw(3);
                
            #else

            if (MOUSE_DOWN(Right))
                cameraCtrl.Process(&camera);

            camera.UpdateView();

            //#   define DRAWING_TEST
            #   ifndef DRAWING_TEST
                
            // Draw scene
            renderContext->BindShader(shader.get());
            sceneRenderer.RenderSceneFromCamera(&sceneGraph, camera);

            #   else

            renderContext->ChangeState(renderSystem->GetDefaultBlendState());

            // Draw menu
            primRenderer->BeginDrawing2D();
            {
                primRenderer->Setup2DProjection();
                    
                primRenderer->DrawText2D(font.get(), { 15, 15 }, "Hello, World!");
                primRenderer->DrawText2D(font.get(), { 15, 45 }, "1234567890.:,;-_#'@");
                primRenderer->DrawText2D(font.get(), { 15, 75 }, "abcdefghijklmnopqrstuvwxyz");
                primRenderer->DrawText2D(font.get(), { 15, 105 }, "ABCDEFGHIJKLMNOPQRSTUJVWXYZ");

                primRenderer->DrawLine(Math::Line2f{ { 15, 150 }, { 300, 300 } });
                primRenderer->DrawRect({ Math::Point2f{ 400, 150 }, Math::Size2f{ 100, 100 } }, { 0, 1, 0, 1 });

                primRenderer->DrawImage(font->GetTexture(), Math::Point2f{ 15, 300 });//, Math::Size2f{ 100, 100 });

                // Draw terminal
                if (term.IsEnabled())
                    term.Draw();
            }
            primRenderer->EndDrawing2D();

            #   endif

            #endif
        }
        renderContext->Present();

        engine.UpdateFrameStates();
    }
}


