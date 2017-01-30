/*
 * Direct3D 11 shader provider header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_D3D11_SHADER_PROVIDER_H__
#define __FORK_D3D11_SHADER_PROVIDER_H__


#include "Video/RenderSystem/Shader/ShaderProvider.h"


namespace Fork
{

namespace Video
{


//! Direct3D 11 shader provider implementation.
class D3D11ShaderProvider : public ShaderProvider
{

    public:
        
        D3D11ShaderProvider() = default;

        D3D11ShaderProvider(const D3D11ShaderProvider&) = delete;
        D3D11ShaderProvider& operator = (const D3D11ShaderProvider&) = delete;

    private:
        
        const char* QueryVertexShaderFromResource(const StandardShaders shaderName) const override;
        const char* QueryPixelShaderFromResource(const StandardShaders shaderName) const override;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================