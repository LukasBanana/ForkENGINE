/*
 * Shadow map renderer header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SCENE_SHADOW_MAP_RENDERER_H__
#define __FORK_SCENE_SHADOW_MAP_RENDERER_H__


#include "Core/Export.h"
#include "Core/ThreeStateAssertion.h"
#include "Video/RenderSystem/Shader/ShaderComposition.h"
#include "Video/RenderSystem/RenderState/BlendState.h"


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
class SceneNode;
class LightNode;

/**
Shadow map renderer base class. This can be used to generate shadow maps for different .
\see SceneRenderer
*/
class FORK_EXPORT ShadowMapRenderer
{
    
    public:
        
        ShadowMapRenderer();

        /**
        Begins with shadow map rendering. This will change some render states.
        \throws InvalidStateException (Only in debug mode) If "Begin" was already called before "End" was called.
        */
        void Begin();
        /**
        Ends with shadow map rendering. This will reset the previous render states, before "Begin" was called.
        \throws InvalidStateException (Only in debug mode) If "End" was already called before "Begin" was called.
        */
        void End();

        /**
        Renders the shadow map for the specified light node.
        \param[in,out] shadowMap Specifies the render target in which the shadow map is rendered.
        \param[in] lightNode Specifies the light scene node from which the scene is rendered.
        \param[in] sceneRenderer Specifies the scene renderer which is used to traverse the scene graph.
        \param[in] sceneGraph Specifies the root scene node of the scene graph. If this is null,
        no scene is rendered into the shadow map (but the depth and color buffers are cleared).
        \throws NullPointerException If 'shadowMAap' is null, 'lightNode' is null or 'sceneRenderer' is null.
        */
        void RenderShadowMap(
            Video::RenderTarget* shadowMap,
            const LightNode* lightNode,
            SceneRenderer* sceneRenderer,
            SceneNode* sceneGraph
        );

    private:
        
        void CreateShaders();
        void CreateRenderStates();

        Video::ShaderCompositionPtr vsmShadowShader_; //!< Shader to generate variance shadow maps (VSM).
        Video::ShaderCompositionPtr rsmShaderShader_; //!< Shader to generate reflective shadow maps (RSM).

        const Video::BlendState*    prevBlendState_ = nullptr;
        Video::BlendStatePtr        blendState_;

        FORK_DEBUG_3STATE;

};


} // /namespace Scene

} // /namespace Fork


#endif



// ========================