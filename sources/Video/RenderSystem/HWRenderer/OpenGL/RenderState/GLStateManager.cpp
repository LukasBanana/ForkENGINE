/*
 * OpenGL state manager file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLStateManager.h"
#include "Core/STLHelper.h"
#include "../HardwareBuffer/GLVertexBuffer.h"
#include "IO/Core/Log.h"

#include <limits>


namespace Fork
{

namespace Video
{


/*
 * Internal members
 */

static const int invalidationValueInt32 = std::numeric_limits<int>::min();


/*
 * GLStateManager class
 */

GLStateManager::GLStateManager()
{
    std::fill(states_.begin(), states_.end(), false);
    std::fill(buffers_.begin(), buffers_.end(), 0u);
}

void GLStateManager::InitStates()
{
    /* Initialize all states with the current GL context */
    #define INIT_STATE(s) InitState(GL_##s, s)

    INIT_STATE( BLEND                         );
    INIT_STATE( COLOR_LOGIC_OP                );
    INIT_STATE( CULL_FACE                     );
    INIT_STATE( DEBUG_OUTPUT                  );
    INIT_STATE( DEBUG_OUTPUT_SYNCHRONOUS      );
    INIT_STATE( DEPTH_CLAMP                   );
    INIT_STATE( DEPTH_TEST                    );
    INIT_STATE( DITHER                        );
    INIT_STATE( FRAMEBUFFER_SRGB              );
    INIT_STATE( LINE_SMOOTH                   );
    INIT_STATE( MULTISAMPLE                   );
    INIT_STATE( POLYGON_OFFSET_FILL           );
    INIT_STATE( POLYGON_OFFSET_LINE           );
    INIT_STATE( POLYGON_OFFSET_POINT          );
    INIT_STATE( POLYGON_SMOOTH                );
    INIT_STATE( PRIMITIVE_RESTART             );
    INIT_STATE( PRIMITIVE_RESTART_FIXED_INDEX );
    INIT_STATE( RASTERIZER_DISCARD            );
    INIT_STATE( SAMPLE_ALPHA_TO_COVERAGE      );
    INIT_STATE( SAMPLE_ALPHA_TO_ONE           );
    INIT_STATE( SAMPLE_COVERAGE               );
    INIT_STATE( SAMPLE_SHADING                );
    INIT_STATE( SAMPLE_MASK                   );
    INIT_STATE( SCISSOR_TEST                  );
    INIT_STATE( STENCIL_TEST                  );
    INIT_STATE( TEXTURE_CUBE_MAP_SEAMLESS     );
    INIT_STATE( PROGRAM_POINT_SIZE            );

    #undef INIT_STATE
}

void GLStateManager::Enable(const GLenum cap, const GLStates index)
{
    if (!states_[index])
    {
        states_[index] = true;
        glEnable(cap);
    }
}

void GLStateManager::Disable(const GLenum cap, const GLStates index)
{
    if (states_[index])
    {
        states_[index] = false;
        glDisable(cap);
    }
}

void GLStateManager::Change(const GLenum cap, const GLStates index, bool value)
{
    if (states_[index] != value)
    {
        states_[index] = value;
        if (value)
            glEnable(cap);
        else
            glDisable(cap);
    }
}

void GLStateManager::Push(const GLenum cap, const GLStates index, bool enable)
{
    /* Store previous state onto stack */
    GLboolean state;
    glGetBooleanv(cap, &state);
    stateStack_.push({ cap, index, (state != GL_FALSE) });

    /* Set new state */
    Change(cap, index, enable);
}

void GLStateManager::Pop()
{
    /* Reset previous state */
    const auto& prevState = stateStack_.top();
    Change(prevState.cap, prevState.index, (prevState.enabled != GL_FALSE));

    /* Pop state from stack */
    stateStack_.pop();
}

void GLStateManager::Pop(size_t count)
{
    while (count-- > 0)
        Pop();
}

void GLStateManager::SetupViewport(int x, int y, int width, int height)
{
    /* Check if the GL state must be changed */
    if (viewport_.position.x != x || viewport_.position.y != y || viewport_.size.width != width || viewport_.size.height != height)
    {
        /* Store new state */
        viewport_.position.x    = x;
        viewport_.position.y    = y;
        viewport_.size.width    = width;
        viewport_.size.height   = height;

        /* Change GL state */
        glViewport(x, y, width, height);
    }
}

void GLStateManager::InvalidateViewport()
{
    viewport_.position.x = invalidationValueInt32;
}

void GLStateManager::SetupScissor(int x, int y, int width, int height)
{
    /* Check if the GL state must be changed */
    if (scissor_.position.x != x || scissor_.position.y != y || scissor_.size.width != width || scissor_.size.height != height)
    {
        /* Store new state */
        scissor_.position.x     = x;
        scissor_.position.y     = y;
        scissor_.size.width     = width;
        scissor_.size.height    = height;

        /* Change GL state */
        glScissor(x, y, width, height);
    }
}

void GLStateManager::InvalidateScissor()
{
    scissor_.position.x = invalidationValueInt32;
}

static size_t MapBufferTarget(GLenum target)
{
    switch (target)
    {
        case GL_ARRAY_BUFFER:
            return GLStateManager::ARRAY;
        case GL_ELEMENT_ARRAY_BUFFER:
            return GLStateManager::ELEMENT_ARRAY;
        case GL_UNIFORM_BUFFER:
            return GLStateManager::UNIFORM;
        case GL_SHADER_STORAGE_BUFFER:
            return GLStateManager::SHADER_STORAGE;
    }
    return GLStateManager::__TARGET_NUM__;
}

/* --- Buffers --- */

#define __ENABLE_BINDBUFFER_OPT__
//#define __DEB_DUMP_OUTPUT__

void GLStateManager::BindBuffer(GLenum target, GLuint id)
{
    #ifdef __ENABLE_BINDBUFFER_OPT__
    auto mappedTarget = MapBufferTarget(target);
    if (mappedTarget < __TARGET_NUM__)
    {
        if (buffers_[mappedTarget] != id)
        {
            #ifdef __DEB_DUMP_OUTPUT__
            IO::Log::Debug("GLStateManager", "BindBuffer(target: " + ToStr(mappedTarget) + ", id: " + ToStr(id) + ")");
            #endif
            buffers_[mappedTarget] = id;
            glBindBuffer(target, id);
        }
        #ifdef __DEB_DUMP_OUTPUT__
        else
            IO::Log::Debug("GLStateManager", "BindBuffer(target: " + ToStr(mappedTarget) + ", id: " + ToStr(id) + ") [omitted]");
        #endif
    }
    else
    #endif
        glBindBuffer(target, id);
}

static size_t MapBufferBaseTarget(GLenum target)
{
    switch (target)
    {
        case GL_UNIFORM_BUFFER:
            return GLStateManager::UNIFORM;
        case GL_SHADER_STORAGE_BUFFER:
            return GLStateManager::SHADER_STORAGE;
    }
    return GLStateManager::__TARGET_NUM__;
}

void GLStateManager::BindBufferBase(GLenum target, GLuint index, GLuint id)
{
    auto mappedTarget = MapBufferBaseTarget(target);
    if (mappedTarget < __TARGET_NUM__)
    {
        buffers_[mappedTarget] = id;
        #ifdef __DEB_DUMP_OUTPUT__
        IO::Log::Debug("GLStateManager", "BindBufferBase(target: " + ToStr(mappedTarget) + ", index: " + ToStr(index) + ", id: " + ToStr(id) + ")");
        #endif
    }
    glBindBufferBase(target, index, id);
}

void GLStateManager::BindVertexArray(GLuint id)
{
    /* Bind VAO */
    glBindVertexArray(id);

    if (id != 0)
    {
        #ifdef __DEB_DUMP_OUTPUT__
        IO::Log::Debug("GLStateManager", "BindVertexArray(id: " + ToStr(id) + ")");
        #endif

        /* Reset VBO and IAO info */
        buffers_[ARRAY] = 0;
        buffers_[ELEMENT_ARRAY] = 0;
    }
}


/*
 * ======= Private: =======
 */

void GLStateManager::InitState(const GLenum cap, const GLStates index)
{
    states_[index] = (glIsEnabled(cap) != GL_FALSE);
}


} // /namespace Video

} // /namespace Fork



// ========================