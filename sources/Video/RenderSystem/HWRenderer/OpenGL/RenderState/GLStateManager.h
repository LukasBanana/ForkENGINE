/*
 * OpenGL state manager header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_STATE_MANAGER_H__
#define __FORK_GL_STATE_MANAGER_H__


#include "../GLImport.h"
#include "Video/Core/Viewport.h"
#include "Video/Core/Scissor.h"

#include <stack>
#include <array>


namespace Fork
{

namespace Video
{


class GLVertexBuffer;

/**
Manages common OpenGL states to reduce redundant state changes.
One GL state manager can be shared by several GL render contexts.
This is the case, when they share a single hardware context (on Windows: HGLRC).
*/
class GLStateManager
{
    
    public:
        
        /* === Enumerations === */

        /**
        OpenGL state indices enumeration.
        \note This enumeration is an exception to the engine's coding convention.
        This is for easier compatibility between this enumeraiton and the OpenGL macros.
        */
        enum GLStates
        {
            BLEND = 0,
            COLOR_LOGIC_OP,
            CULL_FACE,
            DEBUG_OUTPUT,
            DEBUG_OUTPUT_SYNCHRONOUS,
            DEPTH_CLAMP,
            DEPTH_TEST,
            DITHER,
            FRAMEBUFFER_SRGB,
            LINE_SMOOTH,
            MULTISAMPLE,
            POLYGON_OFFSET_FILL,
            POLYGON_OFFSET_LINE,
            POLYGON_OFFSET_POINT,
            POLYGON_SMOOTH,
            PRIMITIVE_RESTART,
            PRIMITIVE_RESTART_FIXED_INDEX,
            RASTERIZER_DISCARD,
            SAMPLE_ALPHA_TO_COVERAGE,
            SAMPLE_ALPHA_TO_ONE,
            SAMPLE_COVERAGE,
            SAMPLE_SHADING,
            SAMPLE_MASK,
            SCISSOR_TEST,
            STENCIL_TEST,
            TEXTURE_CUBE_MAP_SEAMLESS,
            PROGRAM_POINT_SIZE,

            __STATE_NUM__
        };

        /**
        OpenGL buffer target enumeration.
        \note This enumeration is an exception to the engine's coding convention.
        This is for easier compatibility between this enumeraiton and the OpenGL macros.
        */
        enum BufferTargets
        {
            ARRAY = 0,
            ELEMENT_ARRAY,
            UNIFORM,
            SHADER_STORAGE,

            __TARGET_NUM__
        };

        /* === Functions === */

        //! Initialize all states in the array with 'false'.
        GLStateManager();

        //! Initializes all states with the current GL context.
        void InitStates();

        //! Enables the specified render state.
        void Enable(const GLenum cap, const GLStates index);
        //! Disables the specified render state.
        void Disable(const GLenum cap, const GLStates index);
        //! Changes the specified render state.
        void Change(const GLenum cap, const GLStates index, bool value);

        //! Enables or disables the specified state and stores the previous state onto the internal stack.
        void Push(const GLenum cap, const GLStates index, bool enable);
        /**
        Pops and resets the previous render state from the stack.
        \note No check for stack underflow!
        */
        void Pop();
        /**
        Pops and resets the previous 'count' render states from the stack.
        \note No check for stack underflow!
        */
        void Pop(size_t count);

        //! Changes the GL viewport if necessary.
        void SetupViewport(int x, int y, int width, int height);
        //! Invalidates the viewport. This will force an update when "SetupViewport" is called the next time.
        void InvalidateViewport();

        //! Changes the GL scissor rectangle if necessary.
        void SetupScissor(int x, int y, int width, int height);
        //! Invalidates the scissor rectangle. This will force an update when "SetupScissor" is called the next time.
        void InvalidateScissor();

        /* --- Buffers --- */

        //! Binds the specified hardware buffer.
        void BindBuffer(GLenum target, GLuint id);
        //! Binds the specified hardware buffer to the specified index.
        void BindBufferBase(GLenum target, GLuint index, GLuint id);
        //! Binds the specified hardware vertex array.
        void BindVertexArray(GLuint id);

        /* === Inline functions === */

        inline bool IsEnabled(const GLStates index) const
        {
            return states_[index];
        }

        /* === Templates === */

        template <GLenum cap, GLStates index> void Enable()
        {
            if (!states_[index])
            {
                states_[index] = true;
                glEnable(cap);
            }
        }

        template <GLenum cap, GLStates index> void Disable()
        {
            if (states_[index])
            {
                states_[index] = false;
                glDisable(cap);
            }
        }

        template <GLenum cap, GLStates index> void Change(bool value)
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

        template <GLStates index> inline bool IsEnabled() const
        {
            return states_[index];
        }

    private:
        
        /* === Structures === */

        struct GLRenderState
        {
            GLenum      cap;
            GLStates    index;
            bool        enabled;
        };

        /* === Functions === */

        void InitState(const GLenum cap, const GLStates index);

        /* === Members === */

        std::array<bool, __STATE_NUM__>     states_;
        std::stack<GLRenderState>           stateStack_;

        std::array<GLuint, __TARGET_NUM__>  buffers_;

        Viewport                            viewport_;
        Scissor                             scissor_;

};


#define GLSTATEMNGR_ENABLE(m, s)    (m).Enable<GL_##s, GLStateManager::s>()
#define GLSTATEMNGR_DISABLE(m, s)   (m).Disable<GL_##s, GLStateManager::s>()
#define GLSTATEMNGR_CHANGE(m, s, v) (m).Change<GL_##s, GLStateManager::s>(v)

#define GLSTATEMNGR_PUSH(m, s, v)   (m).Push(GL_##s, GLStateManager::s, v)


} // /namespace Video

} // /namespace Fork


#endif



// ========================