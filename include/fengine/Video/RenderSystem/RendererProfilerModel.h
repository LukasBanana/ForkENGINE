/*
 * Renderer profiler model header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_RENDERER_PROFILER_MODEL_H__
#define __FORK_RENDERER_PROFILER_MODEL_H__


#include "Core/Export.h"
#include "Video/RenderSystem/RenderState/GeometryPrimitives.h"


namespace Fork
{

namespace Video
{


/**
Renderer profiler model class. These information can be drawn easily with the "RendererProfilerOutput" utility.
\see Utility::RendererProfilerOutput::DrawInfo
*/
class FORK_EXPORT RendererProfilerModel
{
    
    public:
        
        //! Profiling counter type. This is an unsigned 32-bit integer.
        typedef unsigned int CounterType;

        //! Restes all counters.
        void Reset();

        /**
        Returns the number of triangles for the specified primitive type and number of vertices.
        \param[in] primitive Specifies the geometry primitive.
        \param[in] numVertices Specifies the number of vertices to the rendered.
        This must be multiplied by the number of instances (when rendering with hardware instancing).
        \return The number of triangles the specified configuration will produce.
        This is only non-zero if the 'primitive' parameter is one of the following values:
        - Video::GeometryPrimitives::Triangles
        - Video::GeometryPrimitives::TriangleStrip
        - Video::GeometryPrimitives::TriangleFan
        \remarks This can be used to determine the number of triangle to pass to the "RecordDrawCall" function.
        \see RecordDrawCall
        */
        static CounterType NumTriangles(const GeometryPrimitives primitive, const CounterType& numVertices);

        /* --- Increment counter values --- */

        //! Increments the draw call counter.
        inline void RecordDrawCall(const CounterType& numRenderedTriangles)
        {
            ++drawCallCounter_;
            renderedTriangleCounter_ += numRenderedTriangles;
        }
        
        //! Increments the render target binding counter.
        inline void RecordRenderTargetBinding()
        {
            ++renderTargetBindingCounter_;
        }
        //! Increments the texture binding counter.
        inline void RecordTextureBinding()
        {
            ++textureBindingCounter_;
        }
        //! Increments the buffer binding counter.
        inline void RecordBufferBinding()
        {
            ++bufferBindingCounter_;
        }
        //! Increments the shader binding counter.
        inline void RecordShaderBinding()
        {
            ++shaderBindingCounter_;
        }

        //! Increments the texture creation counter.
        inline void RecordTextureCreation()
        {
            ++textureCreationCounter_;
        }
        //! Increments the texture update counter.
        inline void RecordTextureUpdate()
        {
            ++textureUpdateCounter_;
        }

        //! Increments the buffer creation counter.
        inline void RecordBufferCreation()
        {
            ++bufferCreationCounter_;
        }
        //! Increments the buffer update counter.
        inline void RecordBufferUpdate()
        {
            ++bufferUpdateCounter_;
        }

        //! Increments the state change counter.
        inline void RecordStateChange()
        {
            ++stateChangeCounter_;
        }
        //! Increments the context switch counter.
        inline void RecordContextSwitch()
        {
            ++contextSwitchCounter_;
        }

        /* --- Return counter values --- */

        //! Returns the number of recorded draw calls.
        inline CounterType NumDrawCalls() const
        {
            return drawCallCounter_;
        }
        //! Returns the number of rendered triangles.
        inline CounterType NumRenderedTriangles() const
        {
            return renderedTriangleCounter_;
        }

        //! Returns the number of recorded render target bindinds.
        inline CounterType NumRenderTargetBindings() const
        {
            return renderTargetBindingCounter_;
        }
        //! Returns the number of recorded buffer bindinds.
        inline CounterType NumBufferBindings() const
        {
            return bufferBindingCounter_;
        }
        //! Returns the number of recorded texture bindings.
        inline CounterType NumTextureBindings() const
        {
            return textureBindingCounter_;
        }
        //! Returns the number of recorded shader bindings.
        inline CounterType NumShaderBindings() const
        {
            return shaderBindingCounter_;
        }

        //! Returns the number of recorded texture creations.
        inline CounterType NumTextureCreations() const
        {
            return textureCreationCounter_;
        }
        //! Returns the number of recorded texture updates.
        inline CounterType NumTextureUpdates() const
        {
            return textureUpdateCounter_;
        }

        //! Returns the number of recorded buffer creations.
        inline CounterType NumBufferCreations() const
        {
            return bufferCreationCounter_;
        }
        //! Returns the number of recorded buffer updates.
        inline CounterType NumBufferUpdates() const
        {
            return bufferUpdateCounter_;
        }

        //! Returns the number of state changed (including rasterizer-, blend- and depth-stencil states).
        inline CounterType NumStateChanges() const
        {
            return stateChangeCounter_;
        }
        //! Returns the number of render context switches.
        inline CounterType NumContextSwitches() const
        {
            return contextSwitchCounter_;
        }

    private:
        
        CounterType drawCallCounter_            = 0;
        CounterType renderedTriangleCounter_    = 0;

        CounterType renderTargetBindingCounter_ = 0;
        CounterType textureBindingCounter_      = 0;
        CounterType bufferBindingCounter_       = 0;
        CounterType shaderBindingCounter_       = 0;
        
        CounterType textureCreationCounter_     = 0;
        CounterType textureUpdateCounter_       = 0;

        CounterType bufferCreationCounter_      = 0;
        CounterType bufferUpdateCounter_        = 0;

        CounterType stateChangeCounter_         = 0;
        CounterType contextSwitchCounter_       = 0;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================