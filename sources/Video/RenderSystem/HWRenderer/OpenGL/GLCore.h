/*
 * OpenGL core function header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_CORE_H__
#define __FORK_GL_CORE_H__


#include "GLImport.h"
#include "Core/StaticConfig.h"

#include <string>


namespace Fork
{

//! Extension to the other "ToStr" functions.
std::string ToStr(const GLubyte* str);

namespace Video
{


#define CAST_TO_GL_OBJECT(type, obj) CastToConcreteObject<GL##type>(obj, __FUNCTION__, #type, "GL" #type)

/* --- Shared intermediate resources --- */

/**
This GL framebuffer object is used for intermediate
drawing operations, such as custom MIP-map generation.
Instead of a "GLFrameBuffer" instance, a raw GL object ID
is used for optimization purposes.
*/
extern GLuint intermediateFramebuffer;

//! Reference to the active GL state manager.
class GLStateManager;
extern GLStateManager* activeStateMngr;


/* --- Functions --- */

//! Creates all shared intermediate resources.
void CreateSharedResource();
//! Delete all shared intermediate resources.
void DeleteSharedResources();

//! Converts the GL error code into a string.
std::string GLErrorToStr(const GLenum errorCode);

//! Converts the GL debug source into a string.
std::string GLDebugSourceToStr(const GLenum source);
//! Converts the GL debug type into a string.
std::string GLDebugTypeToStr(const GLenum type);
//! Converts the GL debug severity into a string.
std::string GLDebugSeverityToStr(const GLenum severity);

/**
Adjusts a string which has been returned from a GL function (e.g. "glGetProgramInfoLog").
The last wasted null termination character will be removed.
*/
void GLAdjustString(std::string& str);

//! Enables or disables the specified state
void GLSetState(GLenum cap, bool enable);

//! Converts the specified boolean into a GL-boolean value.
inline GLboolean GLBool(bool boolean)
{
    return boolean ? GL_TRUE : GL_FALSE;
}

#ifdef FORK_DEBUG

/**
Shows an error message when a GL error occured.
\param[in] operationDesc Specifies the current OpenGL
operation description (e.g. "glTexImage" or "GLRenderSystem::WriteTexture").
*/
void GLShowError(const std::string& operationDesc);

//! Flushes the last GL error.
void GLFlushError();

/**
Ensures that the shared render context is active.
Otherwise a debug message will be printed into the log output.
\param[in] operationDesc Specifies the current OpenGL
operation description (e.g. "glGenBuffers" or "glGenTextures").
*/
void GLEnsureSharedContext(const std::string& operationDesc);
/**
Appends the buffer ID to the operation description string.
\see GLEnsureSharedContext(const std::string&
*/
void GLEnsureSharedContext(const std::string& operationDesc, GLuint bufferID);

#define SHOW_GL_ERROR(desc)                             GLShowError(desc)
#define FLUSH_GL_ERROR                                  GLFlushError()
#define ENSURE_GL_SHARED_CONTEXT(desc)                  GLEnsureSharedContext(desc)
#define ENSURE_GL_SHARED_CONTEXT_FOR_BUFFER(desc, id)   GLEnsureSharedContext(desc, id)

#else

#define SHOW_GL_ERROR(desc)
#define FLUSH_GL_ERROR
#define ENSURE_GL_SHARED_CONTEXT(desc)
#define ENSURE_GL_SHARED_CONTEXT_FOR_BUFFER(desc, id)

#endif


} // /namespace Video

} // /namespace Fork


#endif



// ========================