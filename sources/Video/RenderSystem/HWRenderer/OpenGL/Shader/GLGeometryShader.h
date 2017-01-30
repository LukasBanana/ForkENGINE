/*
 * OpenGL geometry shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_GEOMETRY_SHADER_H__
#define __FORK_GL_GEOMETRY_SHADER_H__


#include "Video/RenderSystem/Shader/GeometryShader.h"
#include "GLShaderBase.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(GLGeometryShader);

//! OpenGL geometry shader implementation.
class GLGeometryShader : public GeometryShader, public GLShaderBase
{
    
    public:
        
        GLGeometryShader();

        GLGeometryShader(const GLGeometryShader&) = delete;
        GLGeometryShader& operator = (const GLGeometryShader&) = delete;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================