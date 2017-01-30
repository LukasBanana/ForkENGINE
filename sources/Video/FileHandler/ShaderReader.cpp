/*
 * Printer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/FileHandler/ShaderReader.h"
#include "IO/Core/Log.h"
#include "Core/StringModifier.h"

#include <fstream>
#include <sstream>
//#include <HT/Translator.h>


namespace Fork
{

namespace Video
{

#if 0

/*
 * Internal classes
 */

class HTOutputLog : public HTLib::Logger
{
    
    public:
        
        void Info(const std::string& message) override
        {
            IO::Log::Message(message);
        }

        void Warning(const std::string& message) override
        {
            IO::Log::Warning(message);
        }

        void Error(const std::string& message) override
        {
            IO::Log::Error(message);
        }

        void IncIndent() override
        {
            IO::Log::IncIndent();
        }

        void DecIndent() override
        {
            IO::Log::DecIndent();
        }

};

class HTIncludeStreamHandler : public HTLib::IncludeHandler
{
    
    public:
        
        std::shared_ptr<std::istream> Include(std::string& includeName) override
        {
            return std::make_shared<std::ifstream>(includeName);
        }

};

#endif


/*
 * ShaderReader class
 */

bool ShaderReader::ReadShaderCode(
    ShaderCode& shaderCode, const std::string& filename, const std::string& entryPoint,
    const Shader::Types targetType, const ParseFlags::DataType flags) const
{
    return (flags & ParseFlags::TranslateHLSLtoGLSL) != 0 ?
        TranslateShaderCode(shaderCode, filename, entryPoint, targetType, flags) :
        ReadNativeShaderCode(shaderCode, filename, flags);
}

bool ShaderReader::ReadShaderCode(
    ShaderCode& shaderCode, const std::string& filename, const ParseFlags::DataType flags) const
{
    return ReadShaderCode(shaderCode, filename, "", Shader::Types::Vertex, flags);
}


/*
 * ======= Private: =======
 */

bool ShaderReader::ReadNativeShaderCode(
    ShaderCode& shaderCode, const std::string& filename, const ParseFlags::DataType flags) const
{
    /* Read native shader code */
    shaderCode.Clear();
    IncludeSet includeSet;
    return ReadFile(shaderCode, filename, flags, includeSet);
}

#if 0

// Converts the shader type into a shader target for the "HLSLTranslator"
static HTLib::ShaderTargets TargetFromShaderType(const Shader::Types targetType)
{
    typedef HTLib::ShaderTargets Ty;

    switch (targetType)
    {
        case Shader::Types::Vertex:
            return Ty::GLSLVertexShader;
        case Shader::Types::Pixel:
            return Ty::GLSLFragmentShader;
        case Shader::Types::TessControl:
            return Ty::GLSLTessControlShader;
        case Shader::Types::TessEvaluation:
            return Ty::GLSLTessEvaluationShader;
        case Shader::Types::Compute:
            return Ty::GLSLComputeShader;
    }

    return Ty::CommonShader;
}

#endif

bool ShaderReader::TranslateShaderCode(
    ShaderCode& shaderCode, const std::string& filename, const std::string& entryPoint,
    const Shader::Types targetType, const ParseFlags::DataType flags) const
{
    #if 0
    /* Setup input parameters */
    HTOutputLog log;
    HTIncludeStreamHandler includeHandler;
    HTLib::Options options;

    auto inputStream = std::make_shared<std::ifstream>(filename);
    std::stringstream outputStream;

    /* Translate HLSL shader code to GLSL */
    auto result = HTLib::TranslateHLSLtoGLSL(
        inputStream,
        outputStream,
        entryPoint,
        TargetFromShaderType(targetType),
        HTLib::InputShaderVersions::HLSL5,
        HTLib::OutputShaderVersions::GLSL330,
        &includeHandler,
        options,
        &log
    );

    if (result)
    {
        /* Store shader output in shader code object */
        std::string line;
        while (std::getline(outputStream, line))
            shaderCode.Append(line);

        return true;
    }
    #endif
    return false;
}

static bool IgnoreWhiteSpaces(const char*& str, bool isRequired = false)
{
    /* Check if first character is a white space */
    if (isRequired)
    {
        if (!IsWhiteSpace(*str))
            return false;
        ++str;
    }
    
    /* Ignore all following white spaces */
    while (*str)
    {
        if (!IsWhiteSpace(*str))
            return true;
        ++str;
    }
    
    return false;
}

static bool ParseIdentifier(const char*& str, const char* ident)
{
    if (!ident)
        return false;

    /* Check that the directive's identifier spelling is equal to 'ident' */
    while (*ident && *str)
    {
        if (*str != *ident)
            return false;
        ++str;
        ++ident;
    }
    
    if (*ident)
    {
        /* Null termination character of the compare string has not been reached */
        return false;
    }

    return true;
}

/* --- directive '#include "FILE"' --- */

static bool ParseIncludeDirectiveFilename(const char* str, std::string& filename)
{
    bool quoteStarted = false;

    /* Find first '\"' character */
    for (; *str != 0; ++str)
    {
        if (*str == '\"')
        {
            if (quoteStarted)
            {
                /*
                Quote has already been started
                -> End reached, so return with success.
                */
                return true;
            }
            quoteStarted = true;
        }
        else if (quoteStarted)
        {
            /* Record string and output filename */
            filename += *str;
        }
    }

    return false;
}

static bool ParseIncludeDirectiveBegin(const char* str, std::string& filename)
{
    return
        IgnoreWhiteSpaces(str) &&
        ParseIdentifier(str, "include") &&
        IgnoreWhiteSpaces(str) &&
        ParseIncludeDirectiveFilename(str, filename);
}

static bool ParseIncludeDirective(const std::string& line, std::string& filename)
{
    auto str = line.c_str();
    IgnoreWhiteSpaces(str);
    return (*str == '#') ? ParseIncludeDirectiveBegin(++str, filename) : false;
}

/* --- directive '#pragma once' --- */

static bool ParsePragmaOnceDirective(const std::string& line)
{
    auto str = line.c_str();
    IgnoreWhiteSpaces(str);
    
    if (*str == '#')
    {
        ++str;
        return
            IgnoreWhiteSpaces(str) &&
            ParseIdentifier(str, "pragma") &&
            IgnoreWhiteSpaces(str, true) &&
            ParseIdentifier(str, "once");
    }

    return false;
}

bool ShaderReader::ReadFile(
    ShaderCode& shaderCode, const std::string& filename, const ParseFlags::DataType flags, IncludeSet& includeSet) const
{
    /* Open file for reading */
    IO::Log::Message("Read shader file: \"" + filename + "\"");
    IO::Log::ScopedIndent unused;

    std::ifstream file(filename);

    if (file.fail())
    {
        IO::Log::Error("Reading shader file failed");
        return false;
    }

    /* Get file part to reject files with the '#pragma once' directive */
    auto filePart = ExtractFileName(filename);
    MakeLower(filePart);

    /* Read each source line */
    while (!file.eof())
    {
        /* Read next line from file */
        std::string line;
        std::getline(file, line);

        /* Check for parsing flags */
        if (flags)
        {
            /* Check for "#include" directive */
            if ( (flags & ParseFlags::ParseIncludes) != 0 )
            {
                std::string subFilename;
                if (ParseIncludeDirective(line, subFilename))
                {
                    /* Read sub file */
                    const auto path = ExtractFilePath(filename);

                    if (!ReadFile(shaderCode, path + "/" + subFilename, flags, includeSet))
                        return false;

                    continue;
                }
                else if (ParsePragmaOnceDirective(line))
                {
                    /* Check if file has already been read */
                    if (includeSet.find(filePart) != includeSet.end())
                        return true;

                    /* Add this file to the include set */
                    includeSet.insert(filePart);

                    continue;
                }
            }
        }

        /* Append line to sources and add new-line character */
        shaderCode.Append(line);
    }

    return true;
}


} // /namespace Video

} // /namespace Fork



// ========================