/*
 * Forward scene renderer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Renderer/ForwardSceneRenderer.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "../Node/ImportNodes.h"
#include "../Geometry/ImportGeometries.h"
#include "../../Video/RenderSystem/RenderSysCtx.h"


namespace Fork
{

namespace Scene
{


void ForwardSceneRenderer::RenderScene(SceneNode* sceneGraph)
{
    if (!sceneGraph)
        return;

    prevShader_ = RenderCtx()->GetRenderState().shaderComposition;

    for (auto& child : sceneGraph->GetChildren())
    {
        if (child->isEnabled)
        {
            /* Store global position of the current scene node and visit the node */
            globalSceneNodePosition = child->LocalTransform().GetPosition();
            child->Visit(this);
        }
    }
}

void ForwardSceneRenderer::VisitGeometryNode(GeometryNode* node)
{
    if (node->geometry)
    {
        auto renderContext = RenderCtx();

        /* Setup world matrix for current scene node */
        SetupWorldMatrix(node->LocalTransform());

        if (cullingManager.IsBoundingVolumeInsideFrustum(node->geometry->boundingVolume))
        {
            /* Bind shader composition */
            if (node->shaderComposition)
                renderContext->BindShader(node->shaderComposition.get());
            else if (prevShader_)
                renderContext->BindShader(prevShader_);

            /* Update active shader composition and render geometry */
            auto activeShader = renderContext->GetRenderState().shaderComposition;
            if (activeShader)
                activeShader->PostUpdateConstantBuffer(renderContext);

            node->geometry->Visit(this);
        }
    }
}

void ForwardSceneRenderer::VisitLODGeometry(LODGeometry* node)
{
    /*
    Compute distance between camera and scene node
    -> Then select detail level (LOD)
    */
    float distance = Math::Distance(globalCameraPosition, globalSceneNodePosition);

    auto geometry = node->SelectLOD(distance);
    if (geometry && cullingManager.IsBoundingVolumeInsideFrustum(geometry->boundingVolume))
        geometry->Visit(this);
}

void ForwardSceneRenderer::VisitCompositionGeometry(CompositionGeometry* node)
{
    for (const auto& subGeom : node->subGeometries)
    {
        if (cullingManager.IsBoundingVolumeInsideFrustum(subGeom->boundingVolume))
            subGeom->Visit(this);
    }
}

void ForwardSceneRenderer::VisitTexturedGeometry(TexturedGeometry* node)
{
    if (node->actualGeometry && cullingManager.IsBoundingVolumeInsideFrustum(node->actualGeometry->boundingVolume))
    {
        auto renderContext = RenderCtx();

        const auto numTextures = node->textures.size();

        for (size_t l = 0; l < numTextures; ++l)
            renderContext->BindTexture(node->textures[l].get(), l);

        node->actualGeometry->Visit(this);

        for (size_t l = 0; l < numTextures; ++l)
            renderContext->UnbindTexture(node->textures[l].get(), l);
    }
}

void ForwardSceneRenderer::VisitSimple3DMeshGeometry(Simple3DMeshGeometry* node)
{
    RenderCtx()->SetupDrawMode(node->primitiveType);
    DrawGeometry(
        node->GetVertexBuffer(), node->GetIndexBuffer(),
        node->vertices.size(), node->indices.size()
    );
}

//                                                      ^
// TODO -> generalize the above and among functions!!! _|
//                                                      |
//                                                      v

void ForwardSceneRenderer::VisitTangentSpaceMeshGeometry(TangentSpaceMeshGeometry* node)
{
    RenderCtx()->SetupDrawMode(node->primitiveType);
    DrawGeometry(
        node->GetVertexBuffer(), node->GetIndexBuffer(),
        node->vertices.size(), node->indices.size()
    );
}


} // /namespace Scene

} // /namespace Fork



// ========================