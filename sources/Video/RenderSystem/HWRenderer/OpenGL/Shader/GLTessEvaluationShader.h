/*
 * OpenGL tessellation evaluation shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_TESS_EVALUATION_SHADER_H__
#define __FORK_GL_TESS_EVALUATION_SHADER_H__


#include "Video/RenderSystem/Shader/TessEvaluationShader.h"
#include "GLShaderBase.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(GLTessEvaluationShader);

//! OpenGL tessellation evaluation shader implementation.
class GLTessEvaluationShader : public TessEvaluationShader, public GLShaderBase
{
    
    public:
        
        GLTessEvaluationShader();

        GLTessEvaluationShader(const GLTessEvaluationShader&) = delete;
        GLTessEvaluationShader& operator = (const GLTessEvaluationShader&) = delete;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================