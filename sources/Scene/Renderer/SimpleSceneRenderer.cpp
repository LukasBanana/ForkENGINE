/*
 * Simple scene renderer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Renderer/SimpleSceneRenderer.h"
#include "Video/RenderSystem/Shader/ShaderComposition.h"
#include "Video/RenderSystem/Shader/ShaderProvider.h"
#include "Video/BufferFormat/Simple3DVertex.h"
#include "Core/Exception/NullPointerException.h"
#include "Video/RenderSystem/Shader/StandardShader/Simple3DMeshConstantBuffers.h"
#include "Scene/Node/LightNode.h"
#include "Scene/LightSource/DirectionalLightSource.h"
#include "Scene/Geometry/Node/TexturedGeometry.h"
#include "../../Video/RenderSystem/RenderSysCtx.h"

#include <algorithm>


namespace Fork
{

namespace Scene
{


using namespace Video::StandardShader::Simple3DMeshShader;

SimpleSceneRenderer::SimpleSceneRenderer(bool useNormalMapping)
{
    CreateSceneShader(useNormalMapping);
    CreateEventHandler();
}

void SimpleSceneRenderer::RenderScene(SceneNode* sceneGraph)
{
    RenderCtx()->BindShader(sceneShader_.get());
    ForwardSceneRenderer::RenderScene(sceneGraph);
}

void SimpleSceneRenderer::VisitSimple3DMeshGeometry(Simple3DMeshGeometry* node)
{
    UpdateMaterial(numTexturesBound_, Video::ColorRGBAf(), 0.7f);
    ForwardSceneRenderer::VisitSimple3DMeshGeometry(node);
}

void SimpleSceneRenderer::VisitTangentSpaceMeshGeometry(TangentSpaceMeshGeometry* node)
{
    UpdateMaterial(numTexturesBound_, Video::ColorRGBAf(), 0.7f);
    ForwardSceneRenderer::VisitTangentSpaceMeshGeometry(node);
}

void SimpleSceneRenderer::VisitTexturedGeometry(TexturedGeometry* node)
{
    numTexturesBound_ = node->textures.size();
    ForwardSceneRenderer::VisitTexturedGeometry(node);
    numTexturesBound_ = 0;
}

void SimpleSceneRenderer::UpdateLightNodes(const std::vector<LightNode*>& lightNodes)
{
    LightConstBuffer lightBufferData;

    SimpleSceneRenderer::ConvertLightNodesToConstantBuffer(
        lightNodes.begin(),
        lightNodes.end(),
        lightBufferData.lights,
        LightConstBuffer::maxNumLights,
        lightBufferData.numLights
    );

    RenderSys()->UpdateBuffer(lightConstBuffer_.get(), lightBufferData);
}

void SimpleSceneRenderer::ConvertLightNodesToConstantBuffer(
    std::vector<LightNode*>::const_iterator begin,
    std::vector<LightNode*>::const_iterator end,
    LightConstBuffer::LightSource* lightBuffer,
    int maxNumLights, int& numLights)
{
    ASSERT_POINTER(lightBuffer);

    for (numLights = 0; ( begin != end && numLights < maxNumLights ); ++begin, ++numLights, ++lightBuffer)
    {
        /* Get light node */
        auto lightNode = *begin;
        if (!lightNode)
            continue;
        
        /* Get light source */
        auto lightSource = lightNode->lightSource.get();
        if (!lightSource)
            continue;
        
        /* Convert light source into constant buffer data */
        auto nodeTransform = lightNode->GlobalTransform();

        lightBuffer->positionAndType = Math::Vector4f(
            nodeTransform.GetPosition(),
            static_cast<float>(lightSource->Type())
        );
        lightBuffer->colorAndEnergy = Video::ColorRGBAf(lightSource->color, lightSource->energy);

        switch (lightSource->Type())
        {
            case Scene::LightSource::Types::Point:
            case Scene::LightSource::Types::Spot:
            {
                lightBuffer->direction = { 0, 0, 1, 1 };
            }
            break;

            case Scene::LightSource::Types::Directional:
            {
                auto lightDir = dynamic_cast<Scene::DirectionalLightSource*>(lightSource)->direction;
                lightBuffer->direction =  Math::Vector4f(nodeTransform.RotateVector(lightDir));
            }
            break;
        }
    }
}


/*
 * ======= Protected: =======
 */

void SimpleSceneRenderer::UpdateMaterial(size_t numTextures, const Video::ColorRGBAf& diffuseColor, float roughness)
{
    MaterialConstBuffer materialBufferData;
    {
        materialBufferData.ambientColor     = { 0.2f, 0.2f, 0.2f, 1.0f };
        materialBufferData.diffuseColor     = diffuseColor;
        materialBufferData.specularColor    = diffuseColor * (1.0f - roughness);
        materialBufferData.emissiveColor    = { 0.0f, 0.0f, 0.0f, 1.0f };
        materialBufferData.shininess        = Video::BlinnPhongMaterial::RoughnessToSpecularPower(roughness);
        materialBufferData.numTextures      = static_cast<int>(numTextures);
    };
    RenderSys()->UpdateBuffer(materialConstBuffer_.get(), materialBufferData);
}

void SimpleSceneRenderer::UpdateMaterial(size_t numTextures, const Video::BlinnPhongMaterial& material)
{
    MaterialConstBuffer materialBufferData;
    {
        materialBufferData.ambientColor     = material.ambient;
        materialBufferData.diffuseColor     = material.diffuse;
        materialBufferData.specularColor    = material.specular;
        materialBufferData.emissiveColor    = material.emissive;
        materialBufferData.shininess        = material.shininess;
        materialBufferData.numTextures      = static_cast<int>(numTextures);
    };
    RenderSys()->UpdateBuffer(materialConstBuffer_.get(), materialBufferData);
}


/*
 * ======= Private: =======
 */

void SimpleSceneRenderer::CreateSceneShader(bool useNormalMapping)
{
    typedef Video::ShaderProvider::StandardShaders Shaders;

    sceneShader_ = RenderSys()->GetShaderProvider()->CreateShaderCompositionFromResource(
        (useNormalMapping ? Shaders::TangentSpaceMesh : Shaders::Simple3DMesh),
        Video::Simple3DVertex::Format()
    );

    vertexConstBuffer_      = sceneShader_->CreateAndAttachConstantBuffer("vertexParam",    "Simple3DMesh");
    materialConstBuffer_    = sceneShader_->CreateAndAttachConstantBuffer("materialParam",  "Simple3DMesh");
    lightConstBuffer_       = sceneShader_->CreateAndAttachConstantBuffer("lightParam",     "Simple3DMesh");
}

void SimpleSceneRenderer::CreateEventHandler()
{
    auto eventHandler = std::make_shared<SceneShaderEventHandler>(vertexConstBuffer_.get());
    sceneShader_->SetEventHandler(eventHandler);
}


/*
 * SceneShaderEventHandler class
 */

SimpleSceneRenderer::SceneShaderEventHandler::SceneShaderEventHandler(Video::ConstantBuffer* vertexConstBuffer) :
    vertexConstBuffer_{ vertexConstBuffer }
{
    ASSERT_POINTER(vertexConstBuffer);
}

void SimpleSceneRenderer::SceneShaderEventHandler::OnUpdateConstantBuffers(
    const Video::ShaderComposition* shaderComposition, Video::RenderContext* renderContext)
{
    VertexConstBuffer vertexBufferData
    {
        renderContext->WVPMatrix(),
        renderContext->GetRenderState().worldMatrix,
        renderContext->GetRenderState().cameraMatrix.GetPosition()
    };
    RenderSys()->UpdateBuffer(vertexConstBuffer_, vertexBufferData);
}


} // /namespace Scene

} // /namespace Fork



// ========================