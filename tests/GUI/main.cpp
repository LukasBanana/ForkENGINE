
// ForkENGINE: GUI Test
// 17/11/2014

#include <fengine/import.h>
#include <fengine/helper.h>

using namespace Fork;

Video::RenderSystem*    renderer    = nullptr;
Video::RenderContext*   context     = nullptr;
GUI::Environment*       guiEnvRef   = nullptr;

GUI::Frame*             mainFrame   = nullptr;
GUI::ViewController*    lockFrame   = nullptr;

class GUITest : public Engine::SimpleApp
{
    
    public:
        
        void OnInit() override;
        void OnUpdate() override;
        void OnRender() override;

    private:
        
        std::unique_ptr<GUI::Environment> guiEnv;
        Video::TextureFontPtr font;

        Scene::GeometryNodePtr monitorMdl;
        Video::Texture2DPtr displayTexture;
        Video::RenderTargetPtr displayRenderTarget;
        Video::Texture2DPtr cursorImage;

        bool showScene = false;

};

FORK_IMPLEMENT_APP(GUITest);

class MouseEventHandler : public IO::Mouse::EventHandler
{
    
    public:
        
        bool OnButtonDown(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame) override
        {
            guiEnvRef->PostMouseKeyDown(keyCode);
            return true;
        }

        bool OnButtonUp(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame) override
        {
            guiEnvRef->PostMouseKeyUp(keyCode);
            return true;
        }

        bool OnLocalMotion(const Math::Point2i& position, const Platform::Frame* frame) override
        {
            guiEnvRef->SetupCursorPosition(position);
            return true;
        }

        bool OnWheelMotion(int motion, const Platform::Frame* frame)
        {
            guiEnvRef->PostWheelMotion(motion);
            return true;
        }

};

class FrameEventHandler : public GUI::Frame::ViewEventHandler
{
    
    public:
        
        FrameEventHandler(GUI::Frame* owner) :
            GUI::Frame::ViewEventHandler{ owner }
        {
        }

        void OnDraw(GUI::DrawEvent& event) override
        {
            GUI::Frame::ViewEventHandler::OnDraw(event);
            auto rect = event.GetOwner()->GlobalArea();
            event.GetContext()->DrawFontText(rect.LTPoint() + GUI::Point(10, 5), L"Custom Draw Event", { 0, 255, 0 });
        }

};

void GUITest::OnInit()
{
    InitEngine(L"ForkENGINE GUI Test");//, L"ForkRendererD3D11");

    mouse->AddEventHandler(std::make_shared<MouseEventHandler>());

    /* Create GUI environment */
    guiEnv = std::make_unique<GUI::Environment>(primitiveRenderer);
    guiEnvRef = guiEnv.get();

    auto widget0 = guiEnv->CreateFrame({ 100, 100 }, { 450, 350 }, L"Widget 1");
    auto widget1 = guiEnv->CreateFrame({ 200, 150 }, { 200, 150 }, L"Widget 1.1", widget0, 0);
    auto widget2 = guiEnv->CreateFrame({ 0, 0 }, { 200, 150 }, L"Widget 1.2", widget0);
    auto widget3 = guiEnv->CreateFrame({ 200, 50 }, { 200, 150 }, L"Widget 1.3", widget0);
    
    widget0->ChangeFlags(GUI::Frame::Flags::SizeBox | GUI::Frame::Flags::TitleBar | GUI::Frame::Flags::ScrollArea);
    widget0->SetEventHandler(std::make_shared<FrameEventHandler>(widget0));
    widget0->SetupVirtualSize({ 800, 400 });

    auto buttonCallback = [](GUI::Button* owner)
    {
        IO::Log::Message("Button " + ToStr(owner->GetID()) + " Pushed");
    };

    auto button0 = guiEnv->CreateButton({ 10, 10 }, { 80, 25 }, L"Push Me", widget1);
    button0->SetTriggerCallback(buttonCallback);

    auto button1 = guiEnv->CreateButton({ 50, 30 }, { 80, 25 }, L"LockChild", widget1);

    #if 1
    button1->SetTriggerCallback(buttonCallback);
    #else
    lockFrame = widget1;
    lockFrame->SetupParent(guiEnv->GetDesktop());
    button1->SetTriggerCallback(
        [](GUI::Button* owner)
        {
            static bool s; s = !s;
            if (s)
                mainFrame->LockChild(lockFrame);
            else
                mainFrame->UnlockChild(lockFrame);
        }
    );
    #endif

    widget3->Enable(false);
    
    widget2->ChangeFlags(GUI::Frame::Flags::SizeBox | GUI::Frame::Flags::TitleBar);
    widget2->MinResize({ 100, 100 });
    widget2->MaxResize({ 300, 300 });
    
    widget2->colorScheme.active.top = { 255, 255, 0 };
    widget2->colorScheme.active.bottom = { 255, 170, 0 };
    widget2->colorScheme.ground = { 255, 255, 255 };

    renderContext->SetupClearColor({ 1, 1, 1, 1 });

    mainFrame = widget0;

    /* Create render target */
    Math::Size2i displaySize(
        #if 0
        640, 360
        #else
        800, 450
        #endif
    );

    displayTexture = renderSystem->CreateTexture2D(Video::TextureFormats::RGB8UInt, displaySize);
    displayRenderTarget = renderSystem->CreateRenderTarget({ displayTexture });

    cursorImage = renderSystem->GetTextureManager()->LoadTexture2D("CursorImage.png");

    /* Create scene */
    monitorMdl = Scene::LoadMesh(sceneManager, sceneGraph, "LCD-Monitor.obj");
    
    mainCamera.transform.SetPosition({ -0.3f, 1.6f, -2.6f });
    mainCamera.transform.SetRotation({ Math::deg2rad*8.0f, Math::deg2rad*5.0f, 0 });

    auto light = sceneManager.CreateLightNode(std::make_shared<Scene::PointLightSource>());
    light->transform.SetPosition({ -1, 1, -2 });
    GetSceneRenderer()->UpdateLightNodes({ light.get() });

    auto compGeom = dynamic_cast<Scene::CompositionGeometry*>(monitorMdl->geometry.get());
    if (compGeom && !compGeom->subGeometries.empty())
    {
        auto subGeom = dynamic_cast<Scene::Simple3DMeshGeometry*>(compGeom->subGeometries.back().get());

        auto texGeom = std::make_shared<Scene::TexturedGeometry>();
        texGeom->actualGeometry = compGeom->subGeometries.back();
        texGeom->textures.push_back(displayTexture);

        compGeom->subGeometries.back() = texGeom;
    }
}

void GUITest::OnUpdate()
{
    guiEnv->Update();

    if (keyboard->KeyHit(IO::KeyCodes::KeySpace))
    {
        showScene = !showScene;
        Platform::Cursor::Show(!showScene);
    }
}

void GUITest::OnRender()
{
    //auto timer = engine.MakeLogScopedTimer("Framerate: ");

    monitorMdl->isEnabled = showScene;

    Engine::SimpleApp::OnRender();

    if (showScene)
    {
        guiEnv->GetDesktop()->Resize(displayTexture->GetSize());

        auto cursorPosition = mouse->GetFrameMousePosition().position.Cast<float>();

        renderContext->BindRenderTarget(displayRenderTarget.get());
        {
            renderContext->ClearBuffers();
            guiEnv->Draw();

            primitiveRenderer->BeginDrawing2D();
            {
                primitiveRenderer->DrawImage(cursorImage.get(), cursorPosition);
            }
            primitiveRenderer->EndDrawing2D();
        }
        renderContext->BindRenderTarget(renderContext->GetMainRenderTarget());

        renderContext->GenerateMIPMaps(displayTexture.get());
    }
    else
    {
        guiEnv->GetDesktop()->Resize(renderContext->Resolution().Cast<int>());
        guiEnv->Draw();
    }
}


