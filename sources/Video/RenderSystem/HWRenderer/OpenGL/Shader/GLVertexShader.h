/*
 * OpenGL vertex shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_VERTEX_SHADER_H__
#define __FORK_GL_VERTEX_SHADER_H__


#include "Video/RenderSystem/Shader/VertexShader.h"
#include "GLShaderBase.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(GLVertexShader);

//! OpenGL vertex shader implementation.
class GLVertexShader : public VertexShader, public GLShaderBase
{
    
    public:
        
        GLVertexShader();

        GLVertexShader(const GLVertexShader&) = delete;
        GLVertexShader& operator = (const GLVertexShader&) = delete;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================