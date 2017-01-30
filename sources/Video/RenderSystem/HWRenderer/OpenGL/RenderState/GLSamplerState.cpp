/*
 * OpenGL sampler state file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLSamplerState.h"
#include "../GLParamMapper.h"
#include "../GLExtensions.h"


namespace Fork
{

namespace Video
{


using namespace GLParamMapper;

GLSamplerState::GLSamplerState(const Description& desc) :
    /*
    Store initial GL sampler sates first,
    so that 'ChageDesc' will take effect.
    */
    SamplerState{ InitGLDesc() }
{
    glGenSamplers(1, &samplerID_);
    ChangeDesc(desc);
}
GLSamplerState::~GLSamplerState()
{
    glDeleteSamplers(1, &samplerID_);
}

void GLSamplerState::ChangeDesc(const Description& desc)
{
    /* Update filter states */
    if (desc_.minFilter != desc.minFilter || desc_.mipFilter != desc.mipFilter)
        glSamplerParameteri(GetSamplerID(), GL_TEXTURE_MIN_FILTER, Map(desc.minFilter, desc.mipFilter, desc.useMIPMaps));

    if (desc_.magFilter != desc.magFilter)
        glSamplerParameteri(GetSamplerID(), GL_TEXTURE_MAG_FILTER, Map(desc.magFilter));
        
    if (desc_.anisotropy != desc.anisotropy)
        glSamplerParameterf(GetSamplerID(), GL_TEXTURE_MAX_ANISOTROPY_EXT, static_cast<float>(desc.anisotropy));

    /* Update MIP selection settings */
    glSamplerParameterf(GetSamplerID(), GL_TEXTURE_MIN_LOD, desc.minLOD);
    glSamplerParameterf(GetSamplerID(), GL_TEXTURE_MAX_LOD, desc.maxLOD);
    glSamplerParameterf(GetSamplerID(), GL_TEXTURE_LOD_BIAS, desc.mipLODBias);

    /* Update texture coordiante wrap modes */
    if (desc_.wrapMode.u != desc.wrapMode.u)
        glSamplerParameteri(GetSamplerID(), GL_TEXTURE_WRAP_S, Map(desc.wrapMode.u));

    if (desc_.wrapMode.v != desc.wrapMode.v)
        glSamplerParameteri(GetSamplerID(), GL_TEXTURE_WRAP_T, Map(desc.wrapMode.v));

    if (desc_.wrapMode.w != desc.wrapMode.w)
        glSamplerParameteri(GetSamplerID(), GL_TEXTURE_WRAP_R, Map(desc.wrapMode.w));

    /* Store new description and store GL parameters */
    desc_ = desc;
}

void GLSamplerState::Bind(unsigned int layer) const
{
    glBindSampler(layer, GetSamplerID());
}

void GLSamplerState::Unbind(unsigned int layer) const
{
    glBindSampler(layer, 0);
}


/*
 * ======= Private: =======
 */

SamplerState::Description GLSamplerState::InitGLDesc() const
{
    /* Change the description to the initial GL sampler states */
    Description desc;
    {
        desc.minFilter = SamplerState::Filters::Nearest;
    }
    return desc;
}


} // /namespace Video

} // /namespace Fork



// ========================