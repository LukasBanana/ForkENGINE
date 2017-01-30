/*
 * Direct3D 11 texture 1D header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_TEXTURE_1D_H__
#define __FORK_D3D11_TEXTURE_1D_H__


#include "Video/RenderSystem/Texture/Texture1D.h"
#include "D3D11TextureBase.h"

#include <d3d11.h>


namespace Fork
{

namespace Video
{


class D3D11Texture1D : public Texture1D, public D3D11TextureBase
{
    
    public:
        
        D3D11Texture1D();
        ~D3D11Texture1D();

        D3D11Texture1D(const D3D11Texture1D&) = delete;
        D3D11Texture1D& operator = (const D3D11Texture1D&) = delete;
        
        ID3D11Resource* AbstractResource();

        //! Creates a new texture resource and releases the previous one.
        bool CreateResource(
            ID3D11Device* device,
            const D3D11_TEXTURE1D_DESC& desc,
            const D3D11_SUBRESOURCE_DATA* initialData = nullptr
        );

        inline ID3D11Texture1D* GetResource() const
        {
            return resource_;
        }

    protected:
        
        friend class D3D11RenderSystem;

        void SetupArray(unsigned int arraySize);

    private:
        
        ID3D11Texture1D* resource_ = nullptr;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================