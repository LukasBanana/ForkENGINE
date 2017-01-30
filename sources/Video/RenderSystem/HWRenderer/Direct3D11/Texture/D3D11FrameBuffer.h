/*
 * Direct3D 11 frame buffer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_FRAME_BUFFER_H__
#define __FORK_D3D11_FRAME_BUFFER_H__


#include <vector>
#include <d3d11.h>


namespace Fork
{

namespace Video
{


//! Direct3D11 framebuffer structure.
struct D3D11FrameBuffer
{
    D3D11FrameBuffer() = default;
    D3D11FrameBuffer(
        const std::vector<ID3D11RenderTargetView*>& renderTargetViews,
        ID3D11DepthStencilView* depthStencilView
    );

    std::vector<ID3D11RenderTargetView*>    renderTargetViews;
    ID3D11DepthStencilView*                 depthStencilView    = nullptr;
};


} // /namespace Video

} // /namespace Fork


#endif



// ========================