/*
 * OpenGL sampler state header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_SAMPLER_STATE_H__
#define __FORK_GL_SAMPLER_STATE_H__


#include "Video/RenderSystem/RenderState/SamplerState.h"
#include "../GLImport.h"


namespace Fork
{

namespace Video
{


class GLSamplerState : public SamplerState
{
    
    public:
        
        GLSamplerState(const Description& desc);
        ~GLSamplerState();

        void ChangeDesc(const Description& desc);

        void Bind(unsigned int layer) const;
        void Unbind(unsigned int layer) const;

        inline GLuint GetSamplerID() const
        {
            return samplerID_;
        }

    private:
        
        Description InitGLDesc() const;

        GLenum samplerID_ = 0;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================