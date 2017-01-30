/*
 * Scene renderer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SCENE_RENDERER_H__
#define __FORK_SCENE_RENDERER_H__


#include "Core/Export.h"
#include "Scene/Node/SceneVisitor.h"
#include "Scene/Manager/CullingManager.h"
#include "Video/RenderSystem/HardwareBuffer/VertexBuffer.h"
#include "Video/RenderSystem/HardwareBuffer/IndexBuffer.h"
#include "Math/Core/Vector3.h"


namespace Fork
{

namespace Scene
{


//! Scene renderer base class.
class FORK_EXPORT SceneRenderer : public SceneVisitor
{
    
    public:
        
        /**
        Sets the projection, view matrix and view frustum of the specified camera to the current render context.
        \param[in] camera Specifies the camera scene node from which the view will be set.
        \see Video::RenderContext::SetupProjectionm
        \see Video::RenderContext::SetupViewMatrix
        \see Video::RenderContext::SetupViewFrustum
        */
        virtual void SetupView(const CameraNode& camera);

        /**
        Renders the specified scene.
        \param[in] sceneGraph Specifies the root node of the scene graph which is to be rendered.
        If this is null, the function has no effect.
        */
        virtual void RenderScene(SceneNode* sceneGraph) = 0;

        /**
        Renders the scene graph from the specified camera's projection and view.
        This is equivalent to the following code:
        \code
        SetupView(camera);
        RenderScene(sceneGraph);
        \endcode
        \note Don't forget to update the view of the camera itself (see "CameraNode::SetupView")
        \see RenderScene(SceneNode*)
        \see CameraNode::SetupView
        */
        void RenderSceneFromCamera(SceneNode* sceneGraph, const CameraNode& camera);

        /**
        Global position of the camera which is currently used for scene rendering.
        This can be used to determine the level-of-detail when geometry will be rendered.
        */
        Math::Point3f globalCameraPosition;

        //! Scene node culling manager.
        CullingManager cullingManager;

    protected:
        
        SceneRenderer() = default;

        /**
        Simple function which draws the specified geometry.
        This is a comfort function which binds the specified buffers.
        To improve performance, only bind a buffer once and draw it several times.
        \param[in] vertexBuffer Raw-pointer to the vertex buffer which is to be drawn.
        This must never be null!
        \param[in] indexBuffer Raw-pointer to the index buffer which is to be drawn.
        If this is null, non-indexed geometry will be drawn.
        \param[in] numVertices Specifies the number of vertices which are to be drawn.
        \param[in] numIndices Specifies the number of indices which are to be drawn.
        */
        void DrawGeometry(
            Video::VertexBuffer* vertexBuffer, Video::IndexBuffer* indexBuffer,
            size_t numVertices, size_t numIndices
        );

        /**
        Sets the world matrix for the current render context and the culling manager.
        \see Video::RenderContext::SetupWorldMatrix
        \see SceneCullingManager::SetupWorldMatrix
        */
        void SetupWorldMatrix(const Math::Matrix4f& matrix);

        /**
        Global position of the current scene node.
        This must be written by each individual scene renderer.
        It should be used for LOD (level-of-detail) selections by computing the
        distance between "globalSceneNodePosition" and "globalCameraPosition".
        */
        Math::Point3f globalSceneNodePosition;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================