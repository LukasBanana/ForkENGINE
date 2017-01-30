/*
 * OpenGL tessellation control shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_TESS_CONTROL_SHADER_H__
#define __FORK_GL_TESS_CONTROL_SHADER_H__


#include "Video/RenderSystem/Shader/TessControlShader.h"
#include "GLShaderBase.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(GLTessControlShader);

//! OpenGL tessellation control shader implementation.
class GLTessControlShader : public TessControlShader, public GLShaderBase
{
    
    public:
        
        GLTessControlShader();

        GLTessControlShader(const GLTessControlShader&) = delete;
        GLTessControlShader& operator = (const GLTessControlShader&) = delete;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================