/*
 * Forward scene renderer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_FORWARD_SCENE_RENDERER_H__
#define __FORK_FORWARD_SCENE_RENDERER_H__


#include "Scene/Renderer/SceneRenderer.h"


namespace Fork
{

namespace Video { class ShaderComposition; }

namespace Scene
{


/**
Forward scene renderer base class. This scene renderer renders a scene
straight forward, i.e. no consequent tree hierarchy will be complied.
\see Video::RenderSystem::WVPMatrix
*/
class FORK_EXPORT ForwardSceneRenderer : public SceneRenderer
{
    
    public:
        
        void RenderScene(SceneNode* sceneGraph) override;

        void VisitGeometryNode              (GeometryNode*              node) override;

        void VisitLODGeometry               (LODGeometry*               node) override;
        void VisitCompositionGeometry       (CompositionGeometry*       node) override;
        void VisitTexturedGeometry          (TexturedGeometry*          node) override;

        void VisitSimple3DMeshGeometry      (Simple3DMeshGeometry*      node) override;
        void VisitTangentSpaceMeshGeometry  (TangentSpaceMeshGeometry*  node) override;

    private:
        
        const Video::ShaderComposition* prevShader_ = nullptr;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================