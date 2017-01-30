/*
 * OpenGL depth stencil state file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLDepthStencilState.h"
#include "../GLParamMapper.h"


namespace Fork
{

namespace Video
{


using namespace GLParamMapper;

GLDepthStencilState::GLDepthStencilState(const Description& desc) :
    DepthStencilState{ desc }
{
    ChangeDesc(desc);
}

void GLDepthStencilState::ChangeDesc(const Description& desc)
{
    desc_ = desc;

    depthFunc_ = GLParamMapper::Map(desc_.depthFunc);

    auto SetupFace = [](GLStencilOpDescription& faceGL, const StencilOpDescription& face)
    {
        faceGL.stencilFailOp    = Map( face.stencilFailOp  );
        faceGL.stencilZFailOp   = Map( face.stencilZFailOp );
        faceGL.stencilZPassOp   = Map( face.stencilZPassOp );
        faceGL.stencilFunc      = Map( face.stencilFunc    );
    };

    SetupFace(frontFace_, desc_.frontFace);
    SetupFace(backFace_, desc_.backFace);
}


} // /namespace Video

} // /namespace Fork



// ========================