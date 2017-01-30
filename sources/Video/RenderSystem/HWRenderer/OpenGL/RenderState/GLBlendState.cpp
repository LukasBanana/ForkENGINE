/*
 * OpenGL blend state file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLBlendState.h"
#include "../GLParamMapper.h"


namespace Fork
{

namespace Video
{


using namespace GLParamMapper;

GLBlendState::GLBlendState(const Description& desc) :
    BlendState{ desc }
{
    ChangeDesc(desc);
}

void GLBlendState::ChangeDesc(const Description& desc)
{
    desc_ = desc;

    srcBlend_       = Map( desc_.srcBlend       );
    destBlend_      = Map( desc_.destBlend      );
    srcBlendAlpha_  = Map( desc_.srcBlendAlpha  );
    destBlendAlpha_ = Map( desc_.destBlendAlpha );
}


} // /namespace Video

} // /namespace Fork



// ========================