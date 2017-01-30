/*
 * Editor view scene renderer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_EDITOR_VIEW_SCENE_RENDERER_H__
#define __FORK_EDITOR_VIEW_SCENE_RENDERER_H__


#include "Scene/Renderer/ForwardSceneRenderer.h"
#include "EditorView.h"


namespace Fork
{

namespace Editor
{


//! Main scene renderer for the editor view.
class EditorViewSceneRenderer : public Scene::ForwardSceneRenderer
{
    
    public:
        
        EditorViewSceneRenderer();
        virtual ~EditorViewSceneRenderer();

        void RenderSceneFromEditorView(
            const Scene::CameraNode& camera,
            const EditorView::Configuration& config
        );

    private:
        
        class SimpleColoredGeometry;

        /* === Functions === */

        void CreateExtViewModels();
        void CreateExtModelShader();

        void GenerateGridModel  (SimpleColoredGeometry& geometry);
        void GenerateOriginModel(SimpleColoredGeometry& geometry);

        void DrawExtModels(const EditorView::Configuration& config);

        void DrawSimpleColoredGeometry(const SimpleColoredGeometry& geometry);

        /* === Members === */

        std::unique_ptr<SimpleColoredGeometry> gridModel_;
        std::unique_ptr<SimpleColoredGeometry> originModel_;

        Video::ShaderCompositionPtr extModelShader_;

        Video::ConstantBufferPtr constBuffer_;

};


} // /namespace Editor

} // /namespace Fork


#endif



// ========================