/*
 * Sampler state header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SAMPLER_STATE_H__
#define __FORK_SAMPLER_STATE_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(SamplerState);

/**
Sampler state interface. Sampler states can be used inside a shader
to provide different methods of texture sampling.
*/
class FORK_EXPORT SamplerState
{
    
    public:
        
        //! Texture coordinate wrap modes.
        enum class WrapModes
        {
            Repeat, //!< The texture coordinates are repeated, i.e. they are wrapped to the half open range [0.0 .. 1.0).
            Mirror, //!< The texture coordinates are mirrored.
            Clamp,  //!< The texture coordinates are clamped to the range [0.0 .. 1.0].
        };

        //! Texture sampling filters.
        enum class Filters
        {
            Nearest,    //!< Nearest pixels will be choosen. Looks worse but is faster.
            Linear,     //!< Pixels will be linear interpolated. Looks better but is slower.
        };

        //! Wrap modes for texture coordinates U, V and W.
        struct WrapModesUVW
        {
            WrapModesUVW(const WrapModes wrapMode) :
                u{ wrapMode },
                v{ wrapMode },
                w{ wrapMode }
            {
            }
            WrapModesUVW(const WrapModes wrapModeU, const WrapModes wrapModeV, const WrapModes wrapModeW) :
                u{ wrapModeU },
                v{ wrapModeV },
                w{ wrapModeW }
            {
            }

            WrapModes u, v, w;
        };

        //! Sampler state description structure.
        struct FORK_EXPORT Description
        {
            WrapModesUVW    wrapMode    = WrapModes::Repeat;    //!< Texture coordinate wrap mode. By default WrapModes::Repeat.
            Filters         minFilter   = Filters::Linear;      //!< Minification filter. By default Filters::Linear.
            Filters         magFilter   = Filters::Linear;      //!< Magnification filter. By default Filters::Linear.
            Filters         mipFilter   = Filters::Linear;      //!< MIP-map filter. By default Filters::Linear.
            unsigned int    anisotropy  = 1;                    //!< Maximal anisotropy. This must be in the range [1 .. 16]. By default 1.
            bool            useMIPMaps  = true;                 //!< Specifies whether MIP maps are used or not. By default true.
            float           mipLODBias  = 0.0f;                 //!< MIP-map LOD (Level Of Detail) bias. This will be added to the pre-computed MIP level. By default 0.0.
            float           minLOD      = -1000.0f;             //!< Minimal LOD (Level Of Detail) value for MIP-map selection. By default -1000.0.
            float           maxLOD      = 1000.0f;              //!< Maximal LOD (Level Of Detail) value for MIP-map selection. By default 1000.0.
        };

        virtual ~SamplerState()
        {
        }

        SamplerState(const SamplerState&) = delete;
        SamplerState& operator = (const SamplerState&) = delete;

        //! Returns the blend state description.
        inline const Description& GetDesc() const
        {
            return desc_;
        }

    protected:
        
        SamplerState(const Description& desc) :
            desc_{ desc }
        {
        }

        Description desc_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================