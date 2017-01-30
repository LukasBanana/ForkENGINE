/*
 * Gaussian blur post processor file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/AdvancedRenderer/PostProcessor/GaussianBlur.h"
#include "Video/RenderSystem/Shader/ShaderProvider.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Math/Common/ExtMathFunctions.h"
#include "Core/Exception/NullPointerException.h"
#include "../../RenderSystem/RenderSysCtx.h"


namespace Fork
{

namespace Video
{


using namespace StandardShader;

GaussianBlur::GaussianBlur(const Math::Size2i& initResolution) :
    PostProcessor{ initResolution }
{
    /* Create effect resources */
    CreateShader();
    CreateRenderTarget();
    CreateSamplerState();

    /* Initialize configuration */
    ChangeConfig(8);
}

std::string GaussianBlur::Name() const
{
    return "Gaussian Blur";
}

void GaussianBlur::Draw(RenderTarget* renderTarget)
{
    if (!inputTexture_)
        NotInitializedException("Input texture is null");

    auto renderSystem = RenderSys();
    auto renderContext = RenderCtx();

    renderContext->SetupDrawMode(GeometryPrimitives::TriangleStrip);
    renderContext->BindShader(shader_.get());

    renderContext->BindVertexBuffer(renderSystem->GetEmptyVertexBuffer());
    renderContext->BindSamplerState(clampedSamplerState_.get());
    {
        /* 1st render pass: Draw horizontal blur */
        DrawRenderPass(inputTexture_, tempRenderTarget_.get(), renderPassHorz);
        
        /* 2nd render pass: Draw vertical blur */
        DrawRenderPass(tempTexture_.get(), renderTarget, renderPassVert);
    }
    renderContext->UnbindSamplerState();
    renderContext->UnbindVertexBuffer();

    renderContext->UnbindTexture(tempTexture_.get());
}

void GaussianBlur::Setup(const Texture2D* inputTexture)
{
    ASSERT_POINTER(inputTexture);
    inputTexture_ = inputTexture;
}

void GaussianBlur::DrawRenderPass(const Texture2D* inputTexture, RenderTarget* renderTarget, int renderPass)
{
    /* Setup render target and update render pass constant buffer */
    RenderCtx()->BindRenderTarget(renderTarget);
    SetupRenderPass(renderPass);

    /* Draw fullscreen triangle */
    RenderCtx()->BindTexture(inputTexture);
    RenderCtx()->Draw(3);
}

void GaussianBlur::ChangeConfig(int numSamples, float stretchFactor, float variance)
{
    numSamples = Math::Clamp(numSamples, 1, static_cast<int>(GaussianBlurShader::ConfigConstBuffer::maxNumSamples));

    /* Re-compute blur- weights and offsets */
    ComputeWeights(numSamples, variance);

    config_.stretchFactor   = stretchFactor;
    config_.variance        = variance;

    if (config_.numSamples != numSamples)
    {
        config_.numSamples = numSamples;
        constBufferConfigData_.numSamples = numSamples;
        ComputeOffsets(numSamples, stretchFactor);
    }

    UpdateConfigConstantBuffer();
}


/*
 * ======= Private: =======
 */

void GaussianBlur::OnUpdateResolution()
{
    /* Store new size and re-compute blur offset */
    ComputeOffsets(config_.numSamples, config_.stretchFactor);

    /* Update texture size and 'config' constant buffer */
    UpdateTextureSize();
    UpdateConfigConstantBuffer();
}

void GaussianBlur::CreateShader()
{
    shader_ = RenderSys()->GetShaderProvider()->CreateShaderCompositionFromResource(
        ShaderProvider::StandardShaders::GaussianBlur,
        { /* -- this shader has an empty vertex format -- */ }
    );

    constBufferConfig_      = shader_->CreateAndAttachConstantBuffer("configParam", "GaussianBlur");
    constBufferRenderPass_  = shader_->CreateAndAttachConstantBuffer("renderPassParam", "GaussianBlur");

    shader_->Attach(RenderSys()->GetRenderStateConstBuffer(), "vertexParam");
}

void GaussianBlur::CreateRenderTarget()
{
    /*
    This temporary texture and render target will be used to draw the
    first blur pass (horizontal) and then, the result will be used to
    draw the second blur pass (vertical) into the final render target.
    */
    tempTexture_        = RenderSys()->CreateTexture2D(Video::TextureFormats::RGBA, GetResolution());
    tempRenderTarget_   = RenderSys()->CreateRenderTarget({ tempTexture_ });
}

void GaussianBlur::CreateSamplerState()
{
    SamplerState::Description desc;
    {
        desc.wrapMode   = SamplerState::WrapModes::Clamp;
        desc.useMIPMaps = false;
    }
    clampedSamplerState_ = RenderSys()->CreateSamplerState(desc);
}

void GaussianBlur::UpdateTextureSize()
{
    /* Resize texture and re-attach to render target */
    tempRenderTarget_->Clear();
    RenderSys()->WriteTexture(tempTexture_.get(), Video::TextureFormats::RGBA, GetResolution());
    tempRenderTarget_->Attach(tempTexture_);
}

void GaussianBlur::UpdateConfigConstantBuffer()
{
    RenderSys()->UpdateBuffer(constBufferConfig_.get(), constBufferConfigData_);
}

void GaussianBlur::SetupRenderPass(int renderPass)
{
    GaussianBlurShader::RenderPassConstBuffer renderPassBuffer;
    renderPassBuffer.renderPass = renderPass;
    RenderSys()->UpdateBuffer(constBufferRenderPass_.get(), renderPassBuffer);
}

void GaussianBlur::ComputeWeights(int numSamples, float variance)
{
    /* Compute blur weights */
    auto& offsetsAndWeights = constBufferConfigData_.offsetsAndWeights;

    if (numSamples > 1)
    {
        const auto halfSize = HalfSize(numSamples);

        /* Compute weights with gaussian distribution */
        float sum = 0.0f;

        for (int i = 0; i < numSamples; ++i)
        {
            /* Compute gaussian value and apply gaussian-multiplier for disrcete distribution */
            auto pos = (static_cast<float>(i) - halfSize) / halfSize;
            offsetsAndWeights[i].z = Math::GaussianValue(pos, 0.0f, variance);
            sum += offsetsAndWeights[i].z;
        }

        /* Normalize weights so that the sum is equal to 1.0 */
        sum = 1.0f / sum;
        for (int i = 0; i < numSamples; ++i)
            offsetsAndWeights[i].z *= sum;
    }
    else
        offsetsAndWeights[0].z = 1.0f;
}

void GaussianBlur::ComputeOffsets(int numSamples, float stretchFactor)
{
    /* Compute blur offsets */
    const auto halfSize = HalfSize(numSamples);
    
    auto& offsetsAndWeights = constBufferConfigData_.offsetsAndWeights;

    for (int i = 0; i < numSamples; ++i)
    {
        auto pos = static_cast<float>(i) - halfSize;
        
        offsetsAndWeights[i].x = pos * (stretchFactor / GetResolution().width);
        offsetsAndWeights[i].y = pos * (stretchFactor / GetResolution().height);
    }
}

float GaussianBlur::HalfSize(int numSamples) const
{
    return static_cast<float>(numSamples - 1) * 0.5f;
}


} // /namespace Video

} // /namespace Fork



// ========================