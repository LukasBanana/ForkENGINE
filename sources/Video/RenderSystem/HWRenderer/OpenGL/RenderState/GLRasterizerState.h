/*
 * OpenGL rasterizer state header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_RASTERIZER_STATE_H__
#define __FORK_GL_RASTERIZER_STATE_H__


#include "Video/RenderSystem/RenderState/RasterizerState.h"
#include "../GLImport.h"


namespace Fork
{

namespace Video
{


class GLRasterizerState : public RasterizerState
{
    
    public:
        
        GLRasterizerState(const Description& desc);

        void ChangeDesc(const Description& desc);

        inline GLenum GetFillMode() const
        {
            return fillMode_;
        }
        inline GLenum GetCullMode() const
        {
            return cullMode_;
        }

    private:
        
        GLenum fillMode_ = GL_FILL;
        GLenum cullMode_ = GL_BACK;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================