/*
 * Shading model material header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SHADING_MODEL_MATERIAL_H__
#define __FORK_SHADING_MODEL_MATERIAL_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Video/Core/ColorRGBA.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(Material);

//! Material interface for the shading models.
class FORK_EXPORT Material
{
    
    public:
        
        //! Material types.
        enum class Types
        {
            /* --- Internal base types --- */
            BlinnPhong,         //!< Blinn-phong shading model material. \see BlinnPhongMaterial
            CookTorrance,       //!< Cook-torrance shading model material. \see CookTorranceMaterial

            /* --- Internal hierarchical types --- */
            Relief,             //!< Relief mapping material. \see ReliefMaterial

            /* --- Custom types --- */
            CustomA,            //!< Custom type, can be used for custom material class.
            CustomB,            //!< Custom type, can be used for custom material class.
            CustomC,            //!< Custom type, can be used for custom material class.
            CustomD,            //!< Custom type, can be used for custom material class.
            CustomE,            //!< Custom type, can be used for custom material class.
            CustomF,            //!< Custom type, can be used for custom material class.
        };

        virtual ~Material()
        {
        }

        /**
        Returns the material type.
        \see Types
        */
        virtual Types Type() const = 0;

        /**
        Sets the common material color.
        \param[in] color Specifies the material color.
        \see Color
        */
        virtual void SetupColor(const ColorRGBAf& color) = 0;
        /**
        Returns the common material color.
        \see SetupColor
        */
        virtual ColorRGBAf Color() const = 0;

        /**
        Sets the material roughness.
        \param[in] roughness Specififes the roughness factor. Must be in the range [0.0 .. 1.0].
        \remarks The initial roughness depends on the respective shading model.
        \see Roughness
        */
        virtual void SetupRoughness(float roughness) = 0;
        /**
        Returns the roughness factor in the range [0.0 .. 1.0].
        \see SetupRoughness
        */
        virtual float Roughness() const = 0;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================