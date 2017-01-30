/*
 * Direct3D 11 harware buffer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_HARDWARE_BUFFER_H__
#define __FORK_D3D11_HARDWARE_BUFFER_H__


#include <d3d11.h>


namespace Fork
{

namespace Video
{


class D3D11HardwareBuffer
{
    
    public:
        
        D3D11HardwareBuffer();
        virtual ~D3D11HardwareBuffer();

        D3D11HardwareBuffer(const D3D11HardwareBuffer&) = delete;
        D3D11HardwareBuffer& operator = (const D3D11HardwareBuffer&) = delete;

        HRESULT Write(
            ID3D11Device* device, const void* data, UINT size, UINT bindFlags,
            UINT stride = 0, UINT cpuAccessFlags = 0, UINT miscFlags = 0, D3D11_USAGE usage = D3D11_USAGE_DEFAULT
        );

        //! \note All pointers must never be null!
        virtual void WriteSub(
            ID3D11DeviceContext* context, const void* data,
            UINT srcRowPitch, UINT srcDepthPitch, const D3D11_BOX& destBox
        );
        //! \note All pointers must never be null!
        virtual void WriteSub(ID3D11DeviceContext* context, const void* data, UINT offset, UINT size);
        //! \note All pointers must never be null!
        virtual void WriteSub(ID3D11DeviceContext* context, const void* data);

        //! Returns the OpenGL buffer ID number.
        inline ID3D11Buffer* GetBuffer() const
        {
            return buffer_;
        }

    private:
        
        ID3D11Buffer* buffer_ = nullptr;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================