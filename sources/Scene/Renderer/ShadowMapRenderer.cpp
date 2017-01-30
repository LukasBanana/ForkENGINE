/*
 * Shadow map renderer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/Renderer/ShadowMapRenderer.h"
#include "Scene/Renderer/SceneRenderer.h"
#include "Video/RenderSystem/RenderContext.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Core/Exception/NullPointerException.h"
#include "../../Video/RenderSystem/RenderSysCtx.h"


namespace Fork
{

namespace Scene
{


ShadowMapRenderer::ShadowMapRenderer()
{
    CreateShaders();
    CreateRenderStates();
}

void ShadowMapRenderer::Begin()
{
    ASSERT_3STATE_BEGIN;
    prevBlendState_ = RenderCtx()->GetRenderState().blendState;
    RenderCtx()->ChangeState(blendState_.get());
}

void ShadowMapRenderer::End()
{
    ASSERT_3STATE_END;
    RenderCtx()->ChangeState(prevBlendState_);
}

void ShadowMapRenderer::RenderShadowMap(
    Video::RenderTarget* shadowMap, const LightNode* lightNode,
    SceneRenderer* sceneRenderer, SceneNode* sceneGraph)
{
    ASSERT_POINTER(shadowMap);
    ASSERT_POINTER(lightNode);
    ASSERT_POINTER(sceneRenderer);
    ASSERT_3STATE_USE;
    
    //...
}


/*
 * ======= private: =======
 */

void ShadowMapRenderer::CreateShaders()
{
}

void ShadowMapRenderer::CreateRenderStates()
{
    Video::BlendState::Description blendDesc;
    {
        blendDesc.isBlendEnabled = false;
    }
    blendState_ = Video::RenderSystem::Active()->CreateBlendState(blendDesc);
}


} // /namespace Scene

} // /namespace Fork



// ========================