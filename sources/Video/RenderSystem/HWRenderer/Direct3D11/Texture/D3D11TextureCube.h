/*
 * Direct3D 11 texture cube header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_TEXTURE_CUBE_H__
#define __FORK_D3D11_TEXTURE_CUBE_H__


#include "Video/RenderSystem/Texture/TextureCube.h"
#include "D3D11TextureBase.h"

#include <d3d11.h>


namespace Fork
{

namespace Video
{


class D3D11TextureCube : public TextureCube, public D3D11TextureBase
{
    
    public:
        
        D3D11TextureCube();
        ~D3D11TextureCube();

        D3D11TextureCube(const D3D11TextureCube&) = delete;
        D3D11TextureCube& operator = (const D3D11TextureCube&) = delete;
        
        ID3D11Resource* AbstractResource();

        //! Creates a new texture resource and releases the previous one.
        bool CreateResource(
            ID3D11Device* device,
            const D3D11_TEXTURE2D_DESC& desc,
            const D3D11_SUBRESOURCE_DATA* initialData = nullptr
        );

        inline ID3D11Texture2D* GetResource() const
        {
            return resource_;
        }

    protected:

        friend class D3D11RenderSystem;

        void SetupArray(unsigned int arraySize);
        void SetupSize(const TextureCube::SizeType& size);

    private:
        
        ID3D11Texture2D* resource_ = nullptr;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================