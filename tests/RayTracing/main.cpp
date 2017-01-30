
// ForkENGINE: RayTracing Test
// 18/12/2014

#include <fengine/import.h>
#include <fengine/helper.h>

using namespace Fork;

/* --- <App Configuration> --- */

#define HAS_ANIMATIONS
//#define PANORAMA_VIEW
#define COMBINE_WITH_RASTERIZER

/* --- </App Configuration> --- */

class RayTracingTest : public Engine::SimpleApp
{
    
    public:
        
        void OnInit() override;
        void OnUpdate() override;
        void OnRender() override;

    private:
        
        struct Sphere
        {
            Math::Sphere<float> sphere;
            Video::ColorRGBAf   color;
            float               reflectivity;
        };

        struct Plane
        {
            Math::Plane<float>  plane;
            float               reflectivity;
        };

        void AddLight(const Math::Point3f& position, const Video::ColorRGBf& color);
        void AddSphere(const Math::Sphere<float>& sphere, const Video::ColorRGBAf& color, float reflectivity = 0.35f);
        void AddPlane(const Math::Plane<float>& plane, float reflectivity = 0.0f);

        void CreateScene();

        void UpdateLights();
        void UpdateScene();
        void UpdateAnimations();

        Video::ShaderCompositionPtr rayTracingShader;
        Video::ConstantBufferPtr pixelParam, lightParam, sceneParam;

        Video::Texture2DPtr colorMap;

        Scene::FreeViewSceneNodeController cameraCtrl;

        std::vector<Scene::LightNode*> lights;
        std::vector<Sphere> spheres;
        std::vector<Plane> planes;

        bool isCameraAnim = false;
        Anim::NodeAnimation cameraTrackAnim;

        bool enableMovement = false;

        const size_t numAnimatedSpheres = 20;

};

FORK_IMPLEMENT_APP(RayTracingTest);

class SimpleMouseEventHandler : public IO::Mouse::EventHandler
{

    public:
        
        SimpleMouseEventHandler(bool& enableMovement) :
            enableMovement{ enableMovement }
        {
        }

        bool OnButtonDown(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame) override
        {
            if (keyCode == IO::MouseKeyCodes::MouseLeft || keyCode == IO::MouseKeyCodes::MouseRight)
            {
                enableMovement = true;
                LockFocus();
                Platform::Cursor::Show(false);
            }
            return true;
        }

        bool OnButtonUp(const IO::MouseKeyCodes keyCode, const Platform::Frame* frame) override
        {
            if (keyCode == IO::MouseKeyCodes::MouseLeft || keyCode == IO::MouseKeyCodes::MouseRight)
            {
                enableMovement = false;
                UnlockFocus();
                Platform::Cursor::Show(true);
            }
            return true;
        }

    private:
        
        bool& enableMovement;

};

void RayTracingTest::OnInit()
{
    /* Initial game engine */
    #ifdef PANORAMA_VIEW
    const Math::Size2ui screenSize(1900, 560);
    #else
    const Math::Size2ui screenSize(800, 600);
    #endif

    InitEngine(L"ForkENGINE RayTracing Test", screenSize);

    cameraTrackAnim.transform = &(mainCamera.transform);

    mouse->AddEventHandler(std::make_shared<SimpleMouseEventHandler>(enableMovement));

    /* Create ray-tracing shader */
    rayTracingShader = renderSystem->GetShaderManager()->LoadShaderComposition(
        { "RayTracing.glvert", "RayTracing.glfrag" },
        { /* empty vertex format */ }
    );

    pixelParam = rayTracingShader->CreateAndAttachConstantBuffer("pixelParam");
    lightParam = rayTracingShader->CreateAndAttachConstantBuffer("lightParam");
    sceneParam = rayTracingShader->CreateAndAttachConstantBuffer("sceneParam");

    /* Load textures */
    const std::string texPath = "../../../media/Textures/";

    colorMap = renderSystem->GetTextureManager()->LoadTexture2D(texPath + "WoodFloor1.jpg");

    renderContext->BindTexture(colorMap.get());

    /* Create scene */
    CreateScene();
}

void RayTracingTest::OnUpdate()
{
    /* Update user input */
    if (MOUSE_HIT(Middle))
    {
        static size_t animFrame;
        cameraTrackAnim.keyframeSequence.AddTransform(animFrame++, mainCamera.transform);
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

    /* Update camera movement */
    if (isCameraAnim)
        cameraTrackAnim.Update(engine.GetGlobalTimer()->GetDeltaTime());
    else if (enableMovement)
        cameraCtrl.Process(&mainCamera);

    #ifdef HAS_ANIMATIONS
    /* Update scene animations */
    UpdateAnimations();
    #endif
}

struct PixelParam
{
    Math::Matrix4f      viewTransform;
    Math::Matrix4f      invProjection;
    Math::Matrix4f      viewProjection;
    Math::Point4f       viewPosition;
    Video::ColorRGBAf   ambientColor;
    Video::ColorRGBAf   specularColor;
};

void RayTracingTest::OnRender()
{
    /* Update constant buffer */
    PixelParam fragParam;
    {
        fragParam.viewTransform     = mainCamera.transform.GetMatrix();
        fragParam.invProjection     = mainCamera.projection.GetMatrixLH().Inverse();
        fragParam.viewProjection    = mainCamera.projection.GetMatrixLH() * fragParam.viewTransform.Inverse(),
        fragParam.viewPosition      = Math::Point4f(mainCamera.transform.GetPosition());
        fragParam.ambientColor      = { 0.0f, 0.0f, 0.0f, 0.0f };//{ 0.2f, 0.2f, 0.2f, 0.0f };
        fragParam.specularColor     = { 1.0f, 1.0f, 1.0f, 1.0f };
    };
    renderSystem->UpdateBuffer(pixelParam.get(), fragParam);

    /* Bind shader and vertex buffer */
    renderContext->BindVertexBuffer(renderSystem->GetEmptyVertexBuffer());
    renderContext->BindShader(rayTracingShader.get());

    /* Draw ray-tracing scene */
    renderContext->Draw(3);

    #ifdef COMBINE_WITH_RASTERIZER

    /* Draw rasterizer scene */
    mainCamera.UpdateView();
    GetSceneRenderer()->RenderSceneFromCamera(&sceneGraph, mainCamera);

    #endif
}

void RayTracingTest::AddLight(const Math::Point3f& position, const Video::ColorRGBf& color)
{
    /* Create light source */
    auto lightSource = std::make_shared<Scene::PointLightSource>();
    lightSource->color = color;

    /* Create light node */
    auto lightNode = sceneManager.CreateLightNode(lightSource);
    lightNode->transform.SetPosition(position);

    /* Add light node to scene graph and reference list */
    sceneGraph.AddChild(lightNode);
    lights.push_back(lightNode.get());
}

void RayTracingTest::AddSphere(const Math::Sphere<float>& sphere, const Video::ColorRGBAf& color, float reflectivity)
{
    spheres.push_back({ sphere, color, reflectivity });
}

void RayTracingTest::AddPlane(const Math::Plane<float>& plane, float reflectivity)
{
    planes.push_back({ plane, reflectivity });
}

void RayTracingTest::CreateScene()
{
    /* Create lights */
    AddLight({ -1, 2, 0 }, {});
    //AddLight({ 5, 2, 4 }, { 1.0f, 0.7f, 0.2f });

    /* Create scene geometries */
    #ifdef HAS_ANIMATIONS
    
    const std::vector<Video::ColorRGBf> colors
    {
        { 1.0f, 0.3f, 0.2f },
        { 0.1f, 1.0f, 0.4f },
        { 0.8f, 0.7f, 0.3f },
        { 0.1f, 0.5f, 0.8f },
        { 0.8f, 0.1f, 0.9f },
        { 0.2f, 0.9f, 0.6f },
        { 0.7f, 0.6f, 0.4f },
    };

    spheres.resize(numAnimatedSpheres);
    
    size_t colorIndex = 0;
    for (auto& obj : spheres)
    {
        obj.reflectivity    = 0.35f;
        obj.color           = colors[colorIndex % colors.size()];
        ++colorIndex;
    }

    #endif

    AddSphere({ 1.0f, { 0, 0, 4 } }, { 1.0f, 0.7f, 0.2f });
    AddSphere({ 1.5f, { 6, 1, 2 } }, { 0.3f, 1.0f, 0.4f });

    AddPlane({ { 0, 1, 0 }, -1.0f }, 0.3f);
    AddPlane({ { 1, 0, 0 }, -15.0f });
    AddPlane({ { -1, 0, 0 }, -15.0f });
    AddPlane({ { 0, 0, 1 }, -15.0f });
    AddPlane({ { 0, 0, -1 }, -15.0f });

    AddPlane({ { 1, -1, 0 }, -25.0f });
    AddPlane({ { -1, -1, 0 }, -25.0f });

    /* Finalize scene */
    UpdateLights();
    UpdateScene();

    #ifdef COMBINE_WITH_RASTERIZER

    #   if 1
    /* Add rasterizer geometries */
    auto cubeSceenNode = sceneManager.CreateGeometryNode(sceneManager.GenerateCube());
    cubeSceenNode->transform.SetPosition({ -1, -0.5f, 3 });
    sceneGraph.AddChild(cubeSceenNode);
    #   else
    auto model = Scene::LoadMesh(sceneManager, sceneGraph, "../../../media/Models/BlenderMonkeySmall.obj");
    model->transform.SetPosition({ -2, -0.5f, 3.5f });
    model->transform.SetRotation({ Math::pi*0.2f, 0, 0 });
    #   endif

    #endif
}

#include "Core/PackPush.h"

struct LightParam
{
    Video::StandardShader::Simple3DMeshShader::LightConstBuffer::LightSource lights[4];
    int numLights;
}
PACK_STRUCT;

#include "Core/PackPop.h"

void RayTracingTest::UpdateLights()
{
    /* Update ray-tracing lights */
    LightParam lightBuffer;

    Scene::SimpleSceneRenderer::ConvertLightNodesToConstantBuffer(
        lights.begin(), lights.end(), lightBuffer.lights, 4, lightBuffer.numLights
    );

    renderSystem->UpdateBuffer(lightParam.get(), lightBuffer);

    #ifdef COMBINE_WITH_RASTERIZER

    /* Update rasterizer lights */
    GetSceneRenderer()->UpdateLightNodes(lights);

    #endif
}

#include "Core/PackPush.h"

static const int maxNumSpheres  = 32;
static const int maxNumPlanes   = 8;

struct SceneParam
{
    Math::Vector4f      spheres[maxNumSpheres];
    Video::ColorRGBAf   sphereColors[maxNumSpheres];
    Math::Vector4f      planes[maxNumPlanes];
    Math::Vector4f      geometryReflectivites[maxNumSpheres];
    int                 numSpheres;
    int                 numPlanes;
}
PACK_STRUCT;

#include "Core/PackPop.h"

void RayTracingTest::UpdateScene()
{
    SceneParam sceneBuffer;

    /* Convert spheres into buffer */
    int i = 0;
    for (const auto& obj : spheres)
    {
        sceneBuffer.spheres[i] = Math::Vector4f(obj.sphere.point, obj.sphere.radius);
        sceneBuffer.sphereColors[i] = obj.color;
        sceneBuffer.geometryReflectivites[i].x = obj.reflectivity;
        ++i;
        if (i >= maxNumSpheres)
            break;
    }

    sceneBuffer.numSpheres = i;

    /* Convert planes into buffer */
    i = 0;
    for (const auto& obj : planes)
    {
        sceneBuffer.planes[i] = Math::Vector4f(obj.plane.normal, obj.plane.distance);
        sceneBuffer.geometryReflectivites[i].y = obj.reflectivity;
        ++i;
        if (i >= maxNumPlanes)
            break;
    }

    sceneBuffer.numPlanes = i;

    renderSystem->UpdateBuffer(sceneParam.get(), sceneBuffer);
}

void RayTracingTest::UpdateAnimations()
{
    static float animationTime;

    animationTime += 0.0025f;
    const float radius = 10.0f;

    for (size_t i = 0; i < numAnimatedSpheres; ++i)
    {
        auto& obj = spheres[i];

        auto angle          = (static_cast<float>(i) / static_cast<float>(numAnimatedSpheres)) * Math::pi * 2.0f;
        auto anglePos       = animationTime + angle;
        auto angleRadius    = animationTime*7.0f + angle;

        obj.sphere.point    = { std::sin(anglePos)*radius, 0.2f, std::cos(anglePos)*radius };
        obj.sphere.radius   = 0.5f + std::sin(-angleRadius)*0.2f;
    }

    UpdateScene();
}

