/*
 * Shader provider header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SHADER_PROVIDER_H__
#define __FORK_SHADER_PROVIDER_H__


#include "Core/Export.h"
#include "Video/RenderSystem/RenderSystem.h"

#include <string>


namespace Fork
{

namespace Video
{


/**
Standard shader provider interface.
This interface provides all renderer dependent standard shaders.
*/
class FORK_EXPORT ShaderProvider
{

    public:

        //! Enumeration of all standard shaders this interface provides.
        enum class StandardShaders
        {
            /* --- Common drawing shaders --- */
            FontDrawing = 0,    //!< Font drawing shader (used by Video::PrimitiveRenderer).
            PrimitiveDrawing,   //!< Primitive drawing shader (used by Video::PrimitiveRenderer).
            BezierDrawing,      //!< Bezier curve drawing shader (used by Video::PrimitiveRenderer).
            ImageArrayDrawing,  //!< Image array drawing shader (used by Video::PrimitiveRenderer).
            UtilCommonModel,    //!< Utility common model rendering shader (used by Utility::ProjectorView, Utility::SelectorView).
            BoundingBox,        //!< Bounding box rendering shader (used by Scene::BoundingBoxSceneRenderer).
            SimpleColored,      //!< Simple colored rendering shader. Draws a simple color stored in each vertex.
            SolidColored,       //!< Solid colored rendering shader. Draws a constant color for all pixels.
            Simple3DMesh,       //!< Simple3DMesh rendering shader (used by Scene::SimpleSceneRenderer).
            TangentSpaceMesh,   //!< TangentSpace rendering shader (used by Scene::SimpleSceneRenderer).

            /* --- Post processing shaders --- */
            GaussianBlur,       //!< Gaussian blur post-processing shader (used by Video::GaussianBlur).
            SSCTHorzBlur,       //!< SSCT horizontal blur shader (used by Video::SSCT).
            SSCTVertBlur,       //!< SSCT vertical blur shader (used by Video::SSCT).
            SSCTHiZMap,         //!< SSCT Hi-Z map generration shader (used by Video::SSCT).
            SSCTVisibilityMap,  //!< SSCT visibility map generation shader (used by Video::SSCT).
            SSCTReflection,     //!< SSCT reflection shader (used by Video::SSCT).
            SSLRRayTrace,       //!< SSLR ray-tracing shader (used by Video::SSLR).
            //SSLRHorzBlur,       //!< SSLR horizontal blur shader (used by Video::SSLR).
            //SSLRVertBlur,       //!< SSLR vertical blur shader (used by Video::SSLR).
            //SSLRReflection,     //!< SSLR reflection shader (used by Video::SSLR).

            /* --- Advanced rendering shaders --- */
            CustomMIPMaps,      //!< Custom MIP-map generation shader (used by Video::CustomMIPMapsGenerator).
            HiZMIPMaps,         //!< Hi-Z MIP-map generation shader (used by Video::CustomMIPMapsGenerator).

            __Num__             //!< Number of standard shaders. Don't use this as index value!
        };

        virtual ~ShaderProvider();

        ShaderProvider(const ShaderProvider&) = delete;
        ShaderProvider& operator = (const ShaderProvider&) = delete;

        /**
        Creates a shader composition for the specified standard engine shader.
        \param[in] renderSystem Raw-pointer to the render system.
        \param[in] renderContext Raw-pointer to the render context.
        \param[in] shaderName Specifies the shader name of the standard engine shader.
        The shader path must be set with "Config::SetEngineShaderPath".
        \param[in] vertexFormat Specifies the vertex format for the vertex shader.
        \throws NullPointerException If 'renderSystem' or 'renderContext' is null.
        \throws ShaderCompilationException If the shader compilation failed.
        \see RenderSystem::CreateShaderComposition
        \see Config::SetEngineShaderPath
        */
        ShaderCompositionPtr CreateShaderCompositionFromFiles(
            RenderSystem* renderSystem, RenderContext* renderContext,
            const std::string& shaderName, const VertexFormat& vertexFormat
        );

        /**
        Comfort function for the other version of this function.
        This will use the active render- system and context.
        \see CreateShaderCompositionFromFiles(RenderSystem*, RenderContext*, const std::string&, const VertexFormat&)
        \see RenderSystem::Active
        \see RenderContext::Active
        */
        ShaderCompositionPtr CreateShaderCompositionFromFiles(
            const std::string& shaderName, const VertexFormat& vertexFormat
        );

        /**
        Creates a shader composition for the specified standard engine shader.
        These shaders are compiled into the engine library, thus they are not loaded from files.
        \param[in] renderSystem Raw-pointer to the render system.
        \param[in] renderContext Raw-pointer to the render context.
        \param[in] shaderName Specifies the shader name.
        \param[in] vertexFormat Specifies the vertex format for the vertex shader.
        \throws NullPointerException If 'renderSystem' or 'renderContext' is null.
        \throws IndexOutOfBoundsException If 'shaderName' is out of its legal range (see StandardShaders).
        \throws InvalidArgumentException If the specified standard shader is not supported by the active render context.
        \throws ShaderCompilationException If the shader compilation failed.
        \see RenderSystem::CreateShaderComposition
        \see StandardShaders
        */
        ShaderCompositionPtr CreateShaderCompositionFromResource(
            RenderSystem* renderSystem, RenderContext* renderContext,
            const StandardShaders shaderName, const VertexFormat& vertexFormat
        );

        /**
        Comfort function for the other version of this function.
        This will use the active render- system and context.
        \see CreateShaderCompositionFromResource(RenderSystem*, RenderContext*, const StandardShaders, const VertexFormat&)
        \see RenderSystem::Active
        \see RenderContext::Active
        */
        ShaderCompositionPtr CreateShaderCompositionFromResource(
            const StandardShaders shaderName, const VertexFormat& vertexFormat
        );

        //! Returns the path for the standard shader files. By default "shaders/".
        static std::string& ShaderPath();

        //! Converts the specified standard shader name to string.
        static std::string StandardShaderToString(const StandardShaders shaderName);

    protected:
        
        ShaderProvider() = default;

        //! Returns a null terminated string of the specified vertex shader code.
        virtual const char* QueryVertexShaderFromResource(const StandardShaders shaderName) const = 0;
        //! Returns a null terminated string of the specified pixel shader code.
        virtual const char* QueryPixelShaderFromResource(const StandardShaders shaderName) const = 0;

        /**
        Returns the index to the specified shader resource name.
        \throws IndexOutOfBoundsException If 'sahderName' is greater than or equal to StandardShaders::__Num__.
        \see StandardShaders
        */
        size_t ShaderNameToIndex(const StandardShaders shaderName) const;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================