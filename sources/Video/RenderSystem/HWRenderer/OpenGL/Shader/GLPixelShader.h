/*
 * OpenGL pixel shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_PIXEL_SHADER_H__
#define __FORK_GL_PIXEL_SHADER_H__


#include "Video/RenderSystem/Shader/PixelShader.h"
#include "GLShaderBase.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(GLPixelShader);

//! OpenGL pixel shader implementation.
class GLPixelShader : public PixelShader, public GLShaderBase
{
    
    public:
        
        GLPixelShader();

        GLPixelShader(const GLPixelShader&) = delete;
        GLPixelShader& operator = (const GLPixelShader&) = delete;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================