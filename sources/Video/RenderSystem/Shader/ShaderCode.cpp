/*
 * Shader code file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/RenderSystem/Shader/ShaderCode.h"
#include "Video/FileHandler/ShaderReader.h"
#include "Core/StringModifier.h"


namespace Fork
{

namespace Video
{


std::string ShaderCode::ConvertToSingleBuffer() const
{
    std::string singleBuffer;

    for (const auto& line : sources)
        singleBuffer += line;

    return singleBuffer;
}

bool ShaderCode::LoadFromFile(const std::string& filename)
{
    return ShaderReader().ReadShaderCode(
        *this, filename, ShaderReader::ParseFlags::ParseIncludes
    );
}

bool ShaderCode::LoadFromFile(
    const std::string& filename, const std::string& entryPoint,
    const Shader::Types targetType, const Languages outputLanguage)
{
    /* Setup parse flags */
    ShaderReader::ParseFlags::DataType flags = ShaderReader::ParseFlags::ParseIncludes;
    
    if (outputLanguage == Languages::GLSL && ToLower(ExtractFileExt(filename)) == "hlsl")
        flags |= ShaderReader::ParseFlags::TranslateHLSLtoGLSL;

    /* Read shader code from file */
    return ShaderReader().ReadShaderCode(*this, filename, entryPoint, targetType, flags);
}

void ShaderCode::Clear()
{
    sources.clear();
}

void ShaderCode::Append(const std::string& codeLine)
{
    sources.push_back(codeLine + '\n');
}


} // /namespace Video

} // /namespace Fork



// ========================