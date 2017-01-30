/*
 * Direct3D 11 blend state header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_BLEND_STATE_H__
#define __FORK_D3D11_BLEND_STATE_H__


#include "Video/RenderSystem/RenderState/BlendState.h"

#include <d3d11.h>


namespace Fork
{

namespace Video
{


class D3D11BlendState : public BlendState
{
    
    public:
        
        D3D11BlendState(ID3D11Device* device, const Description& desc);
        ~D3D11BlendState();

        bool ChangeDesc(ID3D11Device* device, const Description& desc);

        inline ID3D11BlendState* GetStateObject() const
        {
            return stateObject_;
        }
        inline UINT GetSampleMask() const
        {
            return sampleMask_;
        }

    private:
        
        ID3D11BlendState*   stateObject_    = nullptr;
        UINT                sampleMask_     = 0xffffffff;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================