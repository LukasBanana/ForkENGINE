/*
 * Editor model preview renderer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "ModelPreviewRenderer.h"
#include "ModelUIAsset.h"
#include "Video/RenderSystem/RenderContext.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Scene/Node/LightNode.h"
#include "Scene/LightSource/DirectionalLightSource.h"


namespace Fork
{

namespace Editor
{


ModelPreviewRenderer::ModelPreviewRenderer() :
    geometryNode_{ std::make_shared<Scene::GeometryNode>() }
{
    /* Setup scene graph with single geometry scene node */
    sceneGraph_.AddChild(geometryNode_);
    
    /* Initialize camera projection */
    previewCamera_.projection.SetNearPlane(0.1f);
    previewCamera_.projection.SetFarPlane(5.0f);
    previewCamera_.projection.SetOrthoSize(Math::Size2f(1.75f));

    ChangeProjection(true);

    previewCamera_.UpdateView();

    /* Initialize scene lighting */
    Scene::LightNode lightNode;
    lightNode.lightSource = std::make_shared<Scene::DirectionalLightSource>();
    UpdateLightNodes({ &lightNode });
}

void ModelPreviewRenderer::RenderModelAssetPreview(
    const ModelUIAsset& asset, const Math::Size2i& previewSize, const Math::Quaternionf& rotation)
{
    auto renderContext = Video::RenderContext::Active();
    auto renderSystem = Video::RenderSystem::Active();
    
    auto geometry = asset.GetSharedGeometryResource();

    /* Setup render states for default 3D drawing */
    renderContext->ChangeState(renderSystem->GetDefaultRasterizerState());
    renderContext->ChangeState(renderSystem->GetDefaultDepthStencilState());
    renderContext->ChangeState(renderSystem->GetDefaultBlendState());

    /* Update camera projection viewport */
    previewCamera_.projection.SetViewport({ {}, previewSize });

    /*
    Setup scene node transformation:
    -> This depends on the models size, to make it fit into the viewport.
    */
    const auto viewSize = 1.0f / Math::Size3f(geometry->boundingVolume.box.Size()).Max();
    const Math::Vector3f viewPosition { 0, 0, 1.5f };

    Math::Matrix4f matrix;
    {
        matrix.SetPosition(viewPosition);
        matrix.Scale(Math::Vector3f(viewSize));
        matrix *= rotation.Mat4();
        matrix.Translate(-geometry->boundingVolume.box.Center());
    }
    geometryNode_->transform = matrix;

    /* Setup geometry and render scene */
    geometryNode_->geometry = geometry;

    RenderSceneFromCamera(&sceneGraph_, previewCamera_);
}

void ModelPreviewRenderer::ChangeProjection(bool isOrtho)
{
    previewCamera_.projection.SetOrtho(isOrtho);
}

void ModelPreviewRenderer::ChangeProjection()
{
    ChangeProjection(!previewCamera_.projection.GetOrtho());
}


} // /namespace Editor

} // /namespace Fork



// ========================