/*
 * Editor selection scene renderer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_SELECTION_SCENE_RENDERER_H__
#define __FORK_EDITOR_SELECTION_SCENE_RENDERER_H__


#include "Scene/Renderer/ForwardSceneRenderer.h"
#include "Video/RenderSystem/RenderState/DepthStencilState.h"
#include "Video/RenderSystem/RenderState/RasterizerState.h"
#include "Video/RenderSystem/RenderState/BlendState.h"
#include "Video/RenderSystem/Shader/ShaderComposition.h"
#include "Video/Core/ColorRGBA.h"


namespace Fork
{

namespace Editor
{


//! Selection highlight renderer for the editor view.
class EditorSelectionSceneRenderer : private Scene::ForwardSceneRenderer
{
    
    public:
        
        EditorSelectionSceneRenderer();

        void RenderSelectionHighlights(
            const std::vector<Scene::DynamicSceneNode*>& highlightedSceneNodes,
            const Scene::CameraNode& camera,
            const Video::ColorRGBAf& highlightColor = { 1.0f, 1.0f, 0.0f },
            float highlightSize = 2.0f
        );

    private:
        
        /* === Functions === */

        void CreateShader();
        void CreateRenderStates();

        void HighlightPass1();
        void HighlightPass2();

        /* === Members === */

        Video::ShaderCompositionPtr solidColoredShader_;
        Video::ConstantBufferPtr    constBuffer_;

        Video::DepthStencilStatePtr stencilState1_;
        Video::DepthStencilStatePtr stencilState2_;
        
        Video::RasterizerStatePtr   rasterState1_;
        Video::RasterizerStatePtr   rasterState2_;

        Video::BlendStatePtr        blendState1_;
        Video::BlendStatePtr        blendState2_;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================