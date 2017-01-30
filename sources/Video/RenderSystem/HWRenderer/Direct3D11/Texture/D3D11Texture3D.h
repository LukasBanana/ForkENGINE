/*
 * Direct3D 11 texture 3D header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_TEXTURE_3D_H__
#define __FORK_D3D11_TEXTURE_3D_H__


#include "Video/RenderSystem/Texture/Texture3D.h"
#include "D3D11TextureBase.h"

#include <d3d11.h>


namespace Fork
{

namespace Video
{


class D3D11Texture3D : public Texture3D, public D3D11TextureBase
{
    
    public:
        
        D3D11Texture3D();
        ~D3D11Texture3D();

        D3D11Texture3D(const D3D11Texture3D&) = delete;
        D3D11Texture3D& operator = (const D3D11Texture3D&) = delete;
        
        ID3D11Resource* AbstractResource();

        //! Creates a new texture resource and releases the previous one.
        bool CreateResource(
            ID3D11Device* device,
            const D3D11_TEXTURE3D_DESC& desc,
            const D3D11_SUBRESOURCE_DATA* initialData = nullptr
        );

        inline ID3D11Texture3D* GetResource() const
        {
            return resource_;
        }

    private:
        
        ID3D11Texture3D* resource_ = nullptr;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================