/*
 * OpenGL depth stencil state header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_DEPTH_STENCIL_STATE_H__
#define __FORK_GL_DEPTH_STENCIL_STATE_H__


#include "Video/RenderSystem/RenderState/DepthStencilState.h"
#include "../GLImport.h"


namespace Fork
{

namespace Video
{


class GLDepthStencilState : public DepthStencilState
{
    
    public:
        
        struct GLStencilOpDescription
        {
            GLenum stencilFailOp    = GL_KEEP;
            GLenum stencilZFailOp   = GL_KEEP;
            GLenum stencilZPassOp   = GL_KEEP;
            GLenum stencilFunc      = GL_LEQUAL;
        };

        GLDepthStencilState(const Description& desc);

        void ChangeDesc(const Description& desc);

        inline GLenum GetDepthFunc() const
        {
            return depthFunc_;
        }

        inline const GLStencilOpDescription& GetFrontFace() const
        {
            return frontFace_;
        }
        inline const GLStencilOpDescription& GetBackFace() const
        {
            return backFace_;
        }

    private:
        
        GLenum depthFunc_ = GL_LEQUAL;
        GLStencilOpDescription frontFace_, backFace_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================