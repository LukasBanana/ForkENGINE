/*
 * OpenGL shader base header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_SHADER_BASE_H__
#define __FORK_GL_SHADER_BASE_H__


#include "Video/RenderSystem/Shader/Shader.h"
#include "../GLImport.h"


namespace Fork
{

namespace Video
{


//! OpenGL shader base class.
class GLShaderBase
{
    
    public:
        
        virtual ~GLShaderBase();

        GLShaderBase(const GLShaderBase&) = delete;
        GLShaderBase& operator = (const GLShaderBase&) = delete;

    protected:
        
        friend class GLShaderComposition;

        GLShaderBase(const Shader::Types type);

        bool Compile(const std::vector<std::string>& shaderSource, std::string& infoLog);

        inline GLuint GetShaderID() const
        {
            return shaderID_;
        }

    private:
        
        void SetupShaderSource(const std::vector<std::string>& shaderSource);
        void Compile();
        bool QueryInfoLog(std::string& infoLog);

        GLuint shaderID_ = 0;
        
};


} // /namespace Video

} // /namespace Fork


#endif



// ========================