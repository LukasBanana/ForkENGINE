/*
 * Relief material header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_RELIEF_MATERIAL_H__
#define __FORK_RELIEF_MATERIAL_H__


#include "Video/Material/Material.h"


namespace Fork
{

namespace Video
{


/**
Material implementation for relief mapping.
\remarks This is a hierarchical material.
\ingroup std_materials
*/
class FORK_EXPORT ReliefMaterial : public Material
{
    
    public:
        
        /* === Interface implementation === */

        //! Returns 'Types::Relief'.
        Types Type() const override;

        //! Sets the color of the actual material (if used).
        void SetupColor(const ColorRGBAf& color) override;
        //! Returns the color of the actual material (if used).
        ColorRGBAf Color() const override;

        //! Sets the roughness of the actual material (if used).
        void SetupRoughness(float roughness) override;
        //! Returns the roughness of the actual material (if used).
        float Roughness() const override;

        /* === Members === */

        //! Specifies the minimal relief map samples. By default 0.
        int         minSamples      = 0;
        
        //! Specifies the maximal relief map samples. By default 50.
        int         maxSamples      = 50;
        
        //! Specifies the height map scaling factor. By default 0.015.
        float       heightMapScale  = 0.015f;

        /**
        Specified the threshold for choosing the number of relief map samples
        in the range [minSamples .. maxSamples]. By default 2.0.
        */
        float       threshold       = 2.0f;

        /**
        This is the actual material object.
        This should be a base material (such as "BlinnPhongMaterial" or "CookTorranceMaterial").
        \see BlinnPhongMaterial
        \see CookTorranceMaterial
        */
        MaterialPtr actualMaterial;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================