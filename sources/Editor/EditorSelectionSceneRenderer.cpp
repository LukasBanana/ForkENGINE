/*
 * Editor selection scene renderer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "EditorSelectionSceneRenderer.h"
#include "Video/RenderSystem/RenderContext.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Video/RenderSystem/Shader/ShaderProvider.h"
#include "Video/RenderSystem/Shader/StandardShader/SolidColoredConstantBuffers.h"
#include "../Video/RenderSystem/RenderSysCtx.h"


namespace Fork
{

namespace Editor
{


/*
 * SolidColoredVertex internal structure
 */

static Video::VertexFormat SolidColoredVertexFormat()
{
    Video::VertexFormat format;
    format.SetupAttributes({{ "coord", Video::RendererDataTypes::Float, 3 }});
    return format;
}


/*
 * EditorSelectionSceneRenderer class
 */

EditorSelectionSceneRenderer::EditorSelectionSceneRenderer()
{
    CreateShader();
    CreateRenderStates();
}

void EditorSelectionSceneRenderer::RenderSelectionHighlights(
    const std::vector<Scene::DynamicSceneNode*>& highlightedSceneNodes,
    const Scene::CameraNode& camera, const Video::ColorRGBAf& highlightColor, float highlightSize)
{
    auto renderSystem = RenderSys();
    auto renderContext = RenderCtx();

    /* Setup shader parameters */
    Video::StandardShader::SolidColoredShader::ConstBuffer shaderParam;
    shaderParam.solidColor = highlightColor;

    renderContext->BindShader(solidColoredShader_.get());

    /* Setup view-projection matrix */
    renderContext->SetupProjectionMatrix(camera.projection);
    renderContext->SetupViewMatrix(camera.GetViewMatrix());

    auto viewProjection = renderContext->VPMatrix();

    SetupView(camera);

    /* Setup render states for stencil buffering */
    renderContext->SetupClearStencil(0);

    auto prevLineSize = renderContext->GetRenderState().lineSize;
    renderContext->SetupLineSize(highlightSize);

    auto prevDepthStencil   = renderContext->GetRenderState().depthStencilState;
    auto prevReasterizer    = renderContext->GetRenderState().rasterizerState;
    auto prevBlend          = renderContext->GetRenderState().blendState;
    {
        /* Render all selection highlights */
        for (auto sceneNode : highlightedSceneNodes)
        {
            /* Update constant buffer */
            shaderParam.wvpMatrix = viewProjection * sceneNode->GlobalTransform();
            renderSystem->UpdateBuffer(constBuffer_.get(), shaderParam);

            /* Render highlighting passes */
            HighlightPass1();
            sceneNode->Visit(this);

            HighlightPass2();
            sceneNode->Visit(this);
        }
    }
    renderContext->ChangeState(prevDepthStencil);
    renderContext->ChangeState(prevReasterizer);
    renderContext->ChangeState(prevBlend);

    renderContext->SetupLineSize(prevLineSize);
}


/*
 * ======= Private: =======
 */

void EditorSelectionSceneRenderer::CreateShader()
{
    solidColoredShader_ = RenderSys()->GetShaderProvider()->CreateShaderCompositionFromResource(
        Video::ShaderProvider::StandardShaders::SolidColored,
        SolidColoredVertexFormat()
    );
    constBuffer_ = solidColoredShader_->CreateAndAttachConstantBuffer("param", "SolidColored");
}

void EditorSelectionSceneRenderer::CreateRenderStates()
{
    auto renderSystem = RenderSys();

    /* Create depth-stencil state for the 1st pass */
    Video::DepthStencilState::Description stencilDesc1;
    {
        stencilDesc1.isStencilTestEnabled       = true;
        stencilDesc1.stencilWriteMask           = 0xff;
        stencilDesc1.stencilReadMask            = 0xff;
        stencilDesc1.frontFace.stencilFailOp    = Video::DepthStencilState::StencilOps::Replace;
        stencilDesc1.frontFace.stencilZFailOp   = Video::DepthStencilState::StencilOps::Replace;
        stencilDesc1.frontFace.stencilZPassOp   = Video::DepthStencilState::StencilOps::Replace;
        stencilDesc1.frontFace.stencilFunc      = Video::CompareFunctions::Always;
    }
    stencilState1_ = renderSystem->CreateDepthStencilState(stencilDesc1);

    /* Create depth-stencil state for the 2nd pass */
    Video::DepthStencilState::Description stencilDesc2;
    {
        stencilDesc2.isStencilTestEnabled       = true;
        stencilDesc2.stencilWriteMask           = 0xff;
        stencilDesc2.stencilReadMask            = 0xff;
        stencilDesc2.frontFace.stencilFailOp    = Video::DepthStencilState::StencilOps::Keep;
        stencilDesc2.frontFace.stencilZFailOp   = Video::DepthStencilState::StencilOps::Keep;
        stencilDesc2.frontFace.stencilZPassOp   = Video::DepthStencilState::StencilOps::Keep;
        stencilDesc2.frontFace.stencilFunc      = Video::CompareFunctions::Equal;
    }
    stencilState2_ = renderSystem->CreateDepthStencilState(stencilDesc2);

    /* Create rasterizer state for the 1st pass */
    Video::RasterizerState::Description rasterDesc1;
    {
        rasterDesc1.cullMode                = Video::RasterizerState::CullModes::None;
        rasterDesc1.isMultiSampleEnable     = true;
        rasterDesc1.isAntiAliasedLineEnable = true;
    }
    rasterState1_ = renderSystem->CreateRasterizerState(rasterDesc1);

    /* Create rasterizer state for the 2nd pass */
    Video::RasterizerState::Description rasterDesc2;
    {
        rasterDesc2.fillMode                = Video::RasterizerState::FillModes::Wireframe;
        rasterDesc2.isMultiSampleEnable     = true;
        rasterDesc2.isAntiAliasedLineEnable = true;
    }
    rasterState2_ = renderSystem->CreateRasterizerState(rasterDesc2);

    /* Create blend state for the 1st pass */
    Video::BlendState::Description blendDesc1;
    {
        blendDesc1.colorMask = { false, false, false, false };
    }
    blendState1_ = renderSystem->CreateBlendState(blendDesc1);

    /* Create blend state for the 2nd pass */
    Video::BlendState::Description blendDesc2;
    blendState2_ = renderSystem->CreateBlendState(blendDesc2);
}

void EditorSelectionSceneRenderer::HighlightPass1()
{
    RenderCtx()->ClearBuffers(Video::ClearBuffersFlags::Stencil | Video::ClearBuffersFlags::Depth);

    RenderCtx()->ChangeState(stencilState1_.get(), 1);
    RenderCtx()->ChangeState(rasterState1_.get());
    RenderCtx()->ChangeState(blendState1_.get());
}

void EditorSelectionSceneRenderer::HighlightPass2()
{
    RenderCtx()->ChangeState(stencilState2_.get(), 0);
    RenderCtx()->ChangeState(rasterState2_.get());
    RenderCtx()->ChangeState(blendState2_.get());
}


} // /namespace Editor

} // /namespace Fork



// ========================