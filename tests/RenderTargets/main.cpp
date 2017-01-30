
// ForkENGINE: RenderTargets Test
// 25/04/2014

#include <fengine/import.h>
#include <fengine/using.h>

using namespace Fork;

FORK_IMPLEMENT_APP_MAIN_ONLY(RenderTargetsTest);

void RenderTargetsTest::OnMain()
{
    auto renderSystem = engine.CreateRenderSystem(L"ForkRendererGL");

    Video::ContextDescription contextDesc { true, true, 8 };
    contextDesc.rendererProfileDesc.useExtProfile = true;
    contextDesc.rendererProfileDesc.useGLCoreProfile = true;

    const Math::Size2ui screenSize { 800, 600 };

    auto renderContext = renderSystem->CreateRenderContext(
        Video::VideoMode(screenSize), contextDesc, L"RenderTargets Test"
    );

    auto rendererSupport = renderContext->QueryRendererSupport();

    auto primRenderer = engine.QueryPrimitiveRenderer(renderSystem.get());

    auto frame = renderContext->GetFrame();

    // Get input devices
    auto keyboard = IO::Keyboard::Instance();
    auto mouse = IO::Mouse::Instance();

    //#define MULTI_RT_TEST
    #ifdef MULTI_RT_TEST
    auto mrtShader = renderSystem->GetCommonShaderManager()->LoadShaderComposition(
        { "RenderTargetVertexShader.glsl",
            "RenderTargetPixelShader.glsl" },
        {}
    );
    #endif

    // Create render target texture
    const Math::Size2i texSize { 256 };
    auto renderTargetTex1 = renderSystem->CreateTexture2D(Video::TextureFormats::RGBA, texSize);//, 4);
    renderContext->GenerateMIPMaps(renderTargetTex1.get());

    #ifdef MULTI_RT_TEST
    auto renderTargetTex2 = renderSystem->CreateTexture2D(Video::TextureFormats::RGBA, texSize);//, 4);
    #endif

    // Create render target
    auto renderTarget = renderSystem->CreateRenderTarget();

    // Draw render targets
    renderContext->SetupClearColor({ 0, 0, 0.5f, 1 });

    Scene::Projection renderTargetProjection;
    renderTargetProjection.SetViewport({ {}, renderTargetTex1->GetSize() });

    const Video::ColorRGBAf colors[]
    {
        { 1, 0, 0, 1 },
        { 0, 1, 0, 1 },
        { 0, 0, 1, 1 },
        { 1, 1, 0, 1 }
    };

    renderTarget->SetupMultiSamples(8);

    //for (unsigned int i = 0; i < 4; ++i)
    unsigned int i = 0;
    {
        renderTarget->Attach(renderTargetTex1, 0, i);
        #ifdef MULTI_RT_TEST
        renderTarget->Attach(renderTargetTex2, 1, i);
        #endif

        renderContext->BindRenderTarget(renderTarget.get());
            
        renderContext->ClearBuffers();

        renderContext->SetupProjectionMatrix(renderTargetProjection);
        renderContext->SetupViewMatrix({});

        renderContext->SetupViewport(renderTargetProjection.GetViewport());

        #ifdef MULTI_RT_TEST

        primRenderer->BeginDrawing2D();
        {
            primRenderer->Setup2DProjection(
                {}, renderTargetProjection.GetViewport().size.Cast<float>()
            );

            renderContext->BindVertexBuffer(renderSystem->GetEmptyVertexBuffer());
            renderContext->BindShader(mrtShader.get());
            renderContext->Draw(3);
        }
        primRenderer->EndDrawing2D();

        #else

        primRenderer->BeginDrawing3D();
        {
            primRenderer->Setup3DProjection();
            primRenderer->DrawLine(
                Math::Line4f { { -2, 0, 3, 1 }, { 2, 1, 3, 1 } },
                colors[i]
            );
        }
        primRenderer->EndDrawing3D();

        primRenderer->BeginDrawing2D();
        {
            primRenderer->Setup2DProjection(
                {}, renderTargetProjection.GetViewport().size.Cast<float>()
            );
            primRenderer->DrawRect({ 15, 15, 50, 50 }, { 0, 1, 0, 1 });
        }
        primRenderer->EndDrawing2D();

        #endif
            
        renderContext->BindRenderTarget(renderContext->GetMainRenderTarget());
    }

    // Create texture to copy data in
    auto texA = renderSystem->CreateTexture2D(Video::TextureFormats::RGBA, { 256, 256 });
    renderContext->GenerateMIPMaps(texA.get());

    renderContext->SetupClearColor({ 0.8f, 0.8f, 0.8f, 1 });

    // Main loop
    while (frame->ReceiveEvents() && !keyboard->KeyDown(IO::KeyCodes::KeyEscape))
    {
        // Draw scene
        renderContext->ClearBuffers();
        {
            renderContext->SetupViewport({ {}, renderContext->GetVideoMode().resolution.Cast<int>() });

            renderContext->ChangeState(renderSystem->GetDefaultRasterizerState());

            primRenderer->BeginDrawing2D();
            {
                primRenderer->Setup2DProjection();

                primRenderer->DrawImage(renderTargetTex1.get(), { 15, 15 });
                #ifdef MULTI_RT_TEST
                primRenderer->DrawImage(renderTargetTex2.get(), { static_cast<float>(renderTargetTex1->GetSize().width + 30), 15 });
                #endif
                //primRenderer->DrawRect({ 15, 15, 200, 150 }, { 0, 1, 0, 1 });

                auto mousePos = mouse->GetFrameMousePosition().position.Cast<float>();
                primRenderer->DrawImage(texA.get(), { 15, 300 }, { mousePos.x - 15, mousePos.y - 300 });
            }
            primRenderer->EndDrawing2D();

            renderContext->CopyTextureImage(texA.get(), renderTarget.get(), { 0, 0, 128, 128 }, {}, 1);
        }
        renderContext->Present();

        engine.UpdateFrameStates();
    }

    Platform::NetworkInterface::CleanUp();
}

