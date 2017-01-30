/*
 * SSLR post processor header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_POST_PROCESSOR_SSLR_H__
#define __FORK_POST_PROCESSOR_SSLR_H__


#include "Video/AdvancedRenderer/PostProcessor/PostProcessor.h"
#include "Video/RenderSystem/Shader/ShaderComposition.h"
#include "Video/RenderSystem/Texture/Texture2D.h"
#include "Video/RenderSystem/Texture/RenderTarget.h"
#include "Video/RenderSystem/RenderState/SamplerState.h"
#include "Math/Core/Matrix4.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(SSLR);

/**
Screen space local reflections (SSLR) post processor.
This can be used for local specular and glossy reflections. The effect is similar to SSCT.
\ingroup postprocessor
\see SSCT
*/
class FORK_EXPORT SSLR : public PostProcessor
{
    
    public:
        
        /* === Functions === */

        SSLR(const Math::Size2i& initResolution);

        //! Returns "SSLR".
        std::string Name() const override;

        void Draw(RenderTarget* renderTarget) override;

        /**
        Initializes this post processing effect.
        \param[in] colorMap Specifies the color map from which the reflections will be read.
        \param[in] normalMap Specifies the normal map.
        \param[in] depthMap Specifies the linear depth map. Each pixel of this texture stores the linear depth,
        or rather the distance between the view camera and that pixel in world space.
        \param[in] specularMap Specifies the specular (or rather gloss) map,
        which describes how intensely the glossiness is for each pixel on the screen.
        \note This must be called before the effect can be drawn!
        \see Draw
        */
        void Setup(
            Texture2D* colorMap,
            const Texture2D* normalMap,
            Texture2D* depthMap,
            const Texture2D* specularMap
        );

        /**
        Sets the view's projection. This is used to transform the view ray in the reflection shader.
        \note This must be a left-handed projection matrix, because the actual projection
        is done in a pixel shader and not by the hardware rasterizer!
        */
        void SetupProjection(const Math::Matrix4f& projectionMatrix);
        /**
        Sets the view matrix.
        \see Scene::CameraNode::GetViewMatrix
        */
        void SetupView(const Math::Matrix4f& viewMatrix);

        /**
        Specifies the global reflectivity factor for the SSLR effect.
        This is typically in the range [0.0 .. 1.0]. By default 1.0.
        */
        float reflectivity = 1.0f;
        /**
        Specifiefs the global roughness factor for the SSLR effect.
        This is typically in the range [0.0 .. 1.0]. By default 0.0.
        */
        float roughness = 0.0f;

    private:
        
        /* === Functions === */

        void OnUpdateResolution() override;

        void CreateRayTraceResources();
        void CreateBlurResources();
        void CreateReflectionResources();
        void CreateRenderStates();

        void UpdateRayTracePixelConstBuffer();
        void UpdateReflectionPixelConstBuffer();

        /* === Members === */

        ShaderCompositionPtr    rayTraceShader_;        //!< Ray tracing shader to produce the ray-trace- and mask buffers.
        ShaderCompositionPtr    blurHorzShader_;        //!< Horizontal blur shader to blur the ray-trace- and mask buffers.
        ShaderCompositionPtr    blurVertShader_;        //!< Vertical blur shader to blur ray-trace- and mask buffers.
        ShaderCompositionPtr    reflectionShader_;      //!< Reflection shader for the final reflections.

        ConstantBufferPtr       rayTracePixelParam_;
        ConstantBufferPtr       reflectionPixelParam_;

        Texture2D*              colorMap_       = nullptr;
        const Texture2D*        normalMap_      = nullptr;
        Texture2D*              depthMap_       = nullptr;
        const Texture2D*        specularMap_    = nullptr;

        Texture2DPtr            rayTraceColorBuffer_;
        Texture2DPtr            maskBuffer_;

        RenderTargetPtr         rayTraceRenderTarget_;

        SamplerStatePtr         nearestSampler_;
        SamplerStatePtr         trilinearSampler_;

        Math::Matrix4f          projectionMatrix_;
        Math::Matrix4f          viewMatrix_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================