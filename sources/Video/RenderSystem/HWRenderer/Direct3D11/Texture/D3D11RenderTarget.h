/*
 * Direct3D 11 render target header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_RENDER_TARGET_H__
#define __FORK_D3D11_RENDER_TARGET_H__


#include "Video/RenderSystem/Texture/RenderTarget.h"
#include "D3D11FrameBuffer.h"

#include <d3d11.h>


namespace Fork
{

namespace Video
{


class D3D11RenderSystem;

//! Direct3D 11 render target implementation.
class FORK_EXPORT D3D11RenderTarget : public RenderTarget
{
    
    public:
        
        D3D11RenderTarget(D3D11RenderSystem* renderSystem);
        ~D3D11RenderTarget();

        D3D11RenderTarget(const D3D11RenderTarget&) = delete;
        D3D11RenderTarget& operator = (const D3D11RenderTarget&) = delete;
        
        bool Attach(
            const Texture1DPtr& texture, unsigned int attachment = 0,
            unsigned int layer = 0, unsigned int mipLevel = 0
        );
        bool Attach(
            const Texture2DPtr& texture, unsigned int attachment = 0,
            unsigned int layer = 0, unsigned int mipLevel = 0
        );
        bool Attach(
            const Texture3DPtr& texture, unsigned int attachment = 0,
            unsigned int zOffset = 0, unsigned int mipLevel = 0
        );
        bool Attach(
            const TextureCubePtr& texture, const TextureCube::Faces face, unsigned int attachment = 0,
            unsigned int layer = 0, unsigned int mipLevel = 0
        );

        void Clear();

        void SetupMultiSamples(int numSamples);

    protected:
        
        friend class D3D11RenderContext;
        
        inline const std::vector<ID3D11UnorderedAccessView*>& GetUnorderedAccessViews() const
        {
            return unorderedAccessViews_;
        }

        inline ID3D11Texture2D* GetDepthStencil() const
        {
            return depthStencil_;
        }

        inline const D3D11FrameBuffer& GetFrameBuffer() const
        {
            return frameBuffer_;
        }

    private:

        /* === Functions === */

        ID3D11RenderTargetView* CreateRenderTargetView(ID3D11Resource* resource, const D3D11_RENDER_TARGET_VIEW_DESC& desc);

        bool CreateDepthTexture(const Math::Size2ui& resolution);
        void ReleaseDepthTexture();

        /* === Members === */

        D3D11RenderSystem*                      renderSystem_           = nullptr;

        ID3D11Texture2D*                        depthStencil_           = nullptr;

        std::vector<ID3D11UnorderedAccessView*> unorderedAccessViews_;

        D3D11FrameBuffer                        frameBuffer_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================