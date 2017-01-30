/*
 * Bounding box scene renderer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Renderer/BoundingBoxSceneRenderer.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Video/RenderSystem/RenderContextException.h"
#include "Video/BufferFormat/Simple3DVertex.h"
#include "Video/RenderSystem/Shader/ShaderProvider.h"
#include "Video/RenderSystem/Shader/StandardShader/BoundingBoxConstantBuffers.h"
#include "Core/Exception/NullPointerException.h"
#include "Math/Common/Convert.h"

#include "../Geometry/ImportGeometries.h"
#include "../Node/ImportNodes.h"
#include "../../Video/RenderSystem/RenderSysCtx.h"


namespace Fork
{

namespace Scene
{


using namespace Video::StandardShader::BoundingBoxShader;

BoundingBoxSceneRenderer::BoundingBoxSceneRenderer(SceneManager* sceneManager)
{
    ASSERT_POINTER(sceneManager);

    static const int atlasTexSize = 1024;

    CreateAtlasTexture({ atlasTexSize, atlasTexSize });
    CreateBoxGeometry(sceneManager);
    CreateBoxShader();
}
BoundingBoxSceneRenderer::~BoundingBoxSceneRenderer()
{
}

void BoundingBoxSceneRenderer::RenderScene(SceneNode* sceneGraph)
{
    if (!sceneGraph)
        return;

    /* Setup world matrices */
    InitWorldMatrixCounter();

    VisitSceneNode(sceneGraph);

    if (!worldMatrixCounter_)
        return;

    /* Update world matrix atlas texture */
    auto renderSystem = RenderSys();
    auto renderContext = RenderCtx();

    renderSystem->WriteSubTexture(
        worldMatrixAtlasTexture_.get(), { 0, 0 }, MinRequiredTextureSize(), 0,
        Video::ImageColorFormats::RGBA, Video::RendererDataTypes::Float, worldMatrices_.data()
    );

    /* Update vertex constnat buffer */
    ConstBuffer constBufferData
    {
        renderContext->VPMatrix(),
        boundingBoxColor,
        { texSize_.width, texSize_.height, 0, 0 }
    };
    renderSystem->UpdateBuffer(constBuffer_.get(), constBufferData);
    
    /* Render scene with a single draw call */
    renderContext->SetupDrawMode(boxGeometry_->primitiveType);

    renderContext->BindShader(boxShader_.get());
    
    renderContext->BindVertexBuffer(boxGeometry_->GetVertexBuffer());
    renderContext->BindIndexBuffer(boxGeometry_->GetIndexBuffer());

    renderContext->BindTexture(worldMatrixAtlasTexture_.get());
    {
        renderContext->DrawInstancedIndexed(
            boxGeometry_->indices.size(), worldMatrixCounter_
        );
    }
    renderContext->UnbindTexture(worldMatrixAtlasTexture_.get());

    renderContext->UnbindIndexBuffer();
    renderContext->UnbindVertexBuffer();
}


/*
 * ======= Private: =======
 */

void BoundingBoxSceneRenderer::CreateAtlasTexture(const Math::Size2i& texSize)
{
    texSize_ = texSize;

    /* Create atlas texture */
    worldMatrixAtlasTexture_ = RenderSys()->CreateTexture2D();

    RenderSys()->WriteTexture(
        worldMatrixAtlasTexture_.get(), Video::TextureFormats::RGBA32Float, texSize, 0,
        Video::ImageColorFormats::RGBA, Video::RendererDataTypes::Float, nullptr
    );

    /* Store maximal number of world matrices */
    maxNumWorldMatrices_ = texSize.width*texSize.height/4;
    worldMatrices_.resize(maxNumWorldMatrices_);
}

void BoundingBoxSceneRenderer::CreateBoxGeometry(SceneManager* sceneManager)
{
    boxGeometry_ = sceneManager->GenerateWireCube();
}

void BoundingBoxSceneRenderer::CreateBoxShader()
{
    boxShader_ = RenderSys()->GetShaderProvider()->CreateShaderCompositionFromResource(
        Video::ShaderProvider::StandardShaders::BoundingBox,
        Video::Simple3DVertex::Format()
    );
    constBuffer_ = boxShader_->CreateAndAttachConstantBuffer("vertexParam", "BoundingBox");
}

void BoundingBoxSceneRenderer::InitWorldMatrixCounter()
{
    worldMatrixCounter_ = 0;
}

void BoundingBoxSceneRenderer::AppendWorldMatrix(const Math::Matrix4f& matrix)
{
    if (worldMatrixCounter_ < maxNumWorldMatrices_)
    {
        worldMatrices_[worldMatrixCounter_] = matrix;
        ++worldMatrixCounter_;
    }
}

void BoundingBoxSceneRenderer::AppendBoundingBox(const Math::AABB3f& boundingBox)
{
    auto matrix = currentWorldMatrix_;

    /* Transform current world matrix to box boundaries */
    matrix.Translate(boundingBox.Center());
    matrix.Scale(Math::ToVec3(boundingBox.Size()*0.5f));

    AppendWorldMatrix(matrix);
}

Math::Size2i BoundingBoxSceneRenderer::MinRequiredTextureSize() const
{
    return { texSize_.width,
             static_cast<int>(worldMatrixCounter_) / texSize_.width + 1 };
}

/* --- Visitor interface --- */

void BoundingBoxSceneRenderer::VisitSceneNode(SceneNode* node)
{
    /* Append world matrix */
    if (node->HasTransform())
        node->Visit(this);

    /* Visit further children */
    for (auto& child : node->GetChildren())
        VisitSceneNode(child.get());
}

void BoundingBoxSceneRenderer::VisitGeometryNode(GeometryNode* node)
{
    if (node->geometry)
    {
        currentWorldMatrix_ = node->GlobalTransform();
        if (showGeometries)
            node->geometry->Visit(this);
        else
            AppendBoundingBox(node->geometry->boundingVolume.box);
    }
}

void BoundingBoxSceneRenderer::VisitLightNode(LightNode* node)
{
    // dummy
}

void BoundingBoxSceneRenderer::VisitCameraNode(CameraNode* node)
{
    // dummy
}

void BoundingBoxSceneRenderer::VisitLODGeometry(LODGeometry* node)
{
    AppendBoundingBox(node->boundingVolume.box);
    for (auto& subGeom : node->lodGeometries)
        subGeom->Visit(this);
}

void BoundingBoxSceneRenderer::VisitBillboardGeometry(BillboardGeometry* node)
{
    // dummy
}

void BoundingBoxSceneRenderer::VisitTerrainGeometry(TerrainGeometry* node)
{
    if (node->templateGeometry)
        node->templateGeometry->Visit(this);
}

void BoundingBoxSceneRenderer::VisitMeshGeometry(MeshGeometry* node)
{
    AppendBoundingBox(node->boundingVolume.box);
}

void BoundingBoxSceneRenderer::VisitCompositionGeometry(CompositionGeometry* node)
{
    AppendBoundingBox(node->boundingVolume.box);
    for (auto& subGeom : node->subGeometries)
        subGeom->Visit(this);
}

void BoundingBoxSceneRenderer::VisitTexturedGeometry(TexturedGeometry* node)
{
    AppendBoundingBox(node->boundingVolume.box);
    if (node->actualGeometry)
        node->actualGeometry->Visit(this);
}

void BoundingBoxSceneRenderer::VisitSimple2DMeshGeometry(Simple2DMeshGeometry* node)
{
    AppendBoundingBox(node->boundingVolume.box);
}

void BoundingBoxSceneRenderer::VisitSimple3DMeshGeometry(Simple3DMeshGeometry* node)
{
    AppendBoundingBox(node->boundingVolume.box);
}

void BoundingBoxSceneRenderer::VisitTangentSpaceMeshGeometry(TangentSpaceMeshGeometry* node)
{
    AppendBoundingBox(node->boundingVolume.box);
}

void BoundingBoxSceneRenderer::VisitCommonMeshGeometry(CommonMeshGeometry* node)
{
    AppendBoundingBox(node->boundingVolume.box);
}

void BoundingBoxSceneRenderer::VisitTerrainMeshGeometry(TerrainMeshGeometry* node)
{
    AppendBoundingBox(node->boundingVolume.box);
}


} // /namespace Scene

} // /namespace Fork



// ========================