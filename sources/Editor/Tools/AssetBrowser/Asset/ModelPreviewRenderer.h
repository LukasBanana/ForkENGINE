/*
 * Editor model preview renderer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_MODEL_PREVIEW_RENDERER_H__
#define __FORK_EDITOR_MODEL_PREVIEW_RENDERER_H__


#include "Scene/Renderer/SimpleSceneRenderer.h"
#include "Scene/Node/GeometryNode.h"
#include "Scene/Node/CameraNode.h"


namespace Fork
{

namespace Editor
{


class ModelUIAsset;

//! Editor model asset class.
class ModelPreviewRenderer : private Scene::SimpleSceneRenderer
{
    
    public:
        
        ModelPreviewRenderer();

        /**
        Renders the preview for the specified model asset.
        \note The viewport and render-target must have already been set.
        */
        void RenderModelAssetPreview(
            const ModelUIAsset& asset, const Math::Size2i& previewSize, const Math::Quaternionf& rotation = {}
        );

        //! Changes the preview projection between perspective and orthographic.
        void ChangeProjection(bool isOrtho);
        //! Switches the preview projection between perspective and orthographic.
        void ChangeProjection();

    private:

        Scene::SceneNode        sceneGraph_;
        Scene::GeometryNodePtr  geometryNode_;
        Scene::CameraNode       previewCamera_;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================