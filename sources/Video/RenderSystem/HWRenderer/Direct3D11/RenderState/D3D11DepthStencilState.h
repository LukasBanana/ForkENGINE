/*
 * Direct3D 11 depth stencil state header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_DEPTH_STENCIL_STATE_H__
#define __FORK_D3D11_DEPTH_STENCIL_STATE_H__


#include "Video/RenderSystem/RenderState/DepthStencilState.h"

#include <d3d11.h>


namespace Fork
{

namespace Video
{


class D3D11DepthStencilState : public DepthStencilState
{
    
    public:
        
        D3D11DepthStencilState(ID3D11Device* device, const Description& desc);
        ~D3D11DepthStencilState();

        bool ChangeDesc(ID3D11Device* device, const Description& desc);

        inline ID3D11DepthStencilState* GetStateObject() const
        {
            return stateObject_;
        }

    private:
        
        ID3D11DepthStencilState* stateObject_ = nullptr;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================