/*
 * Direct3D 11 vertex buffer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_VERTEX_BUFFER_H__
#define __FORK_D3D11_VERTEX_BUFFER_H__


#include "Video/RenderSystem/HardwareBuffer/VertexBuffer.h"
#include "D3D11HardwareBuffer.h"


namespace Fork
{

namespace Video
{


class D3D11VertexBuffer : public VertexBuffer, public D3D11HardwareBuffer
{
    
    public:
        
        D3D11VertexBuffer() = default;

        D3D11VertexBuffer(const D3D11VertexBuffer&) = delete;
        D3D11VertexBuffer& operator = (const D3D11VertexBuffer&) = delete;

        void SetupFormat(const VertexFormat& format);

        HRESULT WriteVertices(
            ID3D11Device* device, const void* data, UINT size,
            D3D11_USAGE usage = D3D11_USAGE_DEFAULT
        );

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================