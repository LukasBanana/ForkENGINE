/*
 * Direct3D 11 vertex buffer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11VertexBuffer.h"
#include "../D3D11ParamMapper.h"


namespace Fork
{

namespace Video
{


void D3D11VertexBuffer::SetupFormat(const VertexFormat& format)
{
    this->format = format;
}

HRESULT D3D11VertexBuffer::WriteVertices(
    ID3D11Device* device, const void* data, UINT size, D3D11_USAGE usage)
{
    return Write(
        device, data, size, D3D11_BIND_VERTEX_BUFFER, GetFormat().GetFormatSize(),
        D3D11ParamMapper::GetCPUAccessFlagsByUsage(usage),
        0, usage
    );
}


} // /namespace Video

} // /namespace Fork



// ========================