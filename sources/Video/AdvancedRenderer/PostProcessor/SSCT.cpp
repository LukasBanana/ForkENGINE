/*
 * SSCT post processor file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/AdvancedRenderer/PostProcessor/SSCT.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Video/RenderSystem/Texture/TextureAttributes.h"
#include "Video/RenderSystem/Shader/ShaderProvider.h"
#include "Video/RenderSystem/Shader/StandardShader/SSCTConstantBuffers.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/Exception/InvalidArgumentException.h"
#include "Math/Common/ExtMathFunctions.h"
#include "../../RenderSystem/RenderSysCtx.h"

#ifdef _DEBUG_SSCT_
#include "IO/Core/Log.h"
#include "IO/Core/ScopedTimer.h"
#endif


namespace Fork
{

namespace Video
{


using namespace StandardShader;
using namespace std::placeholders;

SSCT::SSCT(const Math::Size2i& initResolution) :
    PostProcessor{ initResolution }
{
    CreateConvolutionResources();
    CreateHiZMapResources();
    CreateVisibilityMapResources();
    CreateReflectionResources();
    CreateRenderStates();
}

std::string SSCT::Name() const
{
    return "SSCT";
}

void SSCT::Draw(RenderTarget* renderTarget)
{
    if (!colorMap_)
        NotInitializedException("Input texture is null");

    auto renderContext = RenderCtx();
    auto renderSystem = RenderSys();

    /*
    Step 1:
    Pre-convolution pass
    */
    #if 1 && defined(_DEBUG_SSCT_)
    if (_Deb_UseBlur)//!!!
    {
        renderContext->GenerateMIPMaps(colorMap_, blurHorzShader_.get());
        renderContext->GenerateMIPMaps(bluredColorMap_.get(), colorMap_, blurVertShader_.get());
    }
    else
    #endif
    renderContext->GenerateMIPMaps(colorMap_);

    /*
    Pass 2:
    Generate Hi-Z depth map
    */
    renderContext->GenerateMIPMaps(
        depthMap_, hiZMapShader_.get(), 0,
        std::bind(&SSCT::OnUpdateHiZPixelParam, this, _1, _2)
    );

    /*
    Step 3:
    Pre-integration pass
    */
    renderContext->BindTexture(depthMap_, 1);
    renderContext->GenerateMIPMaps(
        visibilityMap_.get(), visibilityMapShader_.get(), 0,
        std::bind(&SSCT::OnUpdateVisibilityPixelParam, this, _1, _2)
    );

    /*
    Step 4:
    Reflection (Ray-tracing and cone-tracing) pass
    */
    UpdateReflectionPixelConstBuffer();

    renderContext->SetupDrawMode(GeometryPrimitives::TriangleStrip);
    renderContext->BindShader(reflectionShader_.get());

    renderContext->BindTexture(colorMap_, 0);
    renderContext->BindTexture(visibilityMap_.get(), 2);
    renderContext->BindTexture(normalMap_, 3);
    renderContext->BindTexture(specularMap_, 4);

    if (environmentMap_)
        renderContext->BindTexture(environmentMap_, 5);

    renderContext->BindSamplerState(trilinearSampler_.get(), 0);
    renderContext->BindSamplerState(nearestSampler_.get(), 1);
    renderContext->BindSamplerState(trilinearSampler_.get(), 2);
    {
        renderContext->BindVertexBuffer(renderSystem->GetEmptyVertexBuffer());
        renderContext->BindRenderTarget(renderTarget);

        renderContext->Draw(3);
    }
    renderContext->UnbindSamplerState(2);
    renderContext->UnbindSamplerState(1);
    renderContext->UnbindSamplerState(0);

    if (environmentMap_)
        renderContext->UnbindTexture(environmentMap_, 5);

    renderContext->UnbindTexture(colorMap_, 0);
    renderContext->UnbindTexture(depthMap_, 1);
    renderContext->UnbindTexture(visibilityMap_.get(), 2);
    renderContext->UnbindTexture(normalMap_, 3);
    renderContext->UnbindTexture(specularMap_, 4);
}

void SSCT::Setup(
    Texture2D* colorMap, const Texture2D* normalMap, Texture2D* depthMap,
    const Texture2D* specularMap, const TextureCube* environmentMap)
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

    /* Store optional texture references */
    environmentMap_ = environmentMap;
}

void SSCT::SetupProjection(const Math::Matrix4f& projectionMatrix)
{
    projectionMatrix_ = projectionMatrix;
}

void SSCT::SetupView(const Math::Matrix4f& viewMatrix)
{
    viewMatrix_ = viewMatrix;
}

#ifdef _DEBUG_SSCT_

void SSCT::_Deb_DrawDebugInfo(
    PrimitiveRenderer* renderer, const TextureFont* font,
    const Math::Matrix4f& viewMatrix, const Projectionf& projection)
{
    if (!_Deb_RayMarching || !_Deb_RayMarchFeedback)
        return;

    /* --- Read debug output back from storage buffer --- */

    #include "Core/PackPush.h"

    struct DebugInfoStruct
    {
        Math::Vector4f  originalRay;
        Math::Vector4f  reflectionPos;
        Math::Vector4f  reflectionRay;
        Math::Point4f   rayPoints[64];
        Math::Vector4f  firstRayPos;
        unsigned int    iterations;
        float           cellBoundaryFactor;
        int             earlyExit;
        int             cellsCrossed;
    }
    PACK_STRUCT
    deboutp;

    #include "Core/PackPop.h"

    auto output = reinterpret_cast<DebugInfoStruct*>(
        RenderSys()->MapBuffer(_Deb_RayMarchFeedback.get(), HardwareBufferCPUAccess::ReadOnly)
    );
    memcpy(&deboutp, output, sizeof(deboutp));
    RenderSys()->UnmapBuffer(_Deb_RayMarchFeedback.get());

    float posY = 5.0f;
    auto DrawInfo = [&](const std::string& infoText)
    {
        renderer->DrawText2D(font, { 5, posY }, infoText);
        posY += 15.0f;
    };

    /* --- Draw debug output --- */

    renderer->BeginDrawing3D();
    {
        RenderCtx()->SetupProjectionMatrix(projection);
        
        // Unproject ray points
        Math::Line4f rayLine;
        
        rayLine.start = deboutp.reflectionPos;
        rayLine.start = { (rayLine.start.x - 0.5f) * 2.0f, (rayLine.start.y - 0.5f)*-2.0f, rayLine.start.z, 1.0f };
        rayLine.start = Video::Projectionf::UnprojectPoint(projectionMatrix_, rayLine.start);

        rayLine.end = deboutp.reflectionPos + deboutp.reflectionRay;
        rayLine.end = { (rayLine.end.x - 0.5f) * 2.0f, (rayLine.end.y - 0.5f)*-2.0f, rayLine.end.z, 1.0f };
        rayLine.end = Video::Projectionf::UnprojectPoint(projectionMatrix_, rayLine.end);

        // Draw ray
        RenderCtx()->SetupLineSize(3.0f);
        renderer->DrawLine(rayLine, Video::ColorRGBAf{ 0, 1, 0, 1 });
        RenderCtx()->SetupLineSize(1.0f);
    }
    renderer->EndDrawing3D();

    renderer->BeginDrawing2D();
    {
        auto Vec3ToStr = [](const Math::Vector4f& vec)
        {
            return "{ " + ToStr(vec.x) + ", " + ToStr(vec.y) + ", " + ToStr(vec.z) + " }";
        };

        DrawInfo("Original Ray Direction:   " + Vec3ToStr(deboutp.originalRay));
        DrawInfo("Screen-Space Ray Origin:  " + Vec3ToStr(deboutp.reflectionPos));
        DrawInfo("Screen-Space Ray Dir:     " + Vec3ToStr(deboutp.reflectionRay));
        DrawInfo("Ray Marching Iterations:  " + ToStr(deboutp.iterations));
        DrawInfo("First Boundary Factor:    " + ToStr(deboutp.cellBoundaryFactor));
        DrawInfo("First Ray Pos:            " + Vec3ToStr(deboutp.firstRayPos));
        DrawInfo("Early Ray March Exit:     " + ToStr(deboutp.earlyExit != 0 ? "TRUE" : "FALSE"));
        DrawInfo("Cells Crossed:            " + ToStr(deboutp.cellsCrossed));

        auto w = static_cast<float>(GetResolution().width);
        auto h = static_cast<float>(GetResolution().height);

        for (unsigned int i = 0; i < std::min(deboutp.iterations, 64u); ++i)
        {
            Math::Point2f pos { deboutp.rayPoints[i].x*w + 5.0f, deboutp.rayPoints[i].y*h - 20.0f };
            renderer->DrawText2D(font, Math::Floor(pos), ToStr(i+1));
        }
    }
    renderer->EndDrawing2D();
}

bool SSCT::_Deb_ReloadShaders()
{
    // Reload shader
    ShaderCompositionPtr shader;

    try
    {
        shader = RenderSys()->GetShaderProvider()->CreateShaderCompositionFromFiles("SSCTReflection", {});
    }
    catch (const DefaultException& err)
    {
        IO::Log::Error(err);
        return false;
    }

    if (!shader || !shader->HasCompiledSuccessful())
        return false;

    // Replace old shader
    reflectionShader_ = shader;
    
    reflectionPixelParam_ = reflectionShader_->CreateAndAttachConstantBuffer("pixelParam");
    reflectionShader_->Attach(RenderSys()->GetRenderStateConstBuffer(), "invScreenParam");

    _Deb_RayMarchFeedback = reflectionShader_->CreateAndAttachStorageBuffer("_DebugOutput_", HardwareBufferUsage::Dynamic, true);

    return true;}

#endif


/*
 * ======= Private: =======
 */

void SSCT::OnUpdateResolution()
{
    RenderSys()->WriteTexture(visibilityMap_.get(), visibilityMap_->GetFormat(), GetResolution());
    RenderSys()->WriteTexture(bluredColorMap_.get(), bluredColorMap_->GetFormat(), GetResolution());
}

void SSCT::CreateConvolutionResources()
{
    blurHorzShader_ = RenderSys()->GetShaderProvider()->CreateShaderCompositionFromResource(
        ShaderProvider::StandardShaders::SSCTHorzBlur,
        { /* -- this shader has an empty vertex format -- */ }
    );
    blurVertShader_ = RenderSys()->GetShaderProvider()->CreateShaderCompositionFromResource(
        ShaderProvider::StandardShaders::SSCTVertBlur,
        { /* -- this shader has an empty vertex format -- */ }
    );

    bluredColorMap_ = RenderSys()->CreateTexture2D(TextureFormats::RGBA8UInt, GetResolution());
}

void SSCT::CreateHiZMapResources()
{
    hiZMapShader_ = RenderSys()->GetShaderProvider()->CreateShaderCompositionFromResource(
        ShaderProvider::StandardShaders::SSCTHiZMap,
        { /* -- this shader has an empty vertex format -- */ }
    );

    hiZPixelParam_ = hiZMapShader_->CreateAndAttachConstantBuffer("pixelParam");
}

void SSCT::CreateVisibilityMapResources()
{
    visibilityMapShader_ = RenderSys()->GetShaderProvider()->CreateShaderCompositionFromResource(
        ShaderProvider::StandardShaders::SSCTVisibilityMap,
        { /* -- this shader has an empty vertex format -- */ }
    );

    visibilityPixelParam_ = visibilityMapShader_->CreateAndAttachConstantBuffer("pixelParam");

    visibilityMap_ = RenderSys()->CreateTexture2D(TextureFormats::R8UInt, GetResolution());
    //RenderSys()->ClearTexture(visibilityMap_.get(), Video::ColorRGBAf(1.0f));
}

void SSCT::CreateReflectionResources()
{
    #if 0
    reflectionShader_ = RenderSys()->GetShaderProvider()->CreateShaderCompositionFromResource(
        ShaderProvider::StandardShaders::SSCTReflection,
        { /* -- this shader has an empty vertex format -- */ }
    );
    #else
    reflectionShader_ = RenderSys()->GetShaderProvider()->CreateShaderCompositionFromFiles("SSCTReflection", {});
    #endif
    
    reflectionPixelParam_ = reflectionShader_->CreateAndAttachConstantBuffer("pixelParam");

    reflectionShader_->Attach(RenderSys()->GetRenderStateConstBuffer(), "invScreenParam");

    #ifdef _DEBUG_SSCT_
    
    _Deb_RayMarchFeedback = reflectionShader_->CreateAndAttachStorageBuffer("_DebugOutput_", HardwareBufferUsage::Dynamic, true);
    
    #endif
}

void SSCT::CreateRenderStates()
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

void SSCT::OnUpdateVisibilityPixelParam(const Texture2D::SizeType& mipSize, unsigned int mipLevel)
{
    SSCTShader::VisibilityMapPixelConstBuffer buffer;
    {
        buffer.mipLevel     = mipLevel;
        buffer.nearPlane    = RenderCtx()->GetRenderState().nearPlane;
        buffer.farPlane     = RenderCtx()->GetRenderState().farPlane;
    }
    RenderSys()->UpdateBuffer(visibilityPixelParam_.get(), buffer);
}

/*
This function sets the 'sample offset' for the Hi-Z MIP-map generation pass.
This is required to not omit minimal or maximal Z values in the respective MIP cell.
*/
void SSCT::OnUpdateHiZPixelParam(const Texture2D::SizeType& mipSize, unsigned int mipLevel)
{
    auto mipLevelSize = depthMap_->MIPLevelSize(mipLevel - 1);

    SSCTShader::HiZMapPixelConstBuffer buffer;
    {
        buffer.offset.x = (mipLevelSize.width  % 2 == 0 ? 1 : 2);
        buffer.offset.y = (mipLevelSize.height % 2 == 0 ? 1 : 2);
    }
    RenderSys()->UpdateBuffer(hiZPixelParam_.get(), buffer);
}

void SSCT::UpdateReflectionPixelConstBuffer()
{
    SSCTShader::ReflectionPixelConstBuffer buffer;
    {
        buffer.projectionMatrix     = projectionMatrix_;
        
        buffer.invProjectionMatrix  = projectionMatrix_;// * viewMatrix_;
        buffer.invProjectionMatrix.MakeInverse();
        buffer.viewMatrix           = viewMatrix_;
        
        buffer.mipCount             = Math::NumMIPLevelsBySize(GetResolution());
        buffer.resolution           = GetResolution().Cast<float>();
        buffer.globalReflectivity   = reflectivity;
        buffer.globalRoughness      = roughness;

        Video::Projectionf::ExtractClippingPlanesLH(projectionMatrix_, buffer.nearPlane, buffer.farPlane);

        #ifdef _DEBUG_SSCT_
        //buffer.__pad1 = _Deb_HiZEpsilon;
        buffer._deb_mousepos.x = static_cast<float>(_Deb_SamplePos.x) / GetResolution().width;
        buffer._deb_mousepos.y = static_cast<float>(_Deb_SamplePos.y) / GetResolution().height;
        buffer._deb_options.x = (_Deb_ShowRayMarchLevels ? 1 : 0);
        buffer._deb_options.y = (_Deb_RayMarching ? 1 : 0);
        buffer._deb_options.z = _Deb_DebugMap;
        #endif
    }
    RenderSys()->UpdateBuffer(reflectionPixelParam_.get(), buffer);
}


} // /namespace Video

} // /namespace Fork



// ========================