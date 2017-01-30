
// ForkENGINE: CubeMap Test
// 23/08/2014

#include <fengine/import.h>
#include <fengine/helper.h>

using namespace Fork;

Video::RenderSystem*        renderer            = nullptr;
Video::RenderContext*       context             = nullptr;
Video::ConstantBuffer*      vertexConstBuffer   = nullptr;
Video::ConstantBuffer*      pixelConstBuffer    = nullptr;
Video::ShaderComposition*   shader              = nullptr;

using namespace Video::StandardShader;

class ShaderEventHandler : public Video::ShaderComposition::EventHandler
{

    public:
                
        void OnUpdateConstantBuffers(const Video::ShaderComposition* shaderComposition, Video::RenderContext* renderContext)
        {
            // Update vertex constant buffer
            Simple3DMeshShader::VertexConstBuffer vertexParam
            {
                renderContext->WVPMatrix(),
                renderContext->GetRenderState().worldMatrix,
                renderContext->GetRenderState().cameraMatrix.GetPosition()
            };
            renderer->UpdateBuffer(vertexConstBuffer, vertexParam);

            // Update pixel constant buffer
            struct
            {
                Math::Matrix4f invViewMatrix;
                Math::Vector4f viewDir;
            }
            pixelParam
            {
                {},//renderContext->GetRenderState().viewMatrix.Inverse(),
                Math::Vector4f(renderContext->GetRenderState().cameraMatrix.GetPosition())
            };
            renderer->UpdateBuffer(pixelConstBuffer, pixelParam);
        }

};

FORK_IMPLEMENT_APP_MAIN_ONLY(CubeMapTest);

void CubeMapTest::OnMain()
{
    auto renderSystem = engine.CreateRenderSystem();//L"ForkRendererD3D11");
    renderer = renderSystem.get();

    const Math::Size2ui screenSize { 800, 600 };

    auto renderContext = renderSystem->CreateRenderContext(
        Video::VideoMode(screenSize), { true, true, 8 }, L"CubeMap Test"
    );
    context = renderContext.get();

    renderContext->SetupClearColor({});

    auto frame = renderContext->GetFrame();

    // Create render target texture
    const Math::Size2i texSize { 256 };
    
    const std::string mediaPath = "../../../media/";

    const auto cubePath = mediaPath + "Textures/SkyboxSunny/";
    #define CUBEMAP_TEX(n) (cubePath + ("TropicalSunnyDay" #n "1024.png"))

    #define DYNAMIC_CUBEMAP
    #ifndef DYNAMIC_CUBEMAP
    auto cubeMap = renderSystem->GetTextureManager()->CreateTextureCubeFromFiles(
        { CUBEMAP_TEX( Left  ), CUBEMAP_TEX( Up   ), CUBEMAP_TEX( Front ),
          CUBEMAP_TEX( Right ), CUBEMAP_TEX( Down ), CUBEMAP_TEX( Back  ) }
    );
    #else
    auto cubeMap = renderSystem->CreateTextureCube(Video::TextureFormats::RGBA8UInt, { 512, 512 });
    #endif

    // Setup dynamic cube map
    auto cubeRTs = renderSystem->CreateCubeRenderTargets(cubeMap);

    const Video::TextureCube::FaceRenderTargetRefs cubeFaces
    {
        cubeRTs[0].get(), cubeRTs[1].get(),
        cubeRTs[2].get(), cubeRTs[3].get(),
        cubeRTs[4].get(), cubeRTs[5].get()
    };

    Scene::CubeMapRenderer cubeMapRenderer;
    cubeMapRenderer.projection.SetNearPlane(0.1f);

    // Create render target
    auto renderTarget = renderSystem->CreateRenderTarget();

    // Create scene
    Scene::SceneManager sceneMngr;
    Scene::SceneNode sceneGraph, sceneGraphReflect;

    Scene::SimpleSceneRenderer sceneRenderer;
    Scene::FreeViewSceneNodeController cameraCtrl;

    Scene::CameraNode camera;
    camera.projection.SetViewport({{}, screenSize.Cast<int>()});
    camera.projection.SetNearPlane(0.1f);
    camera.transform.SetPosition({ 0, 0, -3 });

    // Create textured sphere geometry
    auto sphereGeom = sceneMngr.GenerateIcoSphere();
    auto cubeGeom = sceneMngr.GenerateCube();

    auto texGeom = std::make_shared<Scene::TexturedGeometry>();
    texGeom->actualGeometry = sphereGeom;
    texGeom->textures.push_back(cubeMap);

    auto sphereNode = sceneMngr.CreateGeometryNode(texGeom);
    sceneGraphReflect.AddChild(sphereNode);

    // Load model
    auto carMdl = Scene::LoadMesh(&sceneMngr, &sceneGraph, mediaPath + "Models/audi.3ds");
    carMdl->transform.SetPosition({ -3, 0, 0 });
    carMdl->transform.SetScale({ 0.5f, 0.5f, 0.5f });
    carMdl->transform.SetRotation(Math::Quaternionf{ Math::pi*0.5f, 0, 0 });

    auto car2Mdl = std::make_shared<Scene::GeometryNode>();
    car2Mdl->geometry = carMdl->geometry;
    car2Mdl->transform.SetPosition({ 3, 0, 0 });
    car2Mdl->transform.SetScale({ 0.5f, 0.5f, 0.5f });
    car2Mdl->transform.SetRotation(Math::Quaternionf{ Math::pi*0.5f, Math::pi, 0 });
    sceneGraph.AddChild(car2Mdl);

    auto sphereMdl = std::make_shared<Scene::GeometryNode>();
    sphereMdl->geometry = sphereGeom;
    sphereMdl->transform.SetPosition({ 2, 4, 5 });
    sceneGraph.AddChild(sphereMdl);

    auto cubeMdl = std::make_shared<Scene::GeometryNode>();
    cubeMdl->geometry = cubeGeom;
    cubeMdl->transform.SetPosition({ -1, 1, -6 });
    sceneGraph.AddChild(cubeMdl);

    auto cube2Mdl = std::make_shared<Scene::GeometryNode>();
    cube2Mdl->geometry = cubeGeom;
    cube2Mdl->transform.SetPosition({ 1, 6, 1 });
    cube2Mdl->transform.SetRotation({ 0, 45.0f * Math::deg2rad, 0 });
    sceneGraph.AddChild(cube2Mdl);

    // Load shader
    std::vector<std::string> shaderFiles;

    if (renderContext->ShadingLanguageFileExt() == "glsl")
        shaderFiles = { "CubeMapVertexShader.glsl", "CubeMapPixelShader.glsl" };
    else
        shaderFiles = { "CubeMapShader.hlsl", "CubeMapShader.hlsl" };

    sphereNode->shaderComposition = renderSystem->GetShaderManager()->LoadShaderComposition(
        shaderFiles, Video::Simple3DVertex::Format()
    );

    vertexConstBuffer = sphereNode->shaderComposition->CreateAndAttachConstantBuffer("vertexParam").get();
    pixelConstBuffer = sphereNode->shaderComposition->CreateAndAttachConstantBuffer("pixelParam").get();

    sphereNode->shaderComposition->SetEventHandler(std::make_shared<ShaderEventHandler>());

    // Setup lighting
    Scene::LightNode lightNode;
    auto pointLight = std::make_shared<Scene::PointLightSource>();
    lightNode.lightSource = pointLight;
    lightNode.transform.SetPosition({ -1, 1, -3 });

    sceneRenderer.UpdateLightNodes({ &lightNode });

    // Main loop
    while (frame->ReceiveEvents() && !KEY_DOWN(Escape))
    {
        // Update scene
        if (MOUSE_DOWN(Right))
            cameraCtrl.Process(&camera);

        camera.UpdateView();

        renderContext->ChangeState(renderSystem->GetDefaultRasterizerState());

        // Move reflective sphere
        static const float moveSpeed = 0.05f;
        
        if (KEY_DOWN(Left))
            sphereNode->transform.MoveGlobal({ -moveSpeed, 0, 0 });
        if (KEY_DOWN(Right))
            sphereNode->transform.MoveGlobal({ moveSpeed, 0, 0 });
        if (KEY_DOWN(Up))
            sphereNode->transform.MoveGlobal({ 0, 0, moveSpeed });
        if (KEY_DOWN(Down))
            sphereNode->transform.MoveGlobal({ 0, 0, -moveSpeed });

        #if defined(DYNAMIC_CUBEMAP)

        // Render cube map
        sphereNode->isEnabled = false;
        cubeMapRenderer.Begin();
        {
            cubeMapRenderer.RenderCubeMap(cubeFaces, &sceneRenderer, &sceneGraph, sphereNode->transform.GetPosition());
        }
        cubeMapRenderer.End();
        renderContext->GenerateMIPMaps(cubeMap.get());
        sphereNode->isEnabled = true;

        #elif 0

        // Draw render target
        renderTarget->Attach(cubeMap, Video::TextureCube::Faces::XPositive);
        renderContext->BindRenderTarget(renderTarget.get());
        {
            renderContext->ClearBuffers();
            renderContext->SetupViewport({ {}, cubeMap->GetSize() });

            sceneRenderer.RenderSceneFromCamera(&sceneGraph, camera);
        }
        renderContext->BindRenderTarget(renderContext->GetMainRenderTarget());
        renderTarget->Clear();

        #endif

        // Draw scene
        renderContext->ClearBuffers();
        {
            renderContext->SetupViewport(camera.projection.GetViewport());

            sceneRenderer.RenderSceneFromCamera(&sceneGraphReflect, camera);
            sceneRenderer.RenderSceneFromCamera(&sceneGraph, camera);
        }
        renderContext->Present();

        engine.UpdateFrameStates();
    }
}


