/*
 * Renderer support header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_RENDERER_SUPPORT_H__
#define __FORK_RENDERER_SUPPORT_H__


#include "Video/RenderSystem/Texture/Texture1D.h"
#include "Video/RenderSystem/Texture/Texture2D.h"
#include "Video/RenderSystem/Texture/Texture3D.h"
#include "Video/RenderSystem/Texture/TextureCube.h"


namespace Fork
{

namespace Video
{


//! Hardware renderer support structure.
struct RendererSupport
{
    RendererSupport() = default;
    RendererSupport(
        bool hasRenderTargets,
        bool hasGeometryShaders,
        bool hasTessellationShaders,
        bool hasComputeShaders,
        bool hasArrayTextures,
        bool hasArrayCubeTextures,
        bool hasHardwareInstancing,
        bool hasViewportArray,
        unsigned int maxNumArrayTextureLayers,
        unsigned int maxNumRenderTargetAttachments,
        unsigned int maxConstantBufferSize,
        const Math::Vector3<unsigned int>& maxNumComputeShaderWorkGroup,
        const Math::Vector3<unsigned int>& maxComputeShaderWorkGroupSize,
        int maxTexture1DSize,
        int maxTexture2DSize,
        int maxTexture3DSize,
        int maxTextureCubeSize,
        int maxAnisotropy) :
            hasRenderTargets                { hasRenderTargets              },
            hasGeometryShaders              { hasGeometryShaders            },
            hasTessellationShaders          { hasTessellationShaders        },
            hasComputeShaders               { hasComputeShaders             },
            hasArrayTextures                { hasArrayTextures              },
            hasArrayCubeTextures            { hasArrayCubeTextures          },
            hasHardwareInstancing           { hasHardwareInstancing         },
            hasViewportArray                { hasViewportArray              },
            maxNumArrayTextureLayers        { maxNumArrayTextureLayers      },
            maxNumRenderTargetAttachments   { maxNumRenderTargetAttachments },
            maxConstantBufferSize           { maxConstantBufferSize         },
            maxNumComputeShaderWorkGroup    { maxNumComputeShaderWorkGroup  },
            maxComputeShaderWorkGroupSize   { maxComputeShaderWorkGroupSize },
            maxTexture1DSize                { maxTexture1DSize              },
            maxTexture2DSize                { maxTexture2DSize              },
            maxTexture3DSize                { maxTexture3DSize              },
            maxTextureCubeSize              { maxTextureCubeSize            },
            maxAnisotropy                   { maxAnisotropy                 }
    {
    }

    /**
    Specifies whether render targets are supported or not.
    \see RenderSystem::CreateRenderTarget
    \see RenderSystem::Bind(RenderTarget*)
    */
    bool hasRenderTargets { false };

    /**
    Specifies whether geometry shaders are supported or not.
    In OpenGL since version 3.2, in Direct3D since version 10.0.
    \see RenderSystem::CreateGeometryShader
    */
    bool hasGeometryShaders { false };

    /**
    Specifies whether tessellation shaders are supported or not.
    \see RenderSystem::CreateTessControlShader
    \see RenderSystem::CreateTessEvaluationShader
    */
    bool hasTessellationShaders { false };

    /**
    Specifies whether compute shaders are supported or not.
    In OpenGL since version 4.2, in Direct3D since version 11.0.
    \see RenderSystem::CreateComputeShader
    \see RenderSystem::Dispatch
    */
    bool hasComputeShaders { false };

    //! Specifies whether array textures are supported or not.
    bool hasArrayTextures { false };

    //! Specifies whether array cube textures are supported or not.
    bool hasArrayCubeTextures { false };

    /**
    Specifies whether hardware instancing is supported or not.
    \see RenderSystem::DrawInstanced
    \see RenderSystem::DrawInstancedIndexed
    */
    bool hasHardwareInstancing { false };

    /**
    Specifies whether multiple viewports, depth-ranges and scissors are supported or not.
    \see RenderSystem::SetupMultiViewport
    */
    bool hasViewportArray { false };

    //! Maximum number of array texture layers (for 1D-, 2D- and Cube textures).
    unsigned int maxNumArrayTextureLayers { 0 };

    //! Maximum number of attachment points for each render target.
    unsigned int maxNumRenderTargetAttachments { 0 };

    //! Maximum size (in bytes) of a constant buffer.
    unsigned int maxConstantBufferSize { 0 };

    //! Maximum number of work groups in a compute shader.
    Math::Vector3<unsigned int> maxNumComputeShaderWorkGroup { 0 };

    //! Maximum work group size in a compute shader.
    Math::Vector3<unsigned int> maxComputeShaderWorkGroupSize { 0 };

    //! Maximum 1D texture size.
    int maxTexture1DSize { 0 };

    //! Maximum 2D texture size (for with and height).
    int maxTexture2DSize { 0 };

    //! Maximum 3D texture size (for width, height and depth).
    int maxTexture3DSize { 0 };

    //! Maximum cube texture size (for width and height).
    int maxTextureCubeSize { 0 };

    //! Maxmimum anisotropy texture filter.
    int maxAnisotropy { 0 };
};


} // /namespace Video

} // /namespace Fork


#endif



// ========================