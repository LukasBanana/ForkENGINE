
// ForkENGINE: Physics Test
// 18/04/2014

#include <fengine/import.h>

using namespace Fork;


// Global members

Video::ConstantBuffer* vertexParam = nullptr;

std::unique_ptr<Scene::SceneManager> sceneMngr;
std::unique_ptr<Scene::SceneNode> sceneGraph;


// Classes

class CustomSceneRenderer : public Scene::ForwardSceneRenderer
{
    
    public:
        
        void VisitGeometryNode(Scene::GeometryNode* node) override
        {
            auto renderSystem = Video::RenderSystem::Active();
            auto renderContext = Video::RenderContext::Active();

            renderContext->SetupWorldMatrix(node->transform.GetMatrix());

            if (vertexParam)
            {
                struct
                {
                    Math::Matrix4f wvpMatrix;
                    Math::Matrix4f worldMatrix;
                    Video::ColorRGBAf color;
                }
                param;
                {
                    param.wvpMatrix = renderContext->WVPMatrix();
                    param.worldMatrix = renderContext->GetRenderState().worldMatrix;

                    switch (node->metaData.userDataFlags)
                    {
                        case 1: param.color = Video::ColorRGBAf(0.7f, 1, 0.7f, 1); break;
                        case 2: param.color = Video::ColorRGBAf(1, 0.7f, 0.7f, 1); break;
                    }
                }
                renderSystem->WriteBuffer(vertexParam, &param, sizeof(param));

                node->metaData.userDataFlags = 0;
            }

            if (node->geometry)
                node->geometry->Visit(this);
        }

};

static Scene::GeometryNodePtr CreateCube(const Math::Vector3f& position = {})
{
    auto sceneNode = sceneMngr->CreateGeometryNode(sceneMngr->GenerateCube());
    sceneGraph->AddChild(sceneNode);
    sceneNode->transform.SetPosition(position);
    return sceneNode;
}

static Scene::GeometryNodePtr LoadMesh(const std::string& filename, const Math::Vector3f& position = {})
{
    auto sceneNode = Scene::LoadMesh(sceneMngr.get(), sceneGraph.get(), filename);
    sceneNode->transform.SetPosition(position);
    return sceneNode;
}

static void CleanUp()
{
    sceneMngr = nullptr;
    sceneGraph = nullptr;
}


// Main function

int main()
{
    FORK_MATCH_SDK_VERSION

    IO::Log::AddDefaultEventHandler();

    Engine::EngineDevice engine;

    CustomSceneRenderer sceneRenderer;

    try
    {
        auto renderSystem = engine.CreateRenderSystem(L"ForkRendererGL");

        Video::ContextDescription contextDesc(true, true, 8);
        contextDesc.rendererProfileDesc.useExtProfile = true;
        contextDesc.rendererProfileDesc.useGLCoreProfile = true;
        //contextDesc.rendererProfileDesc.versionGL = { 3, 2 };

        const Math::Size2ui screenSize(800, 600);

        auto renderContext = renderSystem->CreateRenderContext(
            Video::VideoMode(screenSize), contextDesc, L"Physics Test"
        );

        auto frame = renderContext->GetFrame();

        // Get input devices
        auto keyboard = IO::Keyboard::Instance();
        auto mouse = IO::Mouse::Instance();

        // Primitive renderer
        auto primRenderer = engine.QueryPrimitiveRenderer(renderSystem.get());
        auto font = primRenderer->CreateTextureFont({ "arial", 20 });

        // Create scene
        sceneMngr = std::make_unique<Scene::SceneManager>();
        sceneGraph = std::make_unique<Scene::SceneNode>();

        auto camera = sceneMngr->CreateCameraNode();
        
        camera->projection.SetViewport({ { 0, 0 }, screenSize.Cast<int>() });
        renderContext->SetupViewport(camera->projection.GetViewport());

        auto obj1 = CreateCube({ 0, 0, 3 });

        auto obj2 = LoadMesh("../../../media/Models/House2.3ds", { 5, -2, 3 });
        obj2->transform.SetRotation({ Math::pi*0.5f, 0, 0 });

        // Create physics system
        auto physicsSystem = engine.CreatePhysicsSystem(L"ForkPhysicsNw");

        auto world = physicsSystem->CreateWorld();
        auto material = world->CreateMaterial();
        //auto geometry = world->CreateBoxGeometry(Math::Size3f(2));
        
        auto collider = world->CreateMeshCollider(*obj2->geometry);
        auto obj2Body = world->CreateStaticBody(collider, obj2->transform.GetMatrix());

        // Create physics environment
        auto envModel = CreateCube();
        envModel->transform.SetScale({ 20, 1, 20 });
        auto envCollider = world->CreateBoxCollider({ 40, 2, 40 });
        auto envBody = world->CreateStaticBody(envCollider);
        envBody->userData = envModel.get();

        Scene::Transform envTransform;
        envModel->transform.SetPosition({ 0, -3, 0 });
        envBody->SetupTransform(envModel->transform);

        auto rigidModel = CreateCube();//{ 0, 1, 3 });
        auto rigidCollider = world->CreateBoxCollider({ 2, 2, 2 });
        auto rigidBody = world->CreateRigidBody(rigidCollider);
        rigidBody->userData = rigidModel.get();

        rigidBody->SetupMass(5);

        Scene::Transform rigidTransform;
        rigidTransform.SetPosition({ 0, 6, 10 });
        rigidBody->SetupTransform(rigidTransform);
        rigidBody->transform = &(rigidModel->transform);

        // Create shaders
        auto shaderComposition = renderSystem->GetShaderManager()->LoadShaderComposition(
            { "TestShader.glvert", "TestShader.glfrag" },
            Video::Simple3DVertex::Format()
        );
        vertexParam = shaderComposition->CreateAndAttachConstantBuffer("vertexParam").get();

        // Some scene node controllers
        Scene::FreeViewSceneNodeController camCtrl;

        class BodyPicker : public Physics::RigidBody::PickingEventHandler
        {
            
            public:
                
                void OnApplyForceAndTorque(Physics::RigidBody* rigidBody) override
                {
                    rigidBody->AddForce({ 0, -Physics::earthGravity, 0 });
                    Physics::RigidBody::PickingEventHandler::OnApplyForceAndTorque(rigidBody);
                }

        };

        auto bodyPicker = std::make_shared<BodyPicker>();
        bool isPicking = true;
        float pickLen = 0.0f;

        // Main loop
        while (frame->ReceiveEvents() && !keyboard->KeyDown(IO::KeyCodes::KeyEscape))
        {
            if (mouse->ButtonDown(IO::MouseKeyCodes::MouseRight))
                camCtrl.ProcessRotation(camera.get());
            camCtrl.ProcessMovement(camera.get());

            camera->UpdateView();

            // Physics simulation
            world->Simulate(2.0f/60.0f);

            // Rendering
            renderContext->ClearBuffers();
            {
                // Render scene
                renderContext->ChangeState(renderSystem->GetDefaultRasterizerState());
                renderContext->BindShader(shaderComposition.get());

                sceneRenderer.RenderSceneFromCamera(sceneGraph.get(), *camera);

                // Draw 3D
                primRenderer->BeginDrawing3D();
                {
                    primRenderer->Setup3DProjection(camera->GetViewMatrix());

                    // Make intersection test
                    auto mousePos = IO::Mouse::Instance()->GetFrameMousePosition().position.Cast<float>();
                    auto pickLine = Math::Line3f(camera->ViewRay(mousePos), 100.0f);

                    renderContext->SetupPointSize(10);

                    if (mouse->ButtonHit(IO::MouseKeyCodes::MouseMiddle))
                    {
                        rigidBody->SetupVelocity({});
                        rigidBody->SetupTransform(rigidTransform.NonScaledMatrix());
                    }

                    #if 0

                    Math::Intersection3f intersection;
                    if (geometry->RayCast(obj2->transform.GetMatrix(), pickLine, intersection))
                    {
                        primRenderer->DrawPoint(
                            Math::Point4f(intersection.point),
                            { 0, 1, 0 }
                        );

                        #if 1//!!!
                        obj1->transform.SetPosition(intersection.point);
                        obj1->transform.SetScale(Math::Vector3f(0.1f));
                        #endif
                    }

                    #else

                    auto pickList = world->RayCast(pickLine);

                    int i = 0;

                    for (auto& pickEntry : pickList)
                    {
                        auto sceneNode = reinterpret_cast<Scene::GeometryNode*>(pickEntry.body->userData);
                        
                        if (sceneNode)
                            sceneNode->metaData.userDataFlags = ++i;

                        primRenderer->DrawPoint(
                            Math::Point4f(pickEntry.point),
                            { 0, 1, 0 }
                        );

                        primRenderer->DrawLine(
                            { Math::Point4f(pickEntry.point), Math::Point4f(pickEntry.point + pickEntry.normal) }
                        );

                        if (pickEntry.body == rigidBody.get() && mouse->ButtonHit(IO::MouseKeyCodes::MouseLeft))
                        {
                            pickLen = Math::Distance(pickLine.start, pickEntry.point);
                            rigidBody->SetEventHandler(bodyPicker);
                            bodyPicker->Init(pickEntry.point, rigidBody.get());
                            isPicking = true;
                        }
                    }

                    if (isPicking)
                    {
                        auto pickPoint = camera->ViewRay(mousePos).Lerp(pickLen);

                        bodyPicker->Locate(pickPoint);

                        if (!mouse->ButtonDown(IO::MouseKeyCodes::MouseLeft))
                        {
                            isPicking = false;
                            rigidBody->SetEventHandler(physicsSystem->GetStdRigidBodyEventHandler());
                        }
                    }

                    #endif

                    primRenderer->DrawPoint(
                        { 0, 0, 0, 1 },
                        { 1, 0, 0 }
                    );
                }
                primRenderer->EndDrawing3D();
            }
            renderContext->Present();

            engine.UpdateFrameStates();
        }

        CleanUp();
    }
    catch (const DefaultException& err)
    {
        IO::Log::Error(err);
        IO::Console::Wait();
        CleanUp();
    }
    catch (const std::exception& err)
    {
        IO::Log::Error(err.what());
        IO::Console::Wait();
        CleanUp();
    }

    return 0;
}


