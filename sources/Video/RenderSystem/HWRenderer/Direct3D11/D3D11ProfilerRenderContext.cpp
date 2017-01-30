/*
 * Direct3D 11 profiler render context file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11ProfilerRenderContext.h"


namespace Fork
{

namespace Video
{


D3D11ProfilerRenderContext::D3D11ProfilerRenderContext(
    RendererProfilerModel& profilerModel,
    D3D11RenderSystem* renderSystem, ID3D11DeviceContext* context,
    const VideoMode& videoMode, const ContextDescription& contextDesc,
    const std::wstring& frameTitle, void* parentFrame) :
        D3D11RenderContext
        {
            renderSystem, context,
            videoMode, contextDesc,
            frameTitle, parentFrame
        },
        profilerModel_ { &profilerModel }
{
}

/* --- Common --- */

bool D3D11ProfilerRenderContext::Activate()
{
    if (!IsActivated())
    {
        profilerModel_->RecordContextSwitch();
        return D3D11RenderContext::Activate();
    }
    return true;
}

bool D3D11ProfilerRenderContext::Deactivate()
{
    if (IsActivated())
    {
        profilerModel_->RecordContextSwitch();
        return D3D11RenderContext::Deactivate();
    }
    return true;
}

/* --- Render states --- */

void D3D11ProfilerRenderContext::ChangeState(const RasterizerState* state)
{
    profilerModel_->RecordStateChange();
    D3D11RenderContext::ChangeState(state);
}

void D3D11ProfilerRenderContext::ChangeState(const DepthStencilState* state)
{
    profilerModel_->RecordStateChange();
    D3D11RenderContext::ChangeState(state);
}

void D3D11ProfilerRenderContext::ChangeState(const BlendState* state)
{
    profilerModel_->RecordStateChange();
    D3D11RenderContext::ChangeState(state);
}

void D3D11ProfilerRenderContext::SetupDrawMode(const GeometryPrimitives type)
{
    drawingPrimitive_ = type;
    D3D11RenderContext::SetupDrawMode(type);
}

/* --- Hardware buffers --- */

void D3D11ProfilerRenderContext::BindVertexBuffer(VertexBuffer* vertexBuffer)
{
    profilerModel_->RecordBufferBinding();
    D3D11RenderContext::BindVertexBuffer(vertexBuffer);
}

void D3D11ProfilerRenderContext::UnbindVertexBuffer()
{
    profilerModel_->RecordBufferBinding();
    D3D11RenderContext::UnbindVertexBuffer();
}

void D3D11ProfilerRenderContext::BindIndexBuffer(IndexBuffer* indexBuffer)
{
    profilerModel_->RecordBufferBinding();
    D3D11RenderContext::BindIndexBuffer(indexBuffer);
}

void D3D11ProfilerRenderContext::UnbindIndexBuffer()
{
    profilerModel_->RecordBufferBinding();
    D3D11RenderContext::UnbindIndexBuffer();
}

/* --- Drawing --- */

void D3D11ProfilerRenderContext::Draw(unsigned int numVertices, unsigned int firstVertex)
{
    profilerModel_->RecordDrawCall(RendererProfilerModel::NumTriangles(drawingPrimitive_, numVertices));
    D3D11RenderContext::Draw(numVertices, firstVertex);
}

void D3D11ProfilerRenderContext::DrawIndexed(unsigned int numVertices)
{
    profilerModel_->RecordDrawCall(RendererProfilerModel::NumTriangles(drawingPrimitive_, numVertices));
    D3D11RenderContext::DrawIndexed(numVertices);
}

void D3D11ProfilerRenderContext::DrawIndexed(unsigned int numVertices, int indexOffset)
{
    profilerModel_->RecordDrawCall(RendererProfilerModel::NumTriangles(drawingPrimitive_, numVertices));
    D3D11RenderContext::DrawIndexed(numVertices, indexOffset);
}

void D3D11ProfilerRenderContext::DrawInstanced(unsigned int numVertices, unsigned int firstVertex, unsigned int numInstances)
{
    profilerModel_->RecordDrawCall(RendererProfilerModel::NumTriangles(drawingPrimitive_, numVertices * numInstances));
    D3D11RenderContext::DrawInstanced(numVertices, firstVertex, numInstances);
}

void D3D11ProfilerRenderContext::DrawInstanced(unsigned int numVertices, unsigned int firstVertex, unsigned int numInstances, unsigned int instanceOffset)
{
    profilerModel_->RecordDrawCall(RendererProfilerModel::NumTriangles(drawingPrimitive_, numVertices * numInstances));
    D3D11RenderContext::DrawInstanced(numVertices, firstVertex, numInstances, instanceOffset);
}

void D3D11ProfilerRenderContext::DrawInstancedIndexed(unsigned int numVertices, unsigned int numInstances)
{
    profilerModel_->RecordDrawCall(RendererProfilerModel::NumTriangles(drawingPrimitive_, numVertices * numInstances));
    D3D11RenderContext::DrawInstancedIndexed(numVertices, numInstances);
}

void D3D11ProfilerRenderContext::DrawInstancedIndexed(unsigned int numVertices, unsigned int numInstances, int indexOffset)
{
    profilerModel_->RecordDrawCall(RendererProfilerModel::NumTriangles(drawingPrimitive_, numVertices * numInstances));
    D3D11RenderContext::DrawInstancedIndexed(numVertices, numInstances, indexOffset);
}

void D3D11ProfilerRenderContext::DrawInstancedIndexed(unsigned int numVertices, unsigned int numInstances, int indexOffset, unsigned int instanceOffset)
{
    profilerModel_->RecordDrawCall(RendererProfilerModel::NumTriangles(drawingPrimitive_, numVertices * numInstances));
    D3D11RenderContext::DrawInstancedIndexed(numVertices, numInstances, indexOffset, instanceOffset);
}

/* --- Textures --- */

void D3D11ProfilerRenderContext::BindTexture(const Texture* texture, unsigned int layer)
{
    profilerModel_->RecordTextureBinding();
    D3D11RenderContext::BindTexture(texture, layer);
}

void D3D11ProfilerRenderContext::UnbindTexture(const Texture* texture, unsigned int layer)
{
    profilerModel_->RecordTextureBinding();
    D3D11RenderContext::UnbindTexture(texture, layer);
}

/* --- Render targets --- */

void D3D11ProfilerRenderContext::BindRenderTarget(RenderTarget* renderTarget)
{
    profilerModel_->RecordRenderTargetBinding();
    D3D11RenderContext::BindRenderTarget(renderTarget);
}

/* --- Shaders --- */

void D3D11ProfilerRenderContext::BindShader(const ShaderComposition* shaderComposition)
{
    if (globalRenderState_.shaderComposition != shaderComposition)
    {
        profilerModel_->RecordShaderBinding();
        D3D11RenderContext::BindShader(shaderComposition);
    }
}

void D3D11ProfilerRenderContext::UnbindShader()
{
    if (globalRenderState_.shaderComposition)
    {
        profilerModel_->RecordShaderBinding();
        D3D11RenderContext::UnbindShader();
    }
}


} // /namespace Video

} // /namespace Fork



// ========================