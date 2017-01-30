/*
 * OpenGL rasterizer state file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLRasterizerState.h"
#include "../GLParamMapper.h"


namespace Fork
{

namespace Video
{


using namespace GLParamMapper;

GLRasterizerState::GLRasterizerState(const Description& desc) :
    RasterizerState{ desc }
{
    ChangeDesc(desc);
}

void GLRasterizerState::ChangeDesc(const Description& desc)
{
    desc_ = desc;

    fillMode_ = Map(desc_.fillMode);
    cullMode_ = Map(desc_.cullMode);
}


} // /namespace Video

} // /namespace Fork



// ========================