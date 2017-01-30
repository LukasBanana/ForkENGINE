/*
 * Custom MIP-maps generator header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_CUSTOM_MIP_MAPS_GENERATOR_H__
#define __FORK_CUSTOM_MIP_MAPS_GENERATOR_H__


#include "Core/DeclPtr.h"
#include "Core/Export.h"
#include "Video/RenderSystem/Shader/ShaderComposition.h"
#include "Video/RenderSystem/Texture/Texture2D.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(CustomMIPMapsGenerator);

/**
Custom MIP-maps generator helper class.
This class provides standard shaders to generate custom MIP-maps.
Hi-Z (hierarchical Z) maps can be generated as well.
*/
class FORK_EXPORT CustomMIPMapsGenerator
{
    
    public:
        
        //! Hi-Z sample modes.
        enum class HiZSampleModes
        {
            Minimize = 0,   //!< Down-samples four texels by taking the minimal value.
            Maximize,       //!< Down-samples four texels by taking the maximal value.
        };

        CustomMIPMapsGenerator();

        /**
        Generates the MIP levels of the specified texture.
        \param[in,out] texture Raw pointer tot he texture whose MIP-maps are to be generated.
        \param[in] layer Specifies the texture layer. For more information
        about this parameter see 'RenderContext::GenerateMIPMaps'.
        \see RenderContext::GenerateMIPMaps(Texture2D*, const ShaderComposition*, unsigned int)
        */
        void GenerateMIPMaps(Texture2D* texture, unsigned int layer = 0);

        /**
        Generates the MIP levels of the specified depth texture.
        \param[in,out] depthTexture Raw pointer to the depth texture
        whose MIP-maps are to be generated in a hierarchical Z fashion.
        \param[in] sampleMode Specifies the down-sampling mode.
        By default HiZSampleModes::Minimize.
        \see HiZSampleModes
        \see RenderContext::GenerateMIPMaps(Texture2D*, const ShaderComposition*, unsigned int)
        */
        void GenerateHiZMaps(
            Texture2D* depthTexture,
            const HiZSampleModes sampleMode = HiZSampleModes::Minimize
        );

    private:
        
        void CreateShaders();
        
        void SetupHiZSampleMode(int sampleMode);
        void SetupTextureLayer(int layer);

        ShaderCompositionPtr    customMIPMapsShader_;
        ConstantBufferPtr       customMIPMapsConstBuffer_;

        ShaderCompositionPtr    hizMIPMapsShader_;
        ConstantBufferPtr       hizMIPMapsConstBuffer_;

        int                     prevHiZSampleMode_ = -1;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================