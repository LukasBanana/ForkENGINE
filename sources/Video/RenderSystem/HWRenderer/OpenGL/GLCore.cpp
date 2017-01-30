/*
 * OpenGL core function file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLCore.h"
#include "GLExtensions.h"
#include "Core/StringModifier.h"
#include "Core/StaticConfig.h"
#include "IO/Core/Log.h"
#include "Video/RenderSystem/RenderContext.h"
#include "Video/RenderSystem/RenderSystem.h"


namespace Fork
{

std::string ToStr(const GLubyte* str)
{
    #ifdef FORK_DEBUG
    if (!str)
    {
        FORK_DEBUG_BREAK
    }
    #endif
    return std::string(reinterpret_cast<const char*>(str));
}

namespace Video
{


/* --- Shared intermediate resources --- */

GLuint intermediateFramebuffer = 0;

GLStateManager* activeStateMngr = nullptr;


/* --- Functions --- */

void CreateSharedResource()
{
    glGenFramebuffers(1, &intermediateFramebuffer);
}

void DeleteSharedResources()
{
    glDeleteFramebuffers(1, &intermediateFramebuffer);
}

std::string GLErrorToStr(const GLenum errorCode)
{
    if (errorCode != GL_NO_ERROR)
    {
        switch (errorCode)
        {
            /* --- Errors produced by "glGetError" --- */
            case GL_INVALID_ENUM:
                return "Invalid Enumeration";
            case GL_INVALID_VALUE:
                return "Invalid Value";
            case GL_INVALID_OPERATION:
                return "Invalid Operation";
            case GL_INVALID_FRAMEBUFFER_OPERATION:
                return "Invalid Framebuffer Operation";
            case GL_OUT_OF_MEMORY:
                return "Out Of Memory";
            case GL_STACK_OVERFLOW:
                return "Stack Overflow";
            case GL_STACK_UNDERFLOW:
                return "Stack Underflow";

            /* --- Errors produced by "glCheckFramebufferStatus" --- */
            case GL_FRAMEBUFFER_UNDEFINED:
                return "Framebuffer Undefined";
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                return "Framebuffer Incomplete Attachment";
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                return "Framebuffer Incomplete Missing Attachment";
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                return "Framebuffer Incomplete Draw Buffer";
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                return "Framebuffer Incomplete Read Buffer";
            case GL_FRAMEBUFFER_UNSUPPORTED:
                return "Framebuffer Unsupported";
            case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                return "Framebuffer Incomplete Multisample";
            case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                return "Framebuffer Incomplete Layer Targets";

            default:
                return "Unknown (Code = " + ToStr(errorCode) + ")";
        }
    }
    return "";
}

std::string GLDebugSourceToStr(const GLenum source)
{
    switch (source)
    {
        case GL_DEBUG_SOURCE_API:
            return "API";
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            return "Window System";
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            return "Shader Compiler";
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            return "Third Party";
        case GL_DEBUG_SOURCE_APPLICATION:
            return "Application";
        case GL_DEBUG_SOURCE_OTHER:
            return "Other";
    }
    return "";
}

std::string GLDebugTypeToStr(const GLenum type)
{
    switch (type)
    {
        case GL_DEBUG_TYPE_ERROR:
            return "Error";
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            return "Deprecated Behavior";
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            return "Undefined Behavior";
        case GL_DEBUG_TYPE_PORTABILITY:
            return "Portability";
        case GL_DEBUG_TYPE_PERFORMANCE:
            return "Performance";
        case GL_DEBUG_TYPE_MARKER:
            return "Marker";
        case GL_DEBUG_TYPE_PUSH_GROUP:
            return "Push Group";
        case GL_DEBUG_TYPE_POP_GROUP:
            return "Pop Group";
        case GL_DEBUG_TYPE_OTHER:
            return "Other";
    }
    return "";
}

std::string GLDebugSeverityToStr(const GLenum severity)
{
    switch (severity)
    {
        case GL_DEBUG_SEVERITY_HIGH:
            return "High";
        case GL_DEBUG_SEVERITY_MEDIUM:
            return "Medium";
        case GL_DEBUG_SEVERITY_LOW:
            return "Low";
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            return "Notification";
    }
    return "";
}

void GLAdjustString(std::string& str)
{
    while (!str.empty() && str.back() == '\0')
        str.pop_back();
}

void GLSetState(GLenum cap, bool enable)
{
    if (enable)
        glEnable(cap);
    else
        glDisable(cap);
}

#ifdef FORK_DEBUG

void GLShowError(const std::string& operationDesc)
{
    static const int debugMessageLimit = 10;

    auto err = glGetError();

    if (err != GL_NO_ERROR)
    {
        IO::Log::Debug("GL Error", operationDesc, GLErrorToStr(err), debugMessageLimit);
        //FORK_DEBUG_BREAK;
    }
}

void GLFlushError()
{
    /* Ignore GL error here to flush it */
    glGetError();
}

void GLEnsureSharedContext(const std::string& operationDesc)
{
    auto renderSystem = RenderSystem::Active();
    if ( !renderSystem || !renderSystem->GetSharedRenderContext() || !renderSystem->GetSharedRenderContext()->IsActivated())
    {
        IO::Log::Debug("GL Error", operationDesc, "Shared render context is not active");
        //FORK_DEBUG_BREAK;
    }
}

void GLEnsureSharedContext(const std::string& operationDesc, GLuint bufferID)
{
    GLEnsureSharedContext(operationDesc + "(" + ToStr(bufferID) + ")");
}

#endif


} // /namespace Video

} // /namespace Fork



// ========================