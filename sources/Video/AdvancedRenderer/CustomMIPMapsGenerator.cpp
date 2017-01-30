/*
 * Custom MIP-maps generator file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/AdvancedRenderer/CustomMIPMapsGenerator.h"
#include "Video/RenderSystem/Shader/StandardShader/HiZMIPMapsConstantBuffers.h"
#include "Video/RenderSystem/Shader/StandardShader/CustomMIPMapsConstantBuffers.h"
#include "Video/RenderSystem/RenderContext.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Video/RenderSystem/Shader/ShaderProvider.h"
#include "../RenderSystem/RenderSysCtx.h"


namespace Fork
{

namespace Video
{


using namespace StandardShader;

CustomMIPMapsGenerator::CustomMIPMapsGenerator()
{
    CreateShaders();
}

void CustomMIPMapsGenerator::GenerateMIPMaps(Texture2D* texture, unsigned int layer)
{
    /* Generate MIP-maps with custom shader */
    SetupTextureLayer(static_cast<int>(layer));
    RenderCtx()->GenerateMIPMaps(texture, customMIPMapsShader_.get(), layer);
}

void CustomMIPMapsGenerator::GenerateHiZMaps(Texture2D* depthTexture, const HiZSampleModes sampleMode)
{
    /* Generate MIP-maps with custom shader */
    SetupHiZSampleMode(static_cast<int>(sampleMode));
    RenderCtx()->GenerateMIPMaps(depthTexture, hizMIPMapsShader_.get());
}


/*
 * ======= Private: =======
 */

void CustomMIPMapsGenerator::CreateShaders()
{
    /* Create custom MIP-maps shader */
    customMIPMapsShader_ = RenderSys()->GetShaderProvider()->CreateShaderCompositionFromResource(
        ShaderProvider::StandardShaders::CustomMIPMaps,
        { /* -- this shader has an empty vertex format -- */ }
    );
    customMIPMapsConstBuffer_ = customMIPMapsShader_->CreateAndAttachConstantBuffer("configParam", "CustomMIPMaps");

    /* Create Hi-Z MIP-maps shader */
    hizMIPMapsShader_ = RenderSys()->GetShaderProvider()->CreateShaderCompositionFromResource(
        ShaderProvider::StandardShaders::HiZMIPMaps,
        { /* -- this shader has an empty vertex format -- */ }
    );
    hizMIPMapsConstBuffer_ = hizMIPMapsShader_->CreateAndAttachConstantBuffer("configParam", "HiZMIPMaps");
}

void CustomMIPMapsGenerator::SetupHiZSampleMode(int sampleMode)
{
    if (prevHiZSampleMode_ != sampleMode)
    {
        prevHiZSampleMode_ = sampleMode;

        /* Update config constant buffer */
        HiZMIPMapsShader::ConfigConstBuffer constBuffer;
        {
            constBuffer.sampleMode = sampleMode;
        }
        RenderSys()->UpdateBuffer(hizMIPMapsConstBuffer_.get(), constBuffer);
    }
}

void CustomMIPMapsGenerator::SetupTextureLayer(int layer)
{
    /* Update config constant buffer */
    CustomMIPMapsShader::ConfigConstBuffer constBuffer;
    {
        constBuffer.layer = layer;
    }
    RenderSys()->UpdateBuffer(customMIPMapsConstBuffer_.get(), constBuffer);
}


} // /namespace Video

} // /namespace Fork



// ========================