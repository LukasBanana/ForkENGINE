/*
 * Scene renderer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Renderer/SceneRenderer.h"
#include "Scene/Node/CameraNode.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "../../Video/RenderSystem/RenderSysCtx.h"


namespace Fork
{

namespace Scene
{


void SceneRenderer::SetupView(const CameraNode& camera)
{
    auto renderContext = RenderCtx();

    /* Store global camera position */
    globalCameraPosition = camera.GlobalTransform().GetPosition();

    /* Setup projection and view matrices */
    renderContext->SetupProjectionMatrix(camera.projection);
    renderContext->SetupViewMatrix(camera.GetViewMatrix());

    /* Setup projection and view to culling manager */
    cullingManager.SetupProjectionMatrix(camera.projection);
    cullingManager.SetupViewMatrix(camera.GetViewMatrix());
}

void SceneRenderer::RenderSceneFromCamera(SceneNode* sceneGraph, const CameraNode& camera)
{
    SetupView(camera);
    RenderScene(sceneGraph);
}


/*
 * ======= Protected: =======
 */

void SceneRenderer::DrawGeometry(
    Video::VertexBuffer* vertexBuffer, Video::IndexBuffer* indexBuffer,
    size_t numVertices, size_t numIndices)
{
    auto renderContext = RenderCtx();

    if (indexBuffer)
    {
        renderContext->BindVertexBuffer(vertexBuffer);
        renderContext->BindIndexBuffer(indexBuffer);
        renderContext->DrawIndexed(numIndices);
    }
    else
    {
        renderContext->BindVertexBuffer(vertexBuffer);
        renderContext->Draw(numVertices);
    }
}

void SceneRenderer::SetupWorldMatrix(const Math::Matrix4f& matrix)
{
    RenderCtx()->SetupWorldMatrix(matrix);
    cullingManager.SetupWorldMatrix(matrix);
}


} // /namespace Scene

} // /namespace Fork



// ========================