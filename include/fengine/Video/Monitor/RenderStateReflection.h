/*
 * Renderer state reflection header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_RENDER_STATE_REFLECTION_H__
#define __FORK_RENDER_STATE_REFLECTION_H__


#include "Math/Core/Matrix4.h"
#include "Video/RenderSystem/RenderState/BlendState.h"
#include "Video/RenderSystem/RenderState/DepthStencilState.h"
#include "Video/RenderSystem/RenderState/RasterizerState.h"
#include "Video/RenderSystem/RenderState/SamplerState.h"
#include "Video/Core/Viewport.h"
#include "Video/Core/Scissor.h"

#include <vector>


namespace Fork
{

namespace Video
{


/**
This structure stores all common render state descriptions.
It can be used to debug the current render pipeline.
\see RenderContext::QueryRenderStateReflection
*/
struct RenderStateReflection
{
    RasterizerState::Description            rasterizerStateDesc;    //!< Current rasterizer states.
    DepthStencilState::Description          depthStencilStateDesc;  //!< Current depth-stencil states.
    BlendState::Description                 blendStateDesc;         //!< Current blend states.

    std::vector<SamplerState::Description>  samplerStateDesc;       //!< Current sampler states.
    std::vector<unsigned int>               boundedTextureLayers;   //!< Current bounded texture layers. No references to the texture objects available here.

    std::vector<Viewport>                   viewports;              //!< Current viewport states.
    std::vector<Scissor>                    scissors;               //!< Current scissor states.
};


} // /namespace Video

} // /namespace Fork


#endif



// ========================