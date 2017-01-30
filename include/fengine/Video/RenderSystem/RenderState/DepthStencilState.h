/*
 * Depth stencil state header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_DEPTH_STENCIL_STATE_H__
#define __FORK_DEPTH_STENCIL_STATE_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Video/RenderSystem/RenderState/CompareFunctions.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(DepthStencilState);

//! Depth-stencil state interface.
class FORK_EXPORT DepthStencilState
{
    
    public:
        
        //! Stencil operations.
        enum class StencilOps
        {
            Keep,
            Zero,
            Replace,
            Inc,
            IncWrap,
            Dec,
            DecWrap,
            Invert,
        };

        //! Stencil operation description structure.
        struct FORK_EXPORT StencilOpDescription
        {
            StencilOps          stencilFailOp   = StencilOps::Keep;
            StencilOps          stencilZFailOp  = StencilOps::Keep;
            StencilOps          stencilZPassOp  = StencilOps::Keep;
            CompareFunctions    stencilFunc     = CompareFunctions::Always;
        };

        //! Depth-stencil state description structure.
        struct FORK_EXPORT Description
        {
            bool                    isDepthTestEnabled      = true;
            bool                    isDepthWriteEnabled     = true;

            CompareFunctions        depthFunc               = CompareFunctions::Less;

            //! Enables or disables stencil test.
            bool                    isStencilTestEnabled    = false;
            //! Specifies whether front- and back faces are separated for stencil operations.
            bool                    isStencilSeparated      = false;

            unsigned char           stencilReadMask         = 0;
            unsigned char           stencilWriteMask        = 0;

            StencilOpDescription    frontFace;
            StencilOpDescription    backFace;
        };

        virtual ~DepthStencilState()
        {
        }

        DepthStencilState(const DepthStencilState&) = delete;
        DepthStencilState& operator = (const DepthStencilState&) = delete;

        //! Returns the depth-stencil state description.
        inline const Description& GetDesc() const
        {
            return desc_;
        }

    protected:
        
        DepthStencilState(const Description& desc) :
            desc_{ desc }
        {
        }

        Description desc_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================