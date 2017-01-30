/*
 * ForkENGINE Tutorial 01 "Getting Started"
 * 26/10/2014
 */

// Import all engine relevant header files
#include <fengine/import.h>

// Use main namespace "Fork"
using namespace Fork;

/*
Main application class.
This inherits from "SimpleApp" which contains common engine devices
such as a renderer, main camera, scene graph etc.
*/
class TutorialApp : public Engine::SimpleApp
{
    
    public:
        
        void OnInit() override;
        void OnUpdate() override;

    private:
        
        void CreateScene();

        Math::Vector3f ArcBallVector(const Math::Point2i& mousePos) const;

        // Scene node to present in this tutorial
        Scene::GeometryNodePtr model;

        // Scene node for a light source.
        Scene::LightNodePtr light;

        // Presentation scene node controller.
        Scene::PresentSceneNodeController sceneNodeCtrl;

};

// This macro implements the application and the "main" function.
FORK_IMPLEMENT_APP(TutorialApp)

void TutorialApp::OnInit()
{
    /*
    Initialize engine device.
    This also creates a render system. By default L"ForkRendererGL".
    The default screen size is 800x600 pixels with 32 bit color depth.
    */
    //InitEngine(L"ForkENGINE Tutorial 01 Getting Started");
    EngineConfiguration config;
    config.useNormalMapping = true;
    InitEngine(L"ForkENGINE Tutorial 01 Getting Started", { 800, 600 }, false, 8u, true, config);

    // Create our 3D scene
    CreateScene();

    #if 0//TEST

    Math::Matrix4f mat;
    mat.Translate({ 1, 2, 3 });
    mat.RotateFree(15.0f*Math::deg2rad, { 1, 2, -3 });
    //mat.Scale({ 3, 2, -4 });

    IO::Log::Message("4x4 Matrix:");
    IO::Log::Message(IO::Printer::Print(mat.Inverse()));

    IO::Log::Message("Affine 4x3 Matrix:");
    IO::Log::Message(IO::Printer::Print(mat.InverseAffine()));

    int x=0;

    #endif
}

Math::Vector3f TutorialApp::ArcBallVector(const Math::Point2i& mousePos) const
{
    const auto& resolution = renderContext->GetVideoMode().resolution;

    auto point = Math::Vector3f(
          (2.0f * mousePos.x) / resolution.width  - 1.0f,
        -((2.0f * mousePos.y) / resolution.height - 1.0f),
        0.0f
    );

    auto lenSq = point.x*point.x + point.y*point.y;
    if (lenSq <= 1.0f)
        point.z = std::sqrt(1.0f - lenSq);
    else
        Math::Normalize(point);

    return point;
}

void TutorialApp::OnUpdate()
{
    #if 0

    static Math::Point2i prevMousePos;

    auto mousePos = mouse->GetFrameMousePosition().position;

    if (mousePos != prevMousePos && mouse->ButtonDown(IO::MouseKeyCodes::MouseLeft))
    {
        auto vecA = ArcBallVector(prevMousePos);
        auto vecB = ArcBallVector(mousePos);
        
        auto AdotB = Math::Dot(vecA, vecB);
        auto angle = std::acos(std::min(1.0f, AdotB));

        auto viewAxis = Math::Cross(vecA, vecB);

        auto cameraToObjectRotation = mainCamera.transform.GetRotation().Inverse() * model->transform.GetRotation();
        //auto objectAxis = cameraToObjectRotation * viewAxis;
        auto objectAxis = model->transform.GetMatrix().RotateVector(viewAxis);
        Math::Normalize(objectAxis);

        //Math::Matrix3f rotationMatrix = model->transform.GetRotation().Mat3();
        //rotationMatrix.RotateFree(angle, objectAxis);
        //model->transform.SetRotation(Math::Quaternionf(rotationMatrix));
        Math::Quaternionf rotation;
        rotation.SetupAngleAxis(angle, objectAxis);
        model->transform.SetRotation(model->transform.GetRotation() * rotation);
    }

    prevMousePos = mousePos;

    #else

    if (mouse->ButtonDown(IO::MouseKeyCodes::MouseLeft))
        sceneNodeCtrl.Process(model.get());

    #endif
}

void TutorialApp::CreateScene()
{
    /* --- Create 3D model --- */

    /*
    Create a geometry, here a 'cylinder' with radius 1.0 and height 2.0.
    Then set this to the scene node's geometry.
    */
    const Scene::ModelReader::Flags::DataType modelFlags = Scene::ModelReader::Flags::GenerateTangentSpace;// | Scene::ModelReader::Flags::OptimizeGeometryGraph;
    const std::string modelPath = "???";

    #if 0
    model = sceneManager.CreateGeometryNode();
    model->geometry = sceneManager.GenerateIcoSphere();
    sceneGraph.AddChild(model);
    #elif 0
    model = Scene::LoadMesh(sceneManager, sceneGraph, "../../../media/Models/audi.3ds", modelFlags);
    const float modelScale = 0.7f;
    #else
    model = Scene::LoadMesh(sceneManager, sceneGraph, modelPath + "beast.ms3d", modelFlags);
    const float modelScale = 0.02f;
    #endif

    #if 0
    auto geom = dynamic_cast<Scene::Simple3DMeshGeometry*>(model->geometry.get());
    if (geom)
        geom->GenerateNormalsFlat();
    #endif

    #if 1
    
    auto normalMap = renderSystem->GetTextureManager()->LoadTexture2D(
        "../../../media/Textures/TilesNormalMap.jpg"
    );

    auto diffuseMapBeast = renderSystem->GetTextureManager()->LoadTexture2D(modelPath + "beast1.jpg");
    auto normalMapBeast = renderSystem->GetTextureManager()->LoadTexture2D(modelPath + "beast1_NORM.png");

    #if 0
    Scene::FilterGeometry<Scene::TexturedGeometry>(
        model.get(),
        [&](Scene::TexturedGeometry* geom)
        {
            IO::Log::Message("< filter callback >");
            geom->textures.push_back(normalMapBeast);
        }
    );
    #endif

    #if 1

    auto geom = dynamic_cast<Scene::CompositionGeometry*>(model->geometry.get());
    if (geom)
    {
        for (auto& subGeom : geom->subGeometries)
        {
            auto subTexGeom = dynamic_cast<Scene::TexturedGeometry*>(subGeom.get());
            if (subTexGeom)
            {
                subTexGeom->textures.push_back(normalMap);
                #if 0
                auto actualGeom = dynamic_cast<Scene::TangentSpaceMeshGeometry*>(subTexGeom->actualGeometry.get());
                if (actualGeom)
                    actualGeom->GenerateTangentSpaceFlat();
                #endif
            }
            else
            {
                auto subTexGeom = std::make_shared<Scene::TexturedGeometry>();
                subTexGeom->textures.push_back(diffuseMapBeast);
                subTexGeom->textures.push_back(normalMapBeast);
                subTexGeom->actualGeometry = subGeom;
                subGeom = subTexGeom;
            }
        }
    }

    #endif

    Scene::GeometryConverter::OptimizeGeometryGraph(model->geometry);

    #endif

    // Locate the object at { X, Y, Z } = { 0, 0, 3 }
    model->transform.SetPosition({ 0, 0, 3 });
    model->transform.SetScale(Math::Vector3f(modelScale));

    /* --- Create light source --- */

    // Create light source meta data
    auto lightSource = std::make_shared<Scene::PointLightSource>();

    // Create light scene node
    light = sceneManager.CreateLightNode(lightSource);
    light->transform.SetPosition({ -2, 2, -2 });

    GetSceneRenderer()->UpdateLightNodes({ light.get() });
}



