/*
 * Gaussian blur post processor header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_POST_PROCESSOR_GAUSSIAN_BLUR_H__
#define __FORK_POST_PROCESSOR_GAUSSIAN_BLUR_H__


#include "Video/AdvancedRenderer/PostProcessor/PostProcessor.h"
#include "Video/RenderSystem/Shader/ShaderComposition.h"
#include "Video/RenderSystem/Shader/StandardShader/GaussianBlurConstantBuffers.h"
#include "Video/RenderSystem/Texture/Texture2D.h"
#include "Video/RenderSystem/Texture/RenderTarget.h"
#include "Video/RenderSystem/RenderState/SamplerState.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(GaussianBlur);

/**
Gaussian blur post processor. This can be used to blur an image smoothly.
\code
auto gaussianBlur = std::make_unique<Video::GaussianBlur>(screenResolution);
auto frameBufferTexture = renderSystem->CreateTexture2D(Video::TextureFormats::RGBA, screenResolution);
auto frameBufferTarget = renderSystem->CreateRenderTarget({ frameBufferTexture });
//...
renderSystem->Bind(frameBufferTarget.get());
{
    // Draw scene into render target ...
}
// Draw gaussian blur effect:
gaussianBlur->Draw(frameBufferTexture.get(), renderContext->GetMainRenderTarget());
\endcode
\remarks This is not the same as the bloom filter. The bloom filter blurs the image
(also with a gaussian blur) in a lower resolution.
\remarks Here is an example picture of this post-processing effect:
\image html images/GaussianBlur.Example.png
\ingroup postprocessor
*/
class FORK_EXPORT GaussianBlur : public PostProcessor
{
    
    public:
        
        /* === Structures === */

        //! Effect configuration structure.
        struct FORK_EXPORT Configuration
        {
            int     numSamples      = 1;    //!< Number of texture samples to blur the image. By default 1.
            float   stretchFactor   = 1.0f; //!< Stretch factor of the texture coordinates. By default 1.0.
            float   variance        = 0.1f; //!< Gaussian distribution variance. By default 0.1.
        };

        /* === Functions === */

        GaussianBlur(const Math::Size2i& initResolution);

        //! Returns "Gaussian Blur".
        std::string Name() const override;

        void Draw(RenderTarget* renderTarget) override;

        /**
        Initializes this post processing effect.
        \param[in] inputTexture Specifies the input texture which is to be blured.
        \remarks The input texture will not be modified, instead the blurred version
        will be rendered to the render target, specified in the 'Draw' function.
        \note This must be called before the effect can be drawn!
        \see Draw
        */
        void Setup(const Texture2D* inputTexture);

        /**
        Changes the effect configuration.
        \param[in] numSamples Specifies the number of texture samples to blur the output image.
        This value will be clamped to the range [1 .. GaussianBlurConfigConstBuffer::maxNumSamples]. By default 8.
        \param[in] stretchFactor Specifies the factor which is used to stretch the blur texture samples.
        Values greater than 1.0 may produce a lower quality but a larger blur. By default 1.0.
        \param[in] variance Specifies the variance of the gaussian distribution.
        A large value (e.g. 10+) will converge the blur to a box filter (i.e. less smooth). By default 0.1.
        \see GaussianBlurConfigConstBuffer::maxNumSamples
        */
        void ChangeConfig(int numSamples, float stretchFactor = 1.0f, float variance = 0.1f);

        //! Returns the effect configuration.
        inline const Configuration& GetConfig() const
        {
            return config_;
        }

    private:
        
        static const int renderPassHorz = 0;
        static const int renderPassVert = 1;

        /* === Functions === */

        void OnUpdateResolution() override;

        void CreateShader();
        void CreateRenderTarget();
        void CreateSamplerState();

        void UpdateTextureSize();
        void UpdateConfigConstantBuffer();

        void SetupRenderPass(int renderPass);
        void DrawRenderPass(const Texture2D* inputTexture, RenderTarget* renderTarget, int renderPass);

        void ComputeWeights(int numSamples, float variance);
        void ComputeOffsets(int numSamples, float stretchFactor);

        float HalfSize(int numSamples) const;
        float GaussianValue(float pos, float mean, float stdDeviation) const;

        /* === Members === */

        ShaderCompositionPtr    shader_;

        ConstantBufferPtr       constBufferConfig_;
        ConstantBufferPtr       constBufferRenderPass_;

        StandardShader::GaussianBlurShader::ConfigConstBuffer constBufferConfigData_;

        const Texture2D*        inputTexture_ = nullptr;
        Texture2DPtr            tempTexture_;           //!< Temporary texture for a separate render pass.
        RenderTargetPtr         tempRenderTarget_;      //!< Render target for the separate render pass.
        SamplerStatePtr         clampedSamplerState_;

        Configuration           config_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================