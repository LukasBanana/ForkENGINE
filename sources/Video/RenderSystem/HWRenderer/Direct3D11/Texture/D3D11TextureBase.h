/*
 * Direct3D 11 texture base header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_TEXTURE_BASE_H__
#define __FORK_D3D11_TEXTURE_BASE_H__


#include <d3d11.h>


namespace Fork
{

namespace Video
{


class D3D11TextureBase
{
    
    public:
        
        virtual ~D3D11TextureBase();

        D3D11TextureBase(const D3D11TextureBase&) = delete;
        D3D11TextureBase& operator = (const D3D11TextureBase&) = delete;

        //! Returns the abstract resource (parent class of each ID3D11Texture... class).
        virtual ID3D11Resource* AbstractResource() = 0;
        
        //! Returns the shader resource view.
        inline ID3D11ShaderResourceView* GetResourceView() const
        {
            return resourceView_;
        }

    protected:
        
        D3D11TextureBase() = default;

        //! Creates the shader resource view.
        bool CreateResourceView(ID3D11Device* device, const D3D11_SHADER_RESOURCE_VIEW_DESC* desc = nullptr);

    private:
        
        ID3D11ShaderResourceView* resourceView_ = nullptr;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================