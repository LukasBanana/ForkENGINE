/*
 * Cube map renderer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SCENE_CUBE_MAP_RENDERER_H__
#define __FORK_SCENE_CUBE_MAP_RENDERER_H__


#include "Core/Export.h"
#include "Core/ThreeStateAssertion.h"
#include "Video/RenderSystem/Texture/TextureCube.h"
#include "Video/RenderSystem/RenderSystemFlags.h"
#include "Video/Core/Projection.h"
#include "Scene/Node/SceneNode.h"


namespace Fork
{

namespace Video
{

// Forward declarations
class RenderTarget;

}

namespace Scene
{


class SceneRenderer;

/**
Cube map renderer class. This can be used to generate static and dynamic environment maps.
\remarks Usage example:
cubeMapRenderer.Begin();
{
    cubeMapRenderer.RenderCubeMap();
}
cubeMapRenderer.End();
\see SceneRenderer
\see TextureCube
\see RenderTarget
*/
class FORK_EXPORT CubeMapRenderer
{
    
    public:
        
        CubeMapRenderer();

        /**
        Begins with cube map rendering. This will change some render states.
        \throws InvalidStateException (Only in debug mode) If "Begin" was already called before "End" was called.
        */
        void Begin();
        /**
        Ends with cube map rendering. This will reset the previous render states, before "Begin" was called.
        \throws InvalidStateException (Only in debug mode) If "End" was already called before "Begin" was called.
        */
        void End();

        /**
        Renders a cube map with the specified scene renderer.
        \param[in,out] cubeFaces Specifies the data model with render targets of all six cube faces.
        \param[in] sceneRenderer Specifies the scene renderer which will be used to render the cube faces.
        The "RenderScene" function will be used with this scene renderer.
        \param[in] sceneGraph Specifies the root node of the scene graph.
        This will passed to the "SceneRenderer::RenderScene" function.
        \param[in] cameraTransform Specifies the base transformation for the cameras when the cube faces are rendered.
        This is not a view matrix, i.e. not an inverse transformation!
        Instead it's the global transformation of the camera and commonly just a translation.
        \param[in] clearFlags Specifies the flags to clear the frame buffer when a new cube face will be rendered.
        By default Video::ClearBuffersFlags::FrameBuffer
        \see SceneRenderer::RenderScene
        \see Video::RenderContext::ClearBuffers
        \throws NullPointerException If 'sceneRenderer' is null or any render target of 'cubeFaces' is null.
        */
        void RenderCubeMap(
            const Video::TextureCube::FaceRenderTargetRefs& cubeFaces,
            SceneRenderer* sceneRenderer,
            SceneNode* sceneGraph,
            const Math::Matrix4f& cameraTransform,
            const Video::ClearBuffersFlags::DataType clearFlags = Video::ClearBuffersFlags::FrameBuffer
        );

        /**
        Renders a cube map with the specified scene renderer from the specified global position (without rotation).
        \see RenderCubeMap(const Video::TextureCube::FaceRenderTargetRefs&, SceneRenderer*, SceneNode*, const Math::Matrix4f&, const Video::ClearBuffersFlags::DataType)
        */
        void RenderCubeMap(
            const Video::TextureCube::FaceRenderTargetRefs& cubeFaces,
            SceneRenderer* sceneRenderer,
            SceneNode* sceneGraph,
            const Math::Point3f& cameraPosition,
            const Video::ClearBuffersFlags::DataType clearFlags = Video::ClearBuffersFlags::FrameBuffer
        );

        /**
        Common projection for cube face rendering. The default field-of-view is 90 degrees.
        \remarks The field-of-view should always be 90 degrees.
        Otherwise the cube faces wont match.
        */
        Video::Projectionf projection;

    private:
        
        //! Type alias to the cube map faces.
        typedef Video::TextureCube::Faces CubeFaces;

        //! Renders the specified cube map face.
        void RenderCubeMapFace(
            const CubeFaces cubeFace,
            Video::RenderTarget* renderTarget,
            SceneRenderer* sceneRenderer,
            SceneNode* sceneGraph,
            Math::Matrix4f cameraTransform,
            const Video::ClearBuffersFlags::DataType clearFlags
        );

        Video::RenderTarget*    prevRenderTarget_ = nullptr;
        Video::Viewport         prevViewport_;

        FORK_DEBUG_3STATE;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================