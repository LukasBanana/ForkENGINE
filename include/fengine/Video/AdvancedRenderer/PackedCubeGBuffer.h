/*
 * Packed cube g-buffer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_PACKED_CUBE_GBUFFER_H__
#define __FORK_PACKED_CUBE_GBUFFER_H__


#include "Core/DeclPtr.h"
#include "Core/Export.h"
#include "Video/RenderSystem/Texture/TextureCube.h"
#include "Video/RenderSystem/Texture/RenderTarget.h"


namespace Fork
{

namespace Video
{


/**
Cube Map Packed G- ("Geometry") Buffer class. This g-buffer compresses four maps into two textures:
Color (1st RGB), Specular (1st alpha chanel), Normal (2nd RGB) and Depth (2nd alpha chanel).
\ingroup gbuffer
*/
class FORK_EXPORT PackedCubeGBuffer
{
    
    public:
        
        //! Texture type for this g-buffer.
        typedef TextureCube TextureType;
        //! Texture shared pointer type for this g-buffer.
        typedef TextureCubePtr TexturePtrType;

        PackedCubeGBuffer(
            const TextureType::SizeType& initSize,
            const TextureFormats colorAndSpecularMapFormat = TextureFormats::RGBA8UInt,
            const TextureFormats normalAndDepthMapFormat = TextureFormats::RGBA16Float
        );
        virtual ~PackedCubeGBuffer();

        //! Resizes all textures of this g-buffer.
        virtual void Resize(const TextureType::SizeType& size);

        //! Returns the color- and specular map.
        inline TextureType* GetColorAndSpecularMap() const
        {
            return colorAndSpecularMap.get();
        }
        //! Returns a shared pointer to the color- and specular map.
        inline const TexturePtrType& GetSharedColorAndSpecularMap() const
        {
            return colorAndSpecularMap;
        }

        //! Returns the normal- and depth map.
        inline TextureType* GetNormalAndDepthMap() const
        {
            return normalAndDepthMap.get();
        }
        //! Returns a shared pointer to the normal- and depth map.
        inline const TexturePtrType& GetSharedNormalAndDepthMap() const
        {
            return normalAndDepthMap;
        }

        //! Returns the cube map render target references.
        inline TextureCube::FaceRenderTargetRefs GetCubeRenderTargetRefs() const
        {
            return cubeRenderTargetRefs;
        }

    protected:
        
        TexturePtrType colorAndSpecularMap, normalAndDepthMap;
        TextureCube::FaceRenderTargets cubeRenderTargets;
        TextureCube::FaceRenderTargetRefs cubeRenderTargetRefs;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================