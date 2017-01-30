/*
 * OpenGL profiler render context file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLProfilerRenderContext.h"


namespace Fork
{

namespace Video
{


GLProfilerRenderContext::GLProfilerRenderContext(
    RendererProfilerModel& profilerModel,
    GLRenderContext* sharedRenderContext,
    const VideoMode& videoMode, const ContextDescription& contextDesc,
    const std::wstring& frameTitle, void* parentFrame) :
        GLRenderContext
        {
            sharedRenderContext,
            videoMode, contextDesc,
            frameTitle, parentFrame
        },
        profilerModel_ { &profilerModel }
{
}

/* --- Common --- */

bool GLProfilerRenderContext::Activate()
{
    if (!IsActivated())
    {
        profilerModel_->RecordContextSwitch();
        return GLRenderContext::Activate();
    }
    return true;
}

bool GLProfilerRenderContext::Deactivate()
{
    if (IsActivated())
    {
        profilerModel_->RecordContextSwitch();
        return GLRenderContext::Deactivate();
    }
    return true;
}

/* --- Render states --- */

void GLProfilerRenderContext::ChangeState(const RasterizerState* state)
{
    profilerModel_->RecordStateChange();
    GLRenderContext::ChangeState(state);
}

void GLProfilerRenderContext::ChangeState(const DepthStencilState* state)
{
    profilerModel_->RecordStateChange();
    GLRenderContext::ChangeState(state);
}

void GLProfilerRenderContext::ChangeState(const BlendState* state)
{
    profilerModel_->RecordStateChange();
    GLRenderContext::ChangeState(state);
}

void GLProfilerRenderContext::SetupDrawMode(const GeometryPrimitives type)
{
    drawingPrimitive_ = type;
    GLRenderContext::SetupDrawMode(type);
}

/* --- Hardware buffers --- */

void GLProfilerRenderContext::BindVertexBuffer(VertexBuffer* vertexBuffer)
{
    profilerModel_->RecordBufferBinding();
    GLRenderContext::BindVertexBuffer(vertexBuffer);
}

void GLProfilerRenderContext::UnbindVertexBuffer()
{
    profilerModel_->RecordBufferBinding();
    GLRenderContext::UnbindVertexBuffer();
}

void GLProfilerRenderContext::BindIndexBuffer(IndexBuffer* indexBuffer)
{
    profilerModel_->RecordBufferBinding();
    GLRenderContext::BindIndexBuffer(indexBuffer);
}

void GLProfilerRenderContext::UnbindIndexBuffer()
{
    profilerModel_->RecordBufferBinding();
    GLRenderContext::UnbindIndexBuffer();
}

/* --- Drawing --- */

void GLProfilerRenderContext::Draw(unsigned int numVertices, unsigned int firstVertex)
{
    profilerModel_->RecordDrawCall(RendererProfilerModel::NumTriangles(drawingPrimitive_, numVertices));
    GLRenderContext::Draw(numVertices, firstVertex);
}

void GLProfilerRenderContext::DrawIndexed(unsigned int numVertices)
{
    profilerModel_->RecordDrawCall(RendererProfilerModel::NumTriangles(drawingPrimitive_, numVertices));
    GLRenderContext::DrawIndexed(numVertices);
}

void GLProfilerRenderContext::DrawIndexed(unsigned int numVertices, int indexOffset)
{
    profilerModel_->RecordDrawCall(RendererProfilerModel::NumTriangles(drawingPrimitive_, numVertices));
    GLRenderContext::DrawIndexed(numVertices, indexOffset);
}

void GLProfilerRenderContext::DrawInstanced(unsigned int numVertices, unsigned int firstVertex, unsigned int numInstances)
{
    profilerModel_->RecordDrawCall(RendererProfilerModel::NumTriangles(drawingPrimitive_, numVertices * numInstances));
    GLRenderContext::DrawInstanced(numVertices, firstVertex, numInstances);
}

void GLProfilerRenderContext::DrawInstanced(unsigned int numVertices, unsigned int firstVertex, unsigned int numInstances, unsigned int instanceOffset)
{
    profilerModel_->RecordDrawCall(RendererProfilerModel::NumTriangles(drawingPrimitive_, numVertices * numInstances));
    GLRenderContext::DrawInstanced(numVertices, firstVertex, numInstances, instanceOffset);
}

void GLProfilerRenderContext::DrawInstancedIndexed(unsigned int numVertices, unsigned int numInstances)
{
    profilerModel_->RecordDrawCall(RendererProfilerModel::NumTriangles(drawingPrimitive_, numVertices * numInstances));
    GLRenderContext::DrawInstancedIndexed(numVertices, numInstances);
}

void GLProfilerRenderContext::DrawInstancedIndexed(unsigned int numVertices, unsigned int numInstances, int indexOffset)
{
    profilerModel_->RecordDrawCall(RendererProfilerModel::NumTriangles(drawingPrimitive_, numVertices * numInstances));
    GLRenderContext::DrawInstancedIndexed(numVertices, numInstances, indexOffset);
}

void GLProfilerRenderContext::DrawInstancedIndexed(unsigned int numVertices, unsigned int numInstances, int indexOffset, unsigned int instanceOffset)
{
    profilerModel_->RecordDrawCall(RendererProfilerModel::NumTriangles(drawingPrimitive_, numVertices * numInstances));
    GLRenderContext::DrawInstancedIndexed(numVertices, numInstances, indexOffset, instanceOffset);
}

/* --- Textures --- */

void GLProfilerRenderContext::BindTexture(const Texture* texture, unsigned int layer)
{
    profilerModel_->RecordTextureBinding();
    GLRenderContext::BindTexture(texture, layer);
}

void GLProfilerRenderContext::UnbindTexture(const Texture* texture, unsigned int layer)
{
    profilerModel_->RecordTextureBinding();
    GLRenderContext::UnbindTexture(texture, layer);
}

/* --- Render targets --- */

void GLProfilerRenderContext::BindRenderTarget(RenderTarget* renderTarget)
{
    profilerModel_->RecordRenderTargetBinding();
    GLRenderContext::BindRenderTarget(renderTarget);
}

/* --- Shaders --- */

void GLProfilerRenderContext::BindShader(const ShaderComposition* shaderComposition)
{
    if (globalRenderState_.shaderComposition != shaderComposition)
    {
        profilerModel_->RecordShaderBinding();
        GLRenderContext::BindShader(shaderComposition);
    }
}

void GLProfilerRenderContext::UnbindShader()
{
    if (globalRenderState_.shaderComposition)
    {
        profilerModel_->RecordShaderBinding();
        GLRenderContext::UnbindShader();
    }
}


} // /namespace Video

} // /namespace Fork



// ========================