/*
 * Shader manager file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/RenderSystem/Shader/ShaderManager.h"
#include "Video/RenderSystem/RenderSystem.h"
#include "Video/RenderSystem/Shader/ShaderCompilationException.h"
#include "Core/Exception/InvalidArgumentException.h"


namespace Fork
{

namespace Video
{


/*
 * Internal functions
 */

static void ResolveTargetVersion(Shader::Versions& targetVersion)
{
    if (targetVersion == Shader::Versions::__Unspecified__)
        targetVersion = RenderContext::Active()->HighestShaderModel();
}

static std::vector<std::string> DefaultEntryPoints(const char* functionName, size_t num)
{
    switch (num)
    {
        case 2:
            return { "VertexMain", "PixelMain" };
        case 3:
            return { "VertexMain", "Geometry", "PixelMain" };
        case 4:
            return { "VertexMain", "HullMain", "DomainMain", "PixelMain" };
        case 5:
            return { "VertexMain", "HullMain", "DomainMain", "Geometry", "PixelMain" };
        default:
            throw InvalidArgumentException(functionName, "entryPoints", "Invalid number of entry points to detect type of shader composition");
    }
    return {};
}

static void LoadShaderFile(
    Shader* shader, const std::string& filename, const std::string& entryPoint, const Shader::Versions targetVersion)
{
    shader->sourceCode->LoadFromFile(filename, entryPoint, shader->Type(), RenderContext::Active()->ShadingLanguage());
    shader->entryPoint = entryPoint;
    shader->targetVersion = targetVersion;
}

static void CreateShader(
    Shader* shader, const std::string& sources, const std::string& entryPoint, const Shader::Versions targetVersion)
{
    shader->sourceCode->Append(sources);
    shader->targetVersion = targetVersion;
    shader->entryPoint = entryPoint;
}


/*
 * ShaderManager class
 */

ShaderCompositionPtr ShaderManager::CreateShaderCompositionFromFiles(
    const std::vector<std::string>& filenames,
    const std::vector<std::string>& entryPoints,
    const VertexFormat& vertexFormat,
    Shader::Versions targetVersion)
{
    if (filenames.size() != entryPoints.size())
        throw InvalidArgumentException(__FUNCTION__, "entryPoints", "Number of filenames and entry points must be equal");

    ResolveTargetVersion(targetVersion);

    /* Get active render system */
    auto renderSystem = RenderSystem::Active(true);

    /* Create shader composition */
    auto shaderComposition = renderSystem->CreateShaderComposition();

    /*
    Create the shaders, load them from file and
    attach them to the shader composition
    */
    const auto num = filenames.size();

    ShaderFilenamesType shaderFilenames;

    switch (num)
    {
        case 2:
        {
            auto vertexShader   = renderSystem->CreateVertexShader  ();
            auto pixelShader    = renderSystem->CreatePixelShader   ();

            LoadShaderFile( vertexShader.get(), shaderFilenames.vertex = filenames[0], entryPoints[0], targetVersion );
            LoadShaderFile( pixelShader.get(),  shaderFilenames.pixel  = filenames[1], entryPoints[1], targetVersion );

            shaderComposition->AttachShader(vertexShader, pixelShader);
        }
        break;

        case 3:
        {
            auto vertexShader   = renderSystem->CreateVertexShader  ();
            auto geometryShader = renderSystem->CreateGeometryShader();
            auto pixelShader    = renderSystem->CreatePixelShader   ();

            LoadShaderFile( vertexShader.get(),   shaderFilenames.vertex   = filenames[0], entryPoints[0], targetVersion);
            LoadShaderFile( geometryShader.get(), shaderFilenames.geometry = filenames[1], entryPoints[1], targetVersion);
            LoadShaderFile( pixelShader.get(),    shaderFilenames.pixel    = filenames[2], entryPoints[2], targetVersion);

            shaderComposition->AttachShader(vertexShader, geometryShader, pixelShader);
        }
        break;

        case 4:
        {
            auto vertexShader           = renderSystem->CreateVertexShader          ();
            auto tessControlShader      = renderSystem->CreateTessControlShader     ();
            auto tessEvaluationShader   = renderSystem->CreateTessEvaluationShader  ();
            auto pixelShader            = renderSystem->CreatePixelShader           ();

            LoadShaderFile( vertexShader.get(),         shaderFilenames.vertex         = filenames[0], entryPoints[0], targetVersion);
            LoadShaderFile( tessControlShader.get(),    shaderFilenames.tessControl    = filenames[1], entryPoints[1], targetVersion);
            LoadShaderFile( tessEvaluationShader.get(), shaderFilenames.tessEvaluation = filenames[2], entryPoints[2], targetVersion);
            LoadShaderFile( pixelShader.get(),          shaderFilenames.pixel          = filenames[3], entryPoints[3], targetVersion);

            shaderComposition->AttachShader(
                vertexShader, tessControlShader, tessEvaluationShader, pixelShader
            );
        }
        break;

        case 5:
        {
            auto vertexShader           = renderSystem->CreateVertexShader          ();
            auto tessControlShader      = renderSystem->CreateTessControlShader     ();
            auto tessEvaluationShader   = renderSystem->CreateTessEvaluationShader  ();
            auto geometryShader         = renderSystem->CreateGeometryShader        ();
            auto pixelShader            = renderSystem->CreatePixelShader           ();

            LoadShaderFile( vertexShader.get(),         shaderFilenames.vertex         = filenames[0], entryPoints[0], targetVersion);
            LoadShaderFile( tessControlShader.get(),    shaderFilenames.tessControl    = filenames[1], entryPoints[1], targetVersion);
            LoadShaderFile( tessEvaluationShader.get(), shaderFilenames.tessEvaluation = filenames[2], entryPoints[2], targetVersion);
            LoadShaderFile( geometryShader.get(),       shaderFilenames.geometry       = filenames[3], entryPoints[3], targetVersion);
            LoadShaderFile( pixelShader.get(),          shaderFilenames.pixel          = filenames[4], entryPoints[4], targetVersion);

            shaderComposition->AttachShader(
                vertexShader, tessControlShader, tessEvaluationShader, geometryShader, pixelShader
            );
        }
        break;

        default:
            throw InvalidArgumentException(__FUNCTION__, "filenames", "Invalid number of filenames to detect type of shader composition");
    }

    /* Setup vertex format and compile the shader composition */
    shaderComposition->SetupVertexFormat(vertexFormat);
    //shaderComposition->Compile(flags);

    shaderCompositionFilesMap_[shaderComposition.get()] = shaderFilenames;

    return AddShaderComposition(shaderComposition);
}

ShaderCompositionPtr ShaderManager::CreateShaderCompositionFromFiles(
    const std::vector<std::string>& filenames,
    const VertexFormat& vertexFormat,
    Shader::Versions targetVersion)
{
    return CreateShaderCompositionFromFiles(
        filenames, DefaultEntryPoints(__FUNCTION__, filenames.size()),
        vertexFormat, targetVersion
    );
}

ShaderCompositionPtr ShaderManager::CreateShaderComposition(
    const std::vector<std::string>& sources,
    const std::vector<std::string>& entryPoints,
    const VertexFormat& vertexFormat,
    Shader::Versions targetVersion)
{
    ResolveTargetVersion(targetVersion);

    /* Get active render system */
    auto renderSystem = RenderSystem::Active(true);

    /* Create shader composition */
    auto shaderComposition = renderSystem->CreateShaderComposition();

    /* Create the shaders, and attach them to the shader composition */
    const auto num = sources.size();

    switch (num)
    {
        case 2:
        {
            auto vertexShader   = renderSystem->CreateVertexShader  ();
            auto pixelShader    = renderSystem->CreatePixelShader   ();

            CreateShader( vertexShader.get(), sources[0], entryPoints[0], targetVersion );
            CreateShader( pixelShader.get(),  sources[1], entryPoints[1], targetVersion );

            shaderComposition->AttachShader(vertexShader, pixelShader);
        }
        break;

        case 3:
        {
            auto vertexShader   = renderSystem->CreateVertexShader  ();
            auto geometryShader = renderSystem->CreateGeometryShader();
            auto pixelShader    = renderSystem->CreatePixelShader   ();

            CreateShader( vertexShader.get(),   sources[0], entryPoints[0], targetVersion );
            CreateShader( geometryShader.get(), sources[1], entryPoints[1], targetVersion );
            CreateShader( pixelShader.get(),    sources[2], entryPoints[2], targetVersion );

            shaderComposition->AttachShader(vertexShader, geometryShader, pixelShader);
        }
        break;

        case 4:
        {
            auto vertexShader           = renderSystem->CreateVertexShader          ();
            auto tessControlShader      = renderSystem->CreateTessControlShader     ();
            auto tessEvaluationShader   = renderSystem->CreateTessEvaluationShader  ();
            auto pixelShader            = renderSystem->CreatePixelShader           ();

            CreateShader( vertexShader.get(),         sources[0], entryPoints[0], targetVersion );
            CreateShader( tessControlShader.get(),    sources[1], entryPoints[1], targetVersion );
            CreateShader( tessEvaluationShader.get(), sources[2], entryPoints[2], targetVersion );
            CreateShader( pixelShader.get(),          sources[3], entryPoints[3], targetVersion );

            shaderComposition->AttachShader(
                vertexShader, tessControlShader, tessEvaluationShader, pixelShader
            );
        }
        break;

        case 5:
        {
            auto vertexShader           = renderSystem->CreateVertexShader          ();
            auto tessControlShader      = renderSystem->CreateTessControlShader     ();
            auto tessEvaluationShader   = renderSystem->CreateTessEvaluationShader  ();
            auto geometryShader         = renderSystem->CreateGeometryShader        ();
            auto pixelShader            = renderSystem->CreatePixelShader           ();

            CreateShader( vertexShader.get(),         sources[0], entryPoints[0], targetVersion );
            CreateShader( tessControlShader.get(),    sources[1], entryPoints[1], targetVersion );
            CreateShader( tessEvaluationShader.get(), sources[2], entryPoints[2], targetVersion );
            CreateShader( geometryShader.get(),       sources[3], entryPoints[3], targetVersion );
            CreateShader( pixelShader.get(),          sources[4], entryPoints[4], targetVersion );

            shaderComposition->AttachShader(
                vertexShader, tessControlShader, tessEvaluationShader, geometryShader, pixelShader
            );
        }
        break;

        default:
            throw InvalidArgumentException(__FUNCTION__, "sources", "Invalid number of sources to detect type of shader composition");
    }

    /* Setup vertex format and compile the shader composition */
    shaderComposition->SetupVertexFormat(vertexFormat);
    //shaderComposition->Compile(flags);

    return AddShaderComposition(shaderComposition);
}

ShaderCompositionPtr ShaderManager::CreateShaderComposition(
    const std::vector<std::string>& sources,
    const VertexFormat& vertexFormat,
    Shader::Versions targetVersion)
{
    return CreateShaderComposition(
        sources, DefaultEntryPoints(__FUNCTION__, sources.size()),
        vertexFormat, targetVersion
    );
}

ShaderCompositionPtr ShaderManager::LoadShaderComposition(
    const std::vector<std::string>& filenames,
    const std::vector<std::string>& entryPoints,
    const VertexFormat& vertexFormat,
    Shader::Versions targetVersion,
    const Shader::CompilationFlags::DataType flags)
{
    //!TODO! -> make use of pre-loaded shader compositions!
    auto shaderComposition = CreateShaderCompositionFromFiles(
        filenames, entryPoints, vertexFormat, targetVersion
    );
    
    if (!shaderComposition->Compile(flags))
        throw ShaderCompilationException(ExtractFileName(filenames[0]));

    return shaderComposition;
}

ShaderCompositionPtr ShaderManager::LoadShaderComposition(
    const std::vector<std::string>& filenames,
    const VertexFormat& vertexFormat,
    Shader::Versions targetVersion,
    const Shader::CompilationFlags::DataType flags)
{
    return LoadShaderComposition(
        filenames, DefaultEntryPoints(__FUNCTION__, filenames.size()),
        vertexFormat, targetVersion, flags
    );
}

void ShaderManager::ReleaseShaderComposition(const ShaderComposition* shaderComposition)
{
    /* Remove shader composition from hash map */
    //loadedShaderCompositions_.Remove(shaderComposition);

    /* Remove shader composition from list */
    for (auto it = shaderCompositions_.begin(); it != shaderCompositions_.end(); ++it)
    {
        if (it->get() == shaderComposition)
        {
            shaderCompositions_.erase(it);
            break;
        }
    }
}

void ShaderManager::ReleaseAllShaderCompositions()
{
    //loadedShaderCompositions_.hashMap.clear();
    shaderCompositions_.clear();
}

bool ShaderManager::ReloadShaderComposition(ShaderComposition* shaderComposition)
{
    /* Find shader filenames in map */
    auto it = shaderCompositionFilesMap_.find(shaderComposition);
    if (it == shaderCompositionFilesMap_.end())
        return false;

    /* Reload shader code from files */
    auto ReloadShaderCode = [](Shader* shader, const std::string& filename)
    {
        if (shader && shader->sourceCode && !filename.empty())
            shader->sourceCode->LoadFromFile(filename);
    };

    const auto& filenames = it->second;

    ReloadShaderCode( shaderComposition->GetVertexShader        ().get(), filenames.vertex         );
    ReloadShaderCode( shaderComposition->GetPixelShader         ().get(), filenames.pixel          );
    ReloadShaderCode( shaderComposition->GetGeometryShader      ().get(), filenames.geometry       );
    ReloadShaderCode( shaderComposition->GetTessControlShader   ().get(), filenames.tessControl    );
    ReloadShaderCode( shaderComposition->GetTessEvaluationShader().get(), filenames.tessEvaluation );
    ReloadShaderCode( shaderComposition->GetComputeShader       ().get(), filenames.compute        );

    /* Temporarily store and then detach all constant buffers */
    const auto constBuffers = shaderComposition->GetConstantBuffers();
    shaderComposition->DetachAllConstantBuffers();

    /* Recompile shader composition */
    if (!shaderComposition->Compile())
        return false;

    /* Re-attach all constant buffers */
    for (const auto& cbuffer : constBuffers)
        shaderComposition->Attach(cbuffer);

    return true;
}


/*
 * ======= Private: =======
 */

ShaderCompositionPtr ShaderManager::AddShaderComposition(const ShaderCompositionPtr& shaderComposition)
{
    shaderCompositions_.push_back(shaderComposition);
    return shaderComposition;
}

} // /namespace Video

} // /namespace Fork



// ========================