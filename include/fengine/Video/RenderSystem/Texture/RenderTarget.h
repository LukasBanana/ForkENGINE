/*
 * Render target header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_RENDER_TARGET_H__
#define __FORK_RENDER_TARGET_H__


#include "Core/DeclPtr.h"
#include "Core/Export.h"
#include "Video/RenderSystem/Texture/Texture1D.h"
#include "Video/RenderSystem/Texture/Texture2D.h"
#include "Video/RenderSystem/Texture/Texture3D.h"
#include "Video/RenderSystem/Texture/TextureCube.h"

#include <vector>


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(RenderTarget);

/**
Render target interface. To a render target one or more textures can be attached.
A render target is an object, on which drawing operations can be performed.
\code
// --- Usage example: --- //

// Create texture
const Math::Size2i textureSize { 512, 512 };
auto renderTargetTexture = renderSystem->CreateTexture2D();
renderSystem->WriteTexture(renderTargetTexture.get(), Video::TextureFormats::RGBA, textureSize);

// Create render target and attach texture

auto renderTarget = renderSystem->CreateRenderTarget({ renderTargetTexture });

// ...

renderSystem->Bind(renderTarget.get());
{
    renderContext->ClearBuffers();
    // Draw scene into render target ...
}
renderSystem->Bind(renderContext->GetMainRenderTarget());
\endcode
\ingroup texture_classes
\see RenderSystem::Bind(RenderTarget*)
*/
class FORK_EXPORT RenderTarget
{
    
    public:
        
        //! Attachment point for a depth-map. This can be used for the 'attachment' parameter of each "Attach" function.
        static const unsigned int attachmentDepthMap = 0xffffffff;

        virtual ~RenderTarget();

        RenderTarget(const RenderTarget&) = delete;
        RenderTarget& operator = (const RenderTarget&) = delete;
        
        /**
        Attaches the specified 1D texture to this render target.
        \param[in] texture Specifies the 1D or 1D-array texture which is to be attached.
        \param[in] attachment Specifies the attachment point. The maximum number of supported attachment points
        can be queried by 'RenderContext::QueryRendererSupport' (See RendererSupport::maxNumRenderTargetAttachments).
        This can also be 'attachmentDepthMap' to attach the specified texture as a depth-map.
        \param[in] layer Specifies the layer if the texture has an array (see Texture1D::HasArray).
        \param[in] mipLevel Specifies the MIP-map level where the rendered result will be stored. By default 0 (highest MIP level).
        \return True of success, otherwise an error message will be printed into the log output.
        \throws PointerConversionException If 'texture' was not created with the same render system as this render target.
        \see Texture1D::HasArray
        \see RendererSupport::maxNumRenderTargetAttachments
        \see RenderTarget::attachmentDepthMap
        */
        virtual bool Attach(
            const Texture1DPtr& texture, unsigned int attachment = 0,
            unsigned int layer = 0, unsigned int mipLevel = 0
        ) = 0;
        /**
        Attaches the specified 2D texture to this render target.
        \see Attach(const Texture1DPtr&, unsigned int, unsigned int, unsigned int)
        */
        virtual bool Attach(
            const Texture2DPtr& texture, unsigned int attachment = 0,
            unsigned int layer = 0, unsigned int mipLevel = 0
        ) = 0;
        /**
        Attaches the specified 3D texture to this render target.
        \param[in] zOffset Specifies the Z-offset (or rather 3D texture slice) which is to be attached to this render target.
        \see Attach(const Texture1DPtr&, unsigned int, unsigned int, unsigned int)
        */
        virtual bool Attach(
            const Texture3DPtr& texture, unsigned int attachment = 0,
            unsigned int zOffset = 0, unsigned int mipLevel = 0
        ) = 0;
        /**
        Attaches the specified cube texture to this render target.
        \param[in] face Specifies the axis direction for a specific cube map face which is to be attached to this render target.
        \see Attach(const Texture1DPtr&, unsigned int, unsigned int, unsigned int)
        */
        virtual bool Attach(
            const TextureCubePtr& texture, const TextureCube::Faces face, unsigned int attachment = 0,
            unsigned int layer = 0, unsigned int mipLevel = 0
        ) = 0;

        //! Detaches all textures from this render target.
        virtual void Clear() = 0;

        /**
        Attaches all textures from the specified list.
        \return Number of attached textures. If this is not the same as the number of textures in the list,
        some textures could not be attached to this render target.
        \remarks If one of the textures could not be attached to this render target,
        an error message will be printed into the log output.
        \remarks One can also attach a single texture several times, e.g. to attach all faces of a cube map,
        just fill a list with the same cube map texture six times.
        \code
        Attach(
            { cubeTex1, cubeTex1, cubeTex1, cubeTex1, cubeTex1, cubeTex1,
              someOther2DTexture,
              cubeTex2, cubeTex2, cubeTex2, cubeTex2, cubeTex2, cubeTex2 }
        );
        \endcode
        Look at 'TextureCube::Faces' to see the order of attachments.
        \see Attach(const Texture1DPtr&, unsigned int, unsigned int)
        \see Attach(const Texture2DPtr&, unsigned int, unsigned int)
        \see Attach(const Texture3DPtr&, unsigned int, unsigned int)
        \see Attach(const TextureCubePtr&, const TextureCube::Faces, unsigned int, unsigned int)
        \see TextureCube::Faces
        */
        virtual size_t Attach(const std::vector<TexturePtr>& textures);

        /**
        Sets the multi-samples for anti-aliased render targets.
        \param[in] numSamples Specifies the number of multi-samples. By default 0.
        */
        virtual void SetupMultiSamples(int numSamples) = 0;

        //! Returns the number of multi-samples.
        inline int GetMultiSamples() const
        {
            return multiSamples_;
        }
        //! Returns true if multi-sampling is used for this render target. Is is true when 'GetMultiSample() > 0'.
        inline bool HasMultiSampling() const
        {
            return GetMultiSamples() > 0;
        }

        /**
        Returns the render target size. This is always a 2D size, although 3D textures can be attached.
        This is because for 3D textures only a single slice will be attached to the render target,
        which is just has 2D size.
        */
        inline const Math::Size2i& GetSize() const
        {
            return size_;
        }

    protected:
        
        RenderTarget() = default;

        bool ValidateMIPLevelInput(unsigned int mipLevel, bool hasMIPMaps) const;

        Math::Size2i    size_;
        int             multiSamples_ = 0;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================