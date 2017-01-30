/*
 * OpenGL blend state header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_BLEND_STATE_H__
#define __FORK_GL_BLEND_STATE_H__


#include "Video/RenderSystem/RenderState/BlendState.h"
#include "../GLImport.h"


namespace Fork
{

namespace Video
{


class GLBlendState : public BlendState
{
    
    public:
        
        GLBlendState(const Description& desc);

        void ChangeDesc(const Description& desc);

        inline GLenum GetSrcBlend() const
        {
            return srcBlend_;
        }
        inline GLenum GetDestBlend() const
        {
            return destBlend_;
        }
        inline GLenum GetSrcBlendAlpha() const
        {
            return srcBlendAlpha_;
        }
        inline GLenum GetDestBlendAlpha() const
        {
            return destBlendAlpha_;
        }

    private:
        
        GLenum srcBlend_        = GL_ONE;
        GLenum destBlend_       = GL_ZERO;
        GLenum srcBlendAlpha_   = GL_ONE;
        GLenum destBlendAlpha_  = GL_ZERO;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================