/*
 * Cube map renderer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Renderer/CubeMapRenderer.h"
#include "Scene/Renderer/SceneRenderer.h"
#include "Video/RenderSystem/Texture/RenderTarget.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Video/RenderSystem/RenderContext.h"
#include "Core/Exception/NullPointerException.h"
#include "../../Video/RenderSystem/RenderSysCtx.h"
#include "Core/StaticConfig.h"


namespace Fork
{

namespace Scene
{


CubeMapRenderer::CubeMapRenderer()
{
    /*
    Initialize default configuration of the projection.
    Viewport has size { 1, 1 } just to have an aspect ratio of 1:1.
    */
    projection.SetFOV(90.0f * Math::deg2rad);
    projection.SetViewport({ {}, { 1, 1 } });
}

void CubeMapRenderer::Begin()
{
    ASSERT_3STATE_BEGIN;
    prevRenderTarget_ = RenderCtx()->GetRenderState().renderTarget;
    prevViewport_ = RenderCtx()->GetRenderState().viewport;
}

void CubeMapRenderer::End()
{
    ASSERT_3STATE_END;
    RenderCtx()->SetupViewport(prevViewport_);
    RenderCtx()->BindRenderTarget(prevRenderTarget_);
}

void CubeMapRenderer::RenderCubeMap(
    const Video::TextureCube::FaceRenderTargetRefs& cubeFaces,
    SceneRenderer* sceneRenderer, SceneNode* sceneGraph,
    const Math::Matrix4f& cameraTransform, const Video::ClearBuffersFlags::DataType clearFlags)
{
    ASSERT_POINTER(sceneRenderer);
    ASSERT_3STATE_USE;
    
    /* Setup global camera position and projection */
    sceneRenderer->globalCameraPosition = cameraTransform.GetPosition();
    
    RenderCtx()->SetupProjectionMatrix(projection);
    sceneRenderer->cullingManager.SetupProjectionMatrix(projection);

    /* Render cube map faces */
    RenderCubeMapFace(Video::TextureCube::Faces::XPositive, cubeFaces.xPositive, sceneRenderer, sceneGraph, cameraTransform, clearFlags);
    RenderCubeMapFace(Video::TextureCube::Faces::XNegative, cubeFaces.xNegative, sceneRenderer, sceneGraph, cameraTransform, clearFlags);
    RenderCubeMapFace(Video::TextureCube::Faces::YPositive, cubeFaces.yPositive, sceneRenderer, sceneGraph, cameraTransform, clearFlags);
    RenderCubeMapFace(Video::TextureCube::Faces::YNegative, cubeFaces.yNegative, sceneRenderer, sceneGraph, cameraTransform, clearFlags);
    RenderCubeMapFace(Video::TextureCube::Faces::ZPositive, cubeFaces.zPositive, sceneRenderer, sceneGraph, cameraTransform, clearFlags);
    RenderCubeMapFace(Video::TextureCube::Faces::ZNegative, cubeFaces.zNegative, sceneRenderer, sceneGraph, cameraTransform, clearFlags);
}

void CubeMapRenderer::RenderCubeMap(
    const Video::TextureCube::FaceRenderTargetRefs& cubeFaces,
    SceneRenderer* sceneRenderer, SceneNode* sceneGraph,
    const Math::Point3f& cameraPosition, const Video::ClearBuffersFlags::DataType clearFlags)
{
    /* Setup translation matrix and render cube map with primary "RenderCubeMap" function */
    Math::Matrix4f cameraTransform;
    cameraTransform.SetPosition(cameraPosition);
    RenderCubeMap(cubeFaces, sceneRenderer, sceneGraph, cameraTransform, clearFlags);
}


/*
 * ======= Private: =======
 */

void CubeMapRenderer::RenderCubeMapFace(
    const CubeFaces cubeFace, Video::RenderTarget* renderTarget,
    SceneRenderer* sceneRenderer, SceneNode* sceneGraph,
    Math::Matrix4f cameraTransform, const Video::ClearBuffersFlags::DataType clearFlags)
{
    ASSERT_POINTER(renderTarget);

    /* Setup view matrix */
    static const float deg90 = 90.0f * Math::deg2rad;
    static const float deg180 = 180.0f * Math::deg2rad;

    switch (cubeFace)
    {
        case CubeFaces::XPositive:
            cameraTransform.RotateY(deg90);
            break;
        case CubeFaces::XNegative:
            cameraTransform.RotateY(-deg90);
            break;
        case CubeFaces::YPositive:
            cameraTransform.RotateX(-deg90);
            break;
        case CubeFaces::YNegative:
            cameraTransform.RotateX(deg90);
            break;
        case CubeFaces::ZPositive:
            // no rotation
            break;
        case CubeFaces::ZNegative:
            cameraTransform.RotateY(deg180);
            break;
    }

    /* Setup render target and clear buffers */
    cameraTransform.MakeInverse();

    auto renderContext = RenderCtx();

    renderContext->BindRenderTarget(renderTarget);
    renderContext->ClearBuffers(clearFlags);
    renderContext->SetupViewport({ {}, renderTarget->GetSize() });
    renderContext->SetupViewMatrix(cameraTransform);

    sceneRenderer->cullingManager.SetupViewMatrix(cameraTransform);

    /* Render scene from current view */
    sceneRenderer->RenderScene(sceneGraph);
}


} // /namespace Scene

} // /namespace Fork



// ========================