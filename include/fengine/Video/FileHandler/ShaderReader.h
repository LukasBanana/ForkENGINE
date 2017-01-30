/*
 * Shader reader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SHADER_READER_H__
#define __FORK_SHADER_READER_H__


#include "Video/RenderSystem/Shader/ShaderCode.h"
#include "Video/RenderSystem/Shader/Shader.h"
#include "Core/Export.h"

#include <set>


namespace Fork
{

namespace Video
{


/**
Shader reader class. This class is not required to read a shader.
But it can be used for some basic parsing functionality, e.g. "#include" directives.
\see Video::ShaderCode
*/
class FORK_EXPORT ShaderReader
{
    
    public:
        
        /* === Flags enumerations === */

        /**
        Flags for parsing a shader file.
        \see LoadFromFile
        */
        struct ParseFlags
        {
            typedef unsigned int DataType;
            enum : DataType
            {
                //! Enables parsing for "#include" directives in shader code.
                ParseIncludes       = (1 << 0),
                /**
                Translates input HLSL code into GLSL. This is done with the open-source tool "HLSLTranslator"
                \see https://github.com/LukasBanana/HLSLTranslator
                */
                TranslateHLSLtoGLSL = (1 << 1),
            };
        };

        /* === Functions === */

        /**
        Reads the sources from the specified file.
        \param[out] shaderCode Specifies the shader code which is to be modified. The previous code will be removed.
        \param[in] filename Specifies the filename for the file which is to be loaded.
        \param[in] entryPoint Specifies the 
        \param[in] flags Specifies optional parsing flags. This can be combination of the "ParseFlags" enumeration entries.
        \return True on success.
        \see ShaderCode
        \see ParseFlags
        \see version
        */
        bool ReadShaderCode(
            ShaderCode& shaderCode,
            const std::string& filename,
            const std::string& entryPoint,
            const Shader::Types targetType,
            const ParseFlags::DataType flags = 0
        ) const;

        /**
        Reads the sources from the specified file. This function ignores the entry point and target type.
        \see ReadShaderCode(ShaderCode&, const std::string&, const std::string&, const Shader::Types, const ParseFlags::DataType)
        */
        bool ReadShaderCode(
            ShaderCode& shaderCode,
            const std::string& filename,
            const ParseFlags::DataType flags = 0
        ) const;

    private:
        
        typedef std::set<std::string> IncludeSet;

        bool ReadNativeShaderCode(
            ShaderCode& shaderCode, const std::string& filename, const ParseFlags::DataType flags
        ) const;

        bool TranslateShaderCode(
            ShaderCode& shaderCode, const std::string& filename, const std::string& entryPoint,
            const Shader::Types targetType, const ParseFlags::DataType flags
        ) const;

        bool ReadFile(
            ShaderCode& shaderCode, const std::string& filename,
            const ParseFlags::DataType flags, IncludeSet& includeSet
        ) const;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================