/*
 * Cube g-buffer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_CUBE_GBUFFER_H__
#define __FORK_CUBE_GBUFFER_H__


#include "Core/DeclPtr.h"
#include "Core/Export.h"
#include "Video/RenderSystem/Texture/TextureCube.h"
#include "Video/RenderSystem/Texture/RenderTarget.h"


namespace Fork
{

namespace Video
{


/**
Cube Map G- ("Geometry") Buffer class. A g-buffer consists of several textures (or rather render targets),
where this class provides: color-, normal- and depth maps.
\ingroup gbuffer
*/
class FORK_EXPORT CubeGBuffer
{
    
    public:
        
        //! Texture type for this g-buffer.
        typedef TextureCube TextureType;
        //! Texture shared pointer type for this g-buffer.
        typedef TextureCubePtr TexturePtrType;

        CubeGBuffer(
            const TextureType::SizeType& initSize,
            const TextureFormats colorMapFormat = TextureFormats::RGBA8UInt,
            const TextureFormats normalMapFormat = TextureFormats::RGB16Float,
            const TextureFormats depthMapFormat = TextureFormats::R32Float
        );
        virtual ~CubeGBuffer();

        //! Resizes all textures of this g-buffer.
        virtual void Resize(const TextureType::SizeType& size);

        //! Returns the color map.
        inline TextureType* GetColorMap() const
        {
            return colorMap.get();
        }
        //! Returns a shared pointer to the color map.
        inline const TexturePtrType& GetSharedColorMap() const
        {
            return colorMap;
        }

        //! Returns the normal map.
        inline TextureType* GetNormalMap() const
        {
            return normalMap.get();
        }
        //! Returns a shared pointer to the normal map.
        inline const TexturePtrType& GetSharedNormalMap() const
        {
            return normalMap;
        }

        //! Returns the depth map.
        inline TextureType* GetDepthMap() const
        {
            return depthMap.get();
        }
        //! Returns a shared pointer to the depth map.
        inline const TexturePtrType& GetSharedDepthMap() const
        {
            return depthMap;
        }

        //! Returns the cube map render target references.
        inline TextureCube::FaceRenderTargetRefs GetCubeRenderTargetRefs() const
        {
            return cubeRenderTargetRefs;
        }

    protected:
        
        TexturePtrType colorMap, normalMap, depthMap;
        TextureCube::FaceRenderTargets cubeRenderTargets;
        TextureCube::FaceRenderTargetRefs cubeRenderTargetRefs;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================