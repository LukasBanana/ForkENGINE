/*
 * OpenGL shader provider header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_SHADER_PROVIDER_H__
#define __FORK_GL_SHADER_PROVIDER_H__


#include "Video/RenderSystem/Shader/ShaderProvider.h"


namespace Fork
{

namespace Video
{


//! OpenGL shader provider implementation.
class GLShaderProvider : public ShaderProvider
{

    public:
        
        GLShaderProvider() = default;

        GLShaderProvider(const GLShaderProvider&) = delete;
        GLShaderProvider& operator = (const GLShaderProvider&) = delete;

    private:
        
        const char* QueryVertexShaderFromResource(const StandardShaders shaderName) const override;
        const char* QueryPixelShaderFromResource(const StandardShaders shaderName) const override;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================