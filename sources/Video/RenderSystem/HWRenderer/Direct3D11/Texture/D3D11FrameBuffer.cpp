/*
 * Direct3D 11 frame buffer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11FrameBuffer.h"


namespace Fork
{

namespace Video
{


D3D11FrameBuffer::D3D11FrameBuffer(
    const std::vector<ID3D11RenderTargetView*>& renderTargetViews,
    ID3D11DepthStencilView* depthStencilView) :
        renderTargetViews   ( renderTargetViews ),
        depthStencilView    ( depthStencilView  )
{
}


} // /namespace Video

} // /namespace Fork



// ========================