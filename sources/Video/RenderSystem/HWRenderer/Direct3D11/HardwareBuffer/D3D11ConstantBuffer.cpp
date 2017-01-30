/*
 * Direct3D 11 constant buffer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "D3D11ConstantBuffer.h"
#include "../D3D11ParamMapper.h"
#include "../D3D11Core.h"
#include "Video/RenderSystem/HardwareBuffer/ConstantBufferException.h"


namespace Fork
{

namespace Video
{


/*
<------------------------------- !TODO! ------------------------------->
DYNAMIC constant buffer for PARTIAL sub resource UPDATE does not work!!!
*/

D3D11ConstantBuffer::D3D11ConstantBuffer(ID3D11Device* device, const Description& desc) :
    ConstantBuffer{ desc }
{
}

HRESULT D3D11ConstantBuffer::WriteConstants(
    ID3D11Device* device, const void* data, UINT size, D3D11_USAGE usage)
{
    return Write(
        device, data, size, D3D11_BIND_CONSTANT_BUFFER, 0,
        D3D11ParamMapper::GetCPUAccessFlagsByUsage(usage),
        0, usage
    );
}

/*
Overwrite this function for "D3D11ConstantBuffer" class,
because "ID3D11DeviceContext::UpdateSubresource" cannot be used
to partially update a shader constant buffer!
*/
void D3D11ConstantBuffer::WriteSub(ID3D11DeviceContext* context, const void* data, UINT offset, UINT size)
{
    if (GetBuffer())
    {
        if (offset == 0 && size == GetDesc().size)
        {
            /* Update entire sub resource */
            context->UpdateSubresource(GetBuffer(), 0, nullptr, data, 0, 0);
        }
        else
        {
            /* Map sub resource for partial buffer update */
            D3D11_MAPPED_SUBRESOURCE mappedSubResource;
            auto result = context->Map(GetBuffer(), 0, D3D11_MAP_WRITE, 0, &mappedSubResource);

            if (!DXAssert(result, "Mapping D3D11 sub resource failed", false) && mappedSubResource.pData)
            {
                /* Copy new data into mapped sub resource */
                auto byteAlignedData = reinterpret_cast<const char*>(data);
                std::copy(
                    byteAlignedData,
                    byteAlignedData + size,
                    reinterpret_cast<char*>(mappedSubResource.pData)
                );

                /* Unmap sub resource for further use on the GPU */
                context->Unmap(GetBuffer(), 0);
            }
        }
    }
}


} // /namespace Video

} // /namespace Fork



// ========================