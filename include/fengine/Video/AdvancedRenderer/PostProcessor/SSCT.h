/*
 * SSCT post processor header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_POST_PROCESSOR_SSCT_H__
#define __FORK_POST_PROCESSOR_SSCT_H__


#include "Video/AdvancedRenderer/PostProcessor/PostProcessor.h"
#include "Video/RenderSystem/Shader/ShaderComposition.h"
#include "Video/RenderSystem/Texture/Texture2D.h"
#include "Video/RenderSystem/Texture/RenderTarget.h"
#include "Video/RenderSystem/RenderState/SamplerState.h"
#include "Math/Core/Matrix4.h"

//#define _DEBUG_SSCT_//!!!

#ifdef _DEBUG_SSCT_
#include "Video/AdvancedRenderer/PrimitiveRenderer.h"
#include "Video/Core/Projection.h"
#endif


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(SSCT);

/**
Screen space cone tracing (SSCT) post processor.
This can be used for local specular and glossy reflections. The effect is similar to SSLR.
\ingroup postprocessor
\see SSLR
*/
class FORK_EXPORT SSCT : public PostProcessor
{
    
    public:
        
        /* === Functions === */

        //struct EnvironmentMapFallback {};//!!!

        SSCT(const Math::Size2i& initResolution);

        //! Returns "SSCT".
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
        \param[in] environmentMap Optional environment color map.
        This can be used as fallback method when reflections can not be reproduced in screen space.
        \note This must be called before the effect can be drawn!
        \see Draw
        \see TextureFormats
        */
        void Setup(
            Texture2D* colorMap,
            const Texture2D* normalMap,
            Texture2D* depthMap,
            const Texture2D* specularMap,
            const TextureCube* environmentMap = nullptr
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
        Specifies the global reflectivity factor for the SSCT effect.
        This is typically in the range [0.0 .. 1.0]. By default 1.0.
        */
        float reflectivity = 1.0f;
        /**
        Specifiefs the global roughness factor for the SSCT effect.
        This is typically in the range [0.0 .. 1.0]. By default 0.0.
        */
        float roughness = 0.0f;

        #ifdef _DEBUG_SSCT_//!!!
        
        void _Deb_DrawDebugInfo(
            PrimitiveRenderer* renderer, const TextureFont* font,
            const Math::Matrix4f& viewMatrix, const Projectionf& projection
        );

        bool _Deb_ReloadShaders();

        inline Texture2D* _Deb_GetVisibilityMap() const
        {
            return visibilityMap_.get();
        }
        inline Texture2D* _Deb_BluredColorMap() const
        {
            return _Deb_UseBlur ? bluredColorMap_.get() : colorMap_;
        }

        bool _Deb_RayMarching = false;
        bool _Deb_UseBlur = false;
        bool _Deb_ShowRayMarchLevels = false;

        int _Deb_DebugMap = 0;
        float _Deb_HiZEpsilon = 0.01f;
        Math::Point2i _Deb_SamplePos;

        #endif

    private:
        
        /* === Functions === */

        void OnUpdateResolution() override;

        void CreateConvolutionResources();
        void CreateHiZMapResources();
        void CreateVisibilityMapResources();
        void CreateReflectionResources();
        void CreateRenderStates();

        void OnUpdateVisibilityPixelParam(const Texture2D::SizeType& mipSize, unsigned int mipLevel);
        void OnUpdateHiZPixelParam(const Texture2D::SizeType& mipSize, unsigned int mipLevel);

        void UpdateReflectionPixelConstBuffer();

        /* === Members === */

        ShaderCompositionPtr    blurHorzShader_;        //!< Horizontal blur shader for the pre-convolution pass.
        ShaderCompositionPtr    blurVertShader_;        //!< Vertical blur shader for the pre-convolution pass.
        ShaderCompositionPtr    hiZMapShader_;          //!< Hi-Z map generation shader for the Hi-Z pass.
        ShaderCompositionPtr    visibilityMapShader_;   //!< Visibility map generation shader for the pre-integration pass.
        ShaderCompositionPtr    reflectionShader_;      //!< Reflection shader for the ray-casting and cone-tracing passes.

        ConstantBufferPtr       visibilityPixelParam_;
        ConstantBufferPtr       hiZPixelParam_;
        ConstantBufferPtr       reflectionPixelParam_;

        #ifdef _DEBUG_SSCT_
        StorageBufferPtr        _Deb_RayMarchFeedback;
        //Texture2DPtr            _Deb_RayCastColor;
        #endif

        Texture2D*              colorMap_       = nullptr;
        const Texture2D*        normalMap_      = nullptr;
        Texture2D*              depthMap_       = nullptr;
        const Texture2D*        specularMap_    = nullptr;
        const TextureCube*      environmentMap_ = nullptr;

        Texture2DPtr            visibilityMap_;
        Texture2DPtr            bluredColorMap_;

        SamplerStatePtr         nearestSampler_;
        SamplerStatePtr         trilinearSampler_;

        Math::Matrix4f          projectionMatrix_;
        Math::Matrix4f          viewMatrix_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================