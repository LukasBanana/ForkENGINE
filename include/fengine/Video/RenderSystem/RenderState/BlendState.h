/*
 * Blend state header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_BLEND_STATE_H__
#define __FORK_BLEND_STATE_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Video/Core/ColorRGBA.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(BlendState);

//! Blend state interface.
class FORK_EXPORT BlendState
{
    
    public:
        
        //! Blending operations.
        enum class BlendOps
        {
            Zero,
            One,
            
            SrcColor,
            InvSrcColor,
            SrcAlpha,
            InvSrcAlpha,

            DestColor,
            InvDestColor,
            DestAlpha,
            InvDestAlpha,
        };

        //! Blend state description structure.
        struct FORK_EXPORT Description
        {
            bool        isBlendEnabled  = true;                     //!< Specifies whether blending is enabled or not. By default true.
            BlendOps    srcBlend        = BlendOps::SrcAlpha;       //!< Source color blend operation.
            BlendOps    destBlend       = BlendOps::InvSrcAlpha;    //!< Destination color blend operation.
            BlendOps    srcBlendAlpha   = BlendOps::SrcAlpha;       //!< Source alpha blend operation. \note Blend operations which end with "Color" are not allowed here.
            BlendOps    destBlendAlpha  = BlendOps::InvSrcAlpha;    //!< Desination alpha blend operation. \note Blend operations which end with "Color" are not allowed here.
            ColorRGBAb  colorMask;                                  //!< Boolean color mask. By default { true, true, true, true }.
        };

        virtual ~BlendState()
        {
        }

        BlendState(const BlendState&) = delete;
        BlendState& operator = (const BlendState&) = delete;

        //! Returns the blend state description.
        inline const Description& GetDesc() const
        {
            return desc_;
        }

    protected:
        
        BlendState(const Description& desc) :
            desc_{ desc }
        {
        }

        Description desc_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================