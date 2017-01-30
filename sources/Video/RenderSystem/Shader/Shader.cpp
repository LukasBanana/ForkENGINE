/*
 * Shader file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/RenderSystem/Shader/Shader.h"
#include "Video/RenderSystem/Shader/ShaderCode.h"


namespace Fork
{

namespace Video
{


Shader::Shader() :
    sourceCode{ std::make_shared<ShaderCode>() }
{
}
Shader::~Shader()
{
}

bool Shader::IsGLSL(const Versions version)
{
    return version >= Versions::GLSL_110 && version <= Versions::GLSL_440;
}

bool Shader::IsHLSL(const Versions version)
{
    return version >= Versions::HLSL_4_0 && version <= Versions::HLSL_5_0;
}

std::map<std::string, Shader::CompilationFlags::DataType> Shader::CompilationFlagsMaps()
{
    typedef Shader::CompilationFlags ShaderFlags;
    return
    {
        { "-skip-opt",  ShaderFlags::SkipOptimization  },
        { "-skip-val",  ShaderFlags::SkipValidation    },
        { "-debug",     ShaderFlags::DebugInfo         },
        { "-Werror",    ShaderFlags::WarningsAreErrors },
        { "-O0",        ShaderFlags::O0                },
        { "-O1",        ShaderFlags::O1                },
        { "-O2",        ShaderFlags::O2                },
        { "-O3",        ShaderFlags::O3                },
        { "-dis-asm",   ShaderFlags::DisassemblyOutput },
    };
}


} // /namespace Video

} // /namespace Fork



// ========================