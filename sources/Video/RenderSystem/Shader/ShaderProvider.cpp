/*
 * Shader provider file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Video/RenderSystem/Shader/ShaderProvider.h"
#include "Core/Exception/NullPointerException.h"
#include "Core/Exception/IndexOutOfBoundsException.h"
#include "Video/RenderSystem/RenderContextException.h"
#include "Video/RenderSystem/Shader/ShaderCompilationException.h"


namespace Fork
{

namespace Video
{


ShaderProvider::~ShaderProvider()
{
}

ShaderCompositionPtr ShaderProvider::CreateShaderCompositionFromFiles(
    RenderSystem* renderSystem, RenderContext* renderContext,
    const std::string& shaderName, const VertexFormat& vertexFormat)
{
    ASSERT_POINTER(renderSystem);
    ASSERT_POINTER(renderContext);

    /* Setup shader filenames */
    auto ShaderFilename = [&](const std::string& shaderTypeName) -> std::string
    {
        return ShaderPath() + shaderName + shaderTypeName + "Shader." + renderContext->ShadingLanguageFileExt();
    };

    auto vertexShaderFilename = ShaderFilename("Vertex");
    auto pixelShaderFilename = ShaderFilename("Pixel");

    /* Create shader composition from files with common shader manager */
    auto shaderComposition = renderSystem->GetShaderManager()->CreateShaderCompositionFromFiles(
        { vertexShaderFilename, pixelShaderFilename },
        vertexFormat
    );

    /* Compile shader composition */
    if (!shaderComposition->Compile())
        throw ShaderCompilationException(shaderName);

    return shaderComposition;
}

ShaderCompositionPtr ShaderProvider::CreateShaderCompositionFromFiles(
    const std::string& shaderName, const VertexFormat& vertexFormat)
{
    return CreateShaderCompositionFromFiles(
        RenderSystem::Active(),
        RenderSystem::Active()->GetSharedRenderContext(),
        shaderName,
        vertexFormat
    );
}

ShaderCompositionPtr ShaderProvider::CreateShaderCompositionFromResource(
    RenderSystem* renderSystem, RenderContext* renderContext,
    const StandardShaders shaderName, const VertexFormat& vertexFormat)
{
    ASSERT_POINTER(renderSystem);
    ASSERT_POINTER(renderContext);

    /* Choose shader source code */
    const char* vertexShaderSources = QueryVertexShaderFromResource(shaderName);
    const char* pixelShaderSources = QueryPixelShaderFromResource(shaderName);

    /* Check for undefined shaders */
    if (!vertexShaderSources || !pixelShaderSources)
        throw InvalidArgumentException(__FUNCTION__, "shaderName", "Active render system does not support the specified standard shader");

    /* Create shader composition from files with common shader manager */
    auto shaderComposition = renderSystem->GetShaderManager()->CreateShaderComposition(
        { std::string(vertexShaderSources), std::string(pixelShaderSources) },
        vertexFormat
    );

    /* Compile shader composition */
    if (!shaderComposition->Compile())
        throw ShaderCompilationException(ShaderProvider::StandardShaderToString(shaderName));

    return shaderComposition;
}

ShaderCompositionPtr ShaderProvider::CreateShaderCompositionFromResource(
    const StandardShaders shaderName, const VertexFormat& vertexFormat)
{
    return CreateShaderCompositionFromResource(
        RenderSystem::Active(),
        RenderSystem::Active()->GetSharedRenderContext(),
        shaderName,
        vertexFormat
    );
}

std::string& ShaderProvider::ShaderPath()
{
    static std::string engineShaderPath = "shaders/";
    return engineShaderPath;
}

std::string ShaderProvider::StandardShaderToString(const StandardShaders shaderName)
{
    #define CASE_ENUM_NAME(name) case StandardShaders::name: return #name

    switch (shaderName)
    {
        CASE_ENUM_NAME( FontDrawing       );
        CASE_ENUM_NAME( PrimitiveDrawing  );
        CASE_ENUM_NAME( BezierDrawing     );
        CASE_ENUM_NAME( ImageArrayDrawing );
        CASE_ENUM_NAME( UtilCommonModel   );
        CASE_ENUM_NAME( BoundingBox       );
        CASE_ENUM_NAME( SimpleColored     );
        CASE_ENUM_NAME( SolidColored      );
        CASE_ENUM_NAME( Simple3DMesh      );

        CASE_ENUM_NAME( GaussianBlur      );
        CASE_ENUM_NAME( SSCTHorzBlur      );
        CASE_ENUM_NAME( SSCTVertBlur      );
        CASE_ENUM_NAME( SSCTHiZMap        );
        CASE_ENUM_NAME( SSCTVisibilityMap );
        CASE_ENUM_NAME( SSCTReflection    );

        CASE_ENUM_NAME( CustomMIPMaps     );
        CASE_ENUM_NAME( HiZMIPMaps        );
    }
    return "";

    #undef CASE_ENUM_NAME
}


/*
 * ======= Protected: =======
 */

size_t ShaderProvider::ShaderNameToIndex(const StandardShaders shaderName) const
{
    const auto shaderIndex = static_cast<size_t>(shaderName);

    if (shaderName >= StandardShaders::__Num__)
        throw IndexOutOfBoundsException(__FUNCTION__, shaderIndex);

    return shaderIndex;
}


} // /namespace Video

} // /namespace Fork



// ========================