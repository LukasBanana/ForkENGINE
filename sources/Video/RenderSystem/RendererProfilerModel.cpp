/*
 * Renderer profiler model header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/RenderSystem/RendererProfilerModel.h"


namespace Fork
{

namespace Video
{


void RendererProfilerModel::Reset()
{
    drawCallCounter_            = 0;
    renderedTriangleCounter_    = 0;

    renderTargetBindingCounter_ = 0;
    textureBindingCounter_      = 0;
    bufferBindingCounter_       = 0;
    shaderBindingCounter_       = 0;

    textureCreationCounter_     = 0;
    textureUpdateCounter_       = 0;

    bufferCreationCounter_      = 0;
    bufferUpdateCounter_        = 0;

    stateChangeCounter_         = 0;
    contextSwitchCounter_       = 0;
}

RendererProfilerModel::CounterType RendererProfilerModel::NumTriangles(
    const GeometryPrimitives primitive, const CounterType& numVertices)
{
    switch (primitive)
    {
        case GeometryPrimitives::Triangles:
            return numVertices / 3;
        case GeometryPrimitives::TriangleStrip:
        case GeometryPrimitives::TriangleFan:
            return (numVertices > 2 ? numVertices - 2 : 0);
        default:
            break;
    }
    return 0;
}


} // /namespace Video

} // /namespace Fork



// ========================