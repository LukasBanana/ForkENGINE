/*
 * Simple scene renderer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SIMPLE_SCENE_RENDERER_H__
#define __FORK_SIMPLE_SCENE_RENDERER_H__


#include "Scene/Renderer/ForwardSceneRenderer.h"
#include "Video/RenderSystem/Shader/ShaderComposition.h"
#include "Video/Material/BlinnPhongMaterial.h"
#include "Video/RenderSystem/Shader/StandardShader/Simple3DMeshConstantBuffers.h"


namespace Fork
{

namespace Scene
{


/**
Forward scene renderer base class. This scene renderer renders a scene
straight forward, i.e. no consequent tree hierarchy will be complied.
\see Video::RenderSystem::WVPMatrix
*/
class FORK_EXPORT SimpleSceneRenderer : public ForwardSceneRenderer
{
    
    public:
        
        /**
        Simple scene renderer constructor.
        \param[in] useNormalMapping Specifies whether normal mapping is used or not.
        If true, the "TangentSpaceMesh" shader (Video::ShaderProvider::StandardShaders::TangentSpaceMesh)
        will be used for scene rendering. Otherwise the "Simple3DMesh" shader
        (Video::ShaderProvider::StandardShaders::Simple3DMesh) will be used. By default false.
        \remarks If normal mapping is used, all rendered mehses should be a "Scene::TangentSpaceMeshGeometry",
        to provide a tangent space (cosisting of tangent, bitangent and normal vectors) for each vertex.
        Otherwise the shading behaviour is undefined!
        \see Video::ShaderProvider::StandardShaders::TangentSpaceMesh
        \see Video::ShaderProvider::StandardShaders::Simple3DMesh
        \see TangentSpaceMeshGeometry
        */
        SimpleSceneRenderer(bool useNormalMapping = false);

        void RenderScene(SceneNode* sceneGraph) override;

        /**
        Updates the light nodes for the shader.
        Only the first 'Video::Simple3DMeshShader::LightConstBuffer::maxNumLights' lights will be used.
        \see Video::Simple3DMeshShader::LightConstBuffer
        */
        void UpdateLightNodes(const std::vector<LightNode*>& lightNodes);

        void VisitSimple3DMeshGeometry      (Simple3DMeshGeometry*      node) override;
        void VisitTangentSpaceMeshGeometry  (TangentSpaceMeshGeometry*  node) override;
        void VisitTexturedGeometry          (TexturedGeometry*          node) override;

        /**
        Converts the light nodes of the specified range [begin .. end) into the specified constant buffer.
        \remarks This is also used in the "UpdateLightNodes" function.
        \throws NullPointerException If 'lightBuffer' is null.
        \see UpdateLightNodes
        */
        static void ConvertLightNodesToConstantBuffer(
            std::vector<LightNode*>::const_iterator begin,
            std::vector<LightNode*>::const_iterator end,
            Video::StandardShader::Simple3DMeshShader::LightConstBuffer::LightSource* lightBuffer,
            int maxNumLights,
            int& numLights
        );

    protected:

        void UpdateMaterial(size_t numTextures, const Video::ColorRGBAf& diffuseColor, float roughness = 0.0f);
        void UpdateMaterial(size_t numTextures, const Video::BlinnPhongMaterial& material);

    private:
        
        class SceneShaderEventHandler : public Video::ShaderComposition::EventHandler
        {
            
            public:
                
                //! \throws NullPointerException If 'vertexConstBuffer' is null.
                SceneShaderEventHandler(Video::ConstantBuffer* vertexConstBuffer);

                void OnUpdateConstantBuffers(
                    const Video::ShaderComposition* shaderComposition, Video::RenderContext* renderContext
                );

            private:
                
                Video::ConstantBuffer* vertexConstBuffer_ = nullptr;

        };

        void CreateSceneShader(bool useNormalMapping);
        void CreateEventHandler();

        Video::ShaderCompositionPtr sceneShader_;

        Video::ConstantBufferPtr    vertexConstBuffer_;
        Video::ConstantBufferPtr    materialConstBuffer_;
        Video::ConstantBufferPtr    lightConstBuffer_;

        size_t                      numTexturesBound_ = 0;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================