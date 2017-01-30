/*
 * SSLR post processor file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/AdvancedRenderer/PostProcessor/SSLR.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Video/RenderSystem/Texture/TextureAttributes.h"
#include "Video/RenderSystem/Shader/ShaderProvider.h"
#include "Video/RenderSystem/Shader/StandardShader/SSLRConstantBuffers.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/Exception/InvalidArgumentException.h"
#include "Math/Common/ExtMathFunctions.h"
#include "../../RenderSystem/RenderSysCtx.h"


namespace Fork
{

namespace Video
{


using namespace StandardShader;
using namespace std::placeholders;

SSLR::SSLR(const Math::Size2i& initResolution) :
    PostProcessor{ initResolution }
{
    CreateRayTraceResources();
    CreateBlurResources();
    CreateReflectionResources();
    CreateRenderStates();
}

std::string SSLR::Name() const
{
    return "SSLR";
}

void SSLR::Draw(RenderTarget* renderTarget)
{
    if (!colorMap_)
        NotInitializedException("Input texture is null");

    auto renderContext = RenderCtx();
    auto renderSystem = RenderSys();

    /*
    Step 1:
    Ray-tracing pass
    */
    UpdateRayTracePixelConstBuffer();

    renderContext->SetupDrawMode(GeometryPrimitives::TriangleStrip);
    renderContext->BindShader(rayTraceShader_.get());

    renderContext->BindTexture(colorMap_, 0);
    renderContext->BindTexture(specularMap_, 1);
    renderContext->BindTexture(depthMap_, 2);
    renderContext->BindTexture(normalMap_, 3);

    //renderContext->BindSamplerState(trilinearSampler_.get(), 0);
    //renderContext->BindSamplerState(nearestSampler_.get(), 1);
    renderContext->BindSamplerState(nearestSampler_.get(), 2);
    //renderContext->BindSamplerState(nearestSampler_.get(), 3);
    {
        renderContext->BindVertexBuffer(renderSystem->GetEmptyVertexBuffer());
        renderContext->BindRenderTarget(rayTraceRenderTarget_.get());

        renderContext->Draw(3);
    }
    //renderContext->UnbindSamplerState(0);
    //renderContext->UnbindSamplerState(1);
    renderContext->UnbindSamplerState(2);
    //renderContext->UnbindSamplerState(3);

    /*
    Pass 2:
    Blur pass
    */
    renderContext->GenerateMIPMaps(rayTraceColorBuffer_.get());
    renderContext->GenerateMIPMaps(maskBuffer_.get());

    /*
    Step 3:
    Reflection pass
    */
    UpdateReflectionPixelConstBuffer();

    renderContext->SetupDrawMode(GeometryPrimitives::TriangleStrip);
    renderContext->BindShader(reflectionShader_.get());

    renderContext->BindTexture(colorMap_, 0);
    renderContext->BindTexture(specularMap_, 1);
    renderContext->BindTexture(rayTraceColorBuffer_.get(), 2);
    renderContext->BindTexture(maskBuffer_.get(), 3);
    {
        renderContext->BindVertexBuffer(renderSystem->GetEmptyVertexBuffer());
        renderContext->BindRenderTarget(renderTarget);

        renderContext->Draw(3);
    }
    renderContext->UnbindTexture(colorMap_, 0);
    renderContext->UnbindTexture(specularMap_, 1);
    renderContext->UnbindTexture(depthMap_, 2);
    renderContext->UnbindTexture(normalMap_, 3);
}

void SSLR::Setup(
    Texture2D* colorMap, const Texture2D* normalMap,
    Texture2D* depthMap, const Texture2D* specularMap)
{
    /* Store required texture references */
    ASSERT_POINTER( colorMap    );
    ASSERT_POINTER( normalMap   );
    ASSERT_POINTER( depthMap    );
    ASSERT_POINTER( specularMap );

    colorMap_       = colorMap;
    normalMap_      = normalMap;
    depthMap_       = depthMap;
    specularMap_    = specularMap;
}

void SSLR::SetupProjection(const Math::Matrix4f& projectionMatrix)
{
    projectionMatrix_ = projectionMatrix;
}

void SSLR::SetupView(const Math::Matrix4f& viewMatrix)
{
    viewMatrix_ = viewMatrix;
}


/*
 * ======= Private: =======
 */

void SSLR::OnUpdateResolution()
{
    rayTraceRenderTarget_->Clear();

    RenderSys()->WriteTexture(rayTraceColorBuffer_.get(), rayTraceColorBuffer_->GetFormat(), GetResolution());
    RenderSys()->WriteTexture(maskBuffer_.get(), maskBuffer_->GetFormat(), GetResolution());

    rayTraceRenderTarget_->Attach({ rayTraceColorBuffer_, maskBuffer_ });
}

void SSLR::CreateRayTraceResources()
{
    #if 0
    rayTraceShader_ = RenderSys()->GetShaderProvider()->CreateShaderCompositionFromResource(
        ShaderProvider::StandardShaders::SSLRRayTrace,
        { /* -- this shader has an empty vertex format -- */ }
    );
    #else
    rayTraceShader_ = RenderSys()->GetShaderProvider()->CreateShaderCompositionFromFiles("SSLRRayTrace", {});
    #endif

    rayTracePixelParam_     = rayTraceShader_->CreateAndAttachConstantBuffer("pixelParam");

    rayTraceColorBuffer_    = RenderSys()->CreateTexture2D(TextureFormats::RGBA8UInt, GetResolution());
    maskBuffer_             = RenderSys()->CreateTexture2D(TextureFormats::R8UInt,    GetResolution());

    rayTraceRenderTarget_   = RenderSys()->CreateRenderTarget({ rayTraceColorBuffer_, maskBuffer_ });
}

void SSLR::CreateBlurResources()
{
    #if 0

    blurHorzShader_ = RenderSys()->GetShaderProvider()->CreateShaderCompositionFromResource(
        ShaderProvider::StandardShaders::SSLRHorzBlur,
        { /* -- this shader has an empty vertex format -- */ }
    );
    blurVertShader_ = RenderSys()->GetShaderProvider()->CreateShaderCompositionFromResource(
        ShaderProvider::StandardShaders::SSLRVertBlur,
        { /* -- this shader has an empty vertex format -- */ }
    );

    #endif
}

void SSLR::CreateReflectionResources()
{
    #if 0
    reflectionShader_ = RenderSys()->GetShaderProvider()->CreateShaderCompositionFromResource(
        ShaderProvider::StandardShaders::SSLRReflection,
        { /* -- this shader has an empty vertex format -- */ }
    );
    #else
    reflectionShader_ = RenderSys()->GetShaderProvider()->CreateShaderCompositionFromFiles("SSLRReflection", {});
    #endif
    
    reflectionPixelParam_ = reflectionShader_->CreateAndAttachConstantBuffer("pixelParam");

    reflectionShader_->Attach(RenderSys()->GetRenderStateConstBuffer(), "invScreenParam");
}

void SSLR::CreateRenderStates()
{
    /* Create sampler states */
    SamplerState::Description nearestSamplerDesc;
    {
        nearestSamplerDesc.minFilter    = SamplerState::Filters::Nearest;
        nearestSamplerDesc.magFilter    = SamplerState::Filters::Nearest;
        nearestSamplerDesc.mipFilter    = SamplerState::Filters::Nearest;
        nearestSamplerDesc.wrapMode     = SamplerState::WrapModes::Clamp;
    }
    nearestSampler_ = RenderSys()->CreateSamplerState(nearestSamplerDesc);

    SamplerState::Description trilinearSamplerDesc;
    {
        trilinearSamplerDesc.minFilter  = SamplerState::Filters::Linear;
        trilinearSamplerDesc.magFilter  = SamplerState::Filters::Linear;
        trilinearSamplerDesc.mipFilter  = SamplerState::Filters::Linear;
        trilinearSamplerDesc.wrapMode   = SamplerState::WrapModes::Clamp;
        //trilinearSamplerDesc.anisotropy  = 16;
    }
    trilinearSampler_ = RenderSys()->CreateSamplerState(trilinearSamplerDesc);
}

void SSLR::UpdateRayTracePixelConstBuffer()
{
    SSLRShader::RayTracePixelConstBuffer buffer;
    {
        buffer.projectionMatrix     = projectionMatrix_;
        buffer.invProjectionMatrix  = projectionMatrix_.Inverse();
        buffer.viewMatrix           = viewMatrix_;
        buffer.globalReflectivity   = reflectivity;
        buffer.globalRoughness      = roughness;

        float farPlane;
        Video::Projectionf::ExtractClippingPlanesLH(projectionMatrix_, buffer.nearPlane, farPlane);
    }
    RenderSys()->UpdateBuffer(rayTracePixelParam_.get(), buffer);
}

void SSLR::UpdateReflectionPixelConstBuffer()
{
    SSLRShader::ReflectionPixelConstBuffer buffer;
    {
        buffer.globalReflectivity   = reflectivity;
        buffer.globalRoughness      = roughness;
        buffer.mipCount             = static_cast<float>(Math::NumMIPLevelsBySize(GetResolution()));
    }
    RenderSys()->UpdateBuffer(reflectionPixelParam_.get(), buffer);
}


} // /namespace Video

} // /namespace Fork



// ========================