/*
 * Direct3D 11 rasterizer state header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_RASTERIZER_STATE_H__
#define __FORK_D3D11_RASTERIZER_STATE_H__


#include "Video/RenderSystem/RenderState/RasterizerState.h"

#include <d3d11.h>


namespace Fork
{

namespace Video
{


class D3D11RasterizerState : public RasterizerState
{
    
    public:
        
        D3D11RasterizerState(ID3D11Device* device, const Description& desc);
        ~D3D11RasterizerState();

        bool ChangeDesc(ID3D11Device* device, const Description& desc);

        inline ID3D11RasterizerState* GetStateObject() const
        {
            return stateObject_;
        }

    private:
        
        ID3D11RasterizerState* stateObject_ = nullptr;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================