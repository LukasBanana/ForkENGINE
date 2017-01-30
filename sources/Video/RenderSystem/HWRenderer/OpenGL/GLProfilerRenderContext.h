/*
 * OpenGL profiler render context header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GL_PROFILER_RENDER_CONTEXT_H__
#define __FORK_GL_PROFILER_RENDER_CONTEXT_H__


#include "GLRenderContext.h"
#include "Video/RenderSystem/RendererProfilerModel.h"


namespace Fork
{

namespace Video
{


//! OpenGL profiler render system.
class GLProfilerRenderContext : public GLRenderContext
{
    
    public:
        
        GLProfilerRenderContext(
            RendererProfilerModel& profilerModel,
            GLRenderContext* sharedRenderContext,
            const VideoMode& videoMode, const ContextDescription& contextDesc,
            const std::wstring& frameTitle, void* parentFrame = nullptr
        );
        
        /* --- Common --- */

        bool Activate();
        bool Deactivate();

        /* --- Render states --- */

        void ChangeState(const RasterizerState*     state);
        void ChangeState(const DepthStencilState*   state);
        void ChangeState(const BlendState*          state);

        void SetupDrawMode(const GeometryPrimitives type);

        /* --- Hardware buffers --- */

        void BindVertexBuffer(VertexBuffer* vertexBuffer);
        void UnbindVertexBuffer();

        void BindIndexBuffer(IndexBuffer* indexBuffer);
        void UnbindIndexBuffer();

        /* --- Drawing --- */

        void Draw(unsigned int numVertices, unsigned int firstVertex = 0);

        void DrawIndexed(unsigned int numVertices);
        void DrawIndexed(unsigned int numVertices, int indexOffset);

        void DrawInstanced(unsigned int numVertices, unsigned int firstVertex, unsigned int numInstances);
        void DrawInstanced(unsigned int numVertices, unsigned int firstVertex, unsigned int numInstances, unsigned int instanceOffset);

        void DrawInstancedIndexed(unsigned int numVertices, unsigned int numInstances);
        void DrawInstancedIndexed(unsigned int numVertices, unsigned int numInstances, int indexOffset);
        void DrawInstancedIndexed(unsigned int numVertices, unsigned int numInstances, int indexOffset, unsigned int instanceOffset);

        /* --- Textures --- */

        void BindTexture(const Texture* texture, unsigned int layer = 0);
        void UnbindTexture(const Texture* texture, unsigned int layer = 0);

        /* --- Render targets --- */

        void BindRenderTarget(RenderTarget* renderTarget);

        /* --- Shaders --- */

        void BindShader(const ShaderComposition* shaderComposition);
        void UnbindShader();

    private:

        RendererProfilerModel* profilerModel_ = nullptr;

        GeometryPrimitives drawingPrimitive_ = GeometryPrimitives::Triangles;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================