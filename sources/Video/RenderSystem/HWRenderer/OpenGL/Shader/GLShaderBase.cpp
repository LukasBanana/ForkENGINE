/*
 * OpenGL shader base file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLShaderBase.h"
#include "../HardwareBuffer/GLConstantBuffer.h"

#include "../GLExtensions.h"
#include "../GLParamMapper.h"
#include "../GLCore.h"


namespace Fork
{

namespace Video
{


GLShaderBase::GLShaderBase(const Shader::Types type)
{
    shaderID_ = glCreateShader(GLParamMapper::Map(type));
    ENSURE_GL_SHARED_CONTEXT_FOR_BUFFER("glCreateShader", shaderID_);
}
GLShaderBase::~GLShaderBase()
{
    ENSURE_GL_SHARED_CONTEXT_FOR_BUFFER("glDeleteShader", shaderID_);
    glDeleteShader(shaderID_);
}

bool GLShaderBase::Compile(const std::vector<std::string>& shaderSource, std::string& infoLog)
{
    SetupShaderSource(shaderSource);
    Compile();
    return QueryInfoLog(infoLog);
}


/*
 * ======= Private: =======
 */

void GLShaderBase::SetupShaderSource(const std::vector<std::string>& shaderSource)
{
    /* Get shader source raw pointers */
    const size_t numLines = shaderSource.size();

    std::vector<const GLchar*> sources(numLines);

    for (size_t i = 0, n = numLines; i < n; ++i)
        sources[i] = shaderSource[i].c_str();

    /* Setup sources to shader object */
    glShaderSource(shaderID_, numLines, sources.data(), nullptr);
}

void GLShaderBase::Compile()
{
    FLUSH_GL_ERROR;

    /* Compile shader */
    glCompileShader(shaderID_);

    SHOW_GL_ERROR("glCompileShader");
}

bool GLShaderBase::QueryInfoLog(std::string& infoLog)
{
    /* Query compilation status */
    GLint compileStatus = 0;
    glGetShaderiv(shaderID_, GL_COMPILE_STATUS, &compileStatus);

    /* Query info log length */
    GLint infoLogLength = 0;
    glGetShaderiv(shaderID_, GL_INFO_LOG_LENGTH, &infoLogLength);

    if (infoLogLength > 0)
    {
        /* Query info log output */
        GLsizei charsWritten = 0;
        infoLog.resize(infoLogLength);

        glGetShaderInfoLog(shaderID_, infoLogLength, &charsWritten, &infoLog[0]);

        GLAdjustString(infoLog);
    }
    else
        infoLog.clear();

    return compileStatus == GL_TRUE;
}


} // /namespace Video

} // /namespace Fork



// ========================