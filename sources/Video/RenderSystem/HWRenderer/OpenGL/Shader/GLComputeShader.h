/*
 * OpenGL compute shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_COMPUTE_SHADER_H__
#define __FORK_GL_COMPUTE_SHADER_H__


#include "Video/RenderSystem/Shader/ComputeShader.h"
#include "GLShaderBase.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(GLComputeShader);

//! OpenGL compute shader implementation.
class GLComputeShader : public ComputeShader, public GLShaderBase
{
    
    public:
        
        GLComputeShader();

        GLComputeShader(const GLComputeShader&) = delete;
        GLComputeShader& operator = (const GLComputeShader&) = delete;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================