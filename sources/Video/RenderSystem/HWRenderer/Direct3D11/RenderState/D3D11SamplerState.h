/*
 * Direct3D 11 sampler state header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_SAMPLER_STATE_H__
#define __FORK_D3D11_SAMPLER_STATE_H__


#include "Video/RenderSystem/RenderState/SamplerState.h"

#include <d3d11.h>


namespace Fork
{

namespace Video
{


class D3D11SamplerState : public SamplerState
{
    
    public:
        
        D3D11SamplerState(ID3D11Device* device, const Description& desc);
        ~D3D11SamplerState();

        bool ChangeDesc(ID3D11Device* device, const Description& desc);

        inline ID3D11SamplerState* GetStateObject() const
        {
            return stateObject_;
        }

    private:
        
        ID3D11SamplerState* stateObject_ = nullptr;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================