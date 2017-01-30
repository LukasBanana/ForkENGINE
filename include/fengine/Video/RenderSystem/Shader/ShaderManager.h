/*
 * Shader manager header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SHADER_MANAGER_H__
#define __FORK_SHADER_MANAGER_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Video/RenderSystem/Shader/ShaderComposition.h"
#include "Core/Container/SharedHashMap.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(ShaderManager);

/**
Shader manager class. This is a helper class to make loading and creating shader easier.
You can also load shaders without this class.
*/
class FORK_EXPORT ShaderManager
{
    
    public:
        
        /**
        Creates a shader composition, loads- and attaches
        the shaders from the specified files, and compiles the entire shader composition.
        \param[in] filenames Specifies the shader filenames. The following number of filenames and orders are allowed:
        \code
        { "vertex shader", "pixel shader" }
        { "vertex shader", "geometry shader", "pixel-shader" }
        { "vertex shader", "tessellation controll shader", "tessellation evaluation shader", "pixel-shader" }
        { "vertex shader", "tessellation controll shader", "tessellation evaluation shader", "geometry shader", "pixel-shader" }
        \endcode
        \param[in] entryPoints Specifies the shader entry points. The number of entry-points must be equal to the number of filenames.
        \param[in] vertexFormat Specifies the vertex input layout format.
        \param[in] targetVersion Specifies the shader target version. If this is 'Shader::Versions::\_\_Unspecified\_\_'
        the highest supported shader model for the respective render context is used. By default Shader::Versions::\_\_Unspecified\_\_.
        For GLSL this must be Shader::Versions::GLSL_... and for HLSL this must be Shader::Versions::HLSL_...
        \throws InvalidArgumentException If the number of filenames do no match any of the above exmaples.
        \throws InvalidArgumentException If the number of filenames are not equal to the number of entry-points.
        \see Shader::Versions
        \see Shader::CompilationFlags
        */
        ShaderCompositionPtr CreateShaderCompositionFromFiles(
            const std::vector<std::string>& filenames,
            const std::vector<std::string>& entryPoints,
            const VertexFormat& vertexFormat,
            Shader::Versions targetVersion = Shader::Versions::__Unspecified__
        );
        /**
        Creates a shader composition with default entry points.
        The default entry points are: { "VertexMain", "HullMain", "DomainMain", "Geometry", "PixelMain" }.
        \remarks For GLSL the entry points will be ignored.
        \see CreateShaderCompositionFromFiles(const std::vector<std::string>&, const std::vector<std::string>&, const VertexFormat&, Shader::Versions)
        */
        ShaderCompositionPtr CreateShaderCompositionFromFiles(
            const std::vector<std::string>& filenames,
            const VertexFormat& vertexFormat,
            Shader::Versions targetVersion = Shader::Versions::__Unspecified__
        );

        /**
        Creates a shader composition and attaches the shaders from the specified sources.
        You only need to compile the entire shader composition.
        \param[in] sources Specifies the shader sources. The following number of sources and orders are allowed:
        \code
        { "vertex shader", "pixel shader" }
        { "vertex shader", "geometry shader", "pixel-shader" }
        { "vertex shader", "tessellation controll shader", "tessellation evaluation shader", "pixel-shader" }
        { "vertex shader", "tessellation controll shader", "tessellation evaluation shader", "geometry shader", "pixel-shader" }
        \endcode
        \throws InvalidArgumentException If the number of sources do no match any of the above exmaples.
        \see CreateShaderCompositionFromFiles
        */
        ShaderCompositionPtr CreateShaderComposition(
            const std::vector<std::string>& sources,
            const std::vector<std::string>& entryPoints,
            const VertexFormat& vertexFormat,
            Shader::Versions targetVersion = Shader::Versions::__Unspecified__
        );
        /**
        Creates a shader composition with default entry points.
        The default entry points are: { "VertexMain", "HullMain", "DomainMain", "Geometry", "PixelMain" }.
        \remarks For GLSL the entry points will be ignored.
        \see CreateShaderComposition(const std::vector<std::string>&, const std::vector<std::string>&, const VertexFormat&, Shader::Versions)
        */
        ShaderCompositionPtr CreateShaderComposition(
            const std::vector<std::string>& sources,
            const VertexFormat& vertexFormat,
            Shader::Versions targetVersion = Shader::Versions::__Unspecified__
        );

        /**
        Loads and compiles the specified shader composition. If the shader composition
        was already loaded, this shader composition will be returned.
        \throws ShaderCompilationException If the shader compilation failed.
        \see CreateShaderCompositionFromFiles
        */
        ShaderCompositionPtr LoadShaderComposition(
            const std::vector<std::string>& filenames,
            const std::vector<std::string>& entryPoints,
            const VertexFormat& vertexFormat,
            Shader::Versions targetVersion = Shader::Versions::__Unspecified__,
            const Shader::CompilationFlags::DataType flags = 0
        );
        //! \see LoadShaderComposition(const std::vector<std::string>&, const std::vector<std::string>&, const VertexFormat&, Shader::Versions)
        ShaderCompositionPtr LoadShaderComposition(
            const std::vector<std::string>& filenames,
            const VertexFormat& vertexFormat,
            Shader::Versions targetVersion = Shader::Versions::__Unspecified__,
            const Shader::CompilationFlags::DataType flags = 0
        );

        void ReleaseShaderComposition(const ShaderComposition* shaderComposition);
        void ReleaseAllShaderCompositions();

        /**
        Reloads the specified shader composition. All filenames are stored inside this shader manager.
        \param[in] shaderComposition Raw pointer to the shader composition whose shaders are to be reloaded from file.
        \return True if the shader composition has been reloaded successful.
        Otherwise it has not been added to the list previously or the shader compilation failed.
        */
        bool ReloadShaderComposition(ShaderComposition* shaderComposition);

    private:
        
        typedef ShaderComposition::Container<std::string> ShaderFilenamesType;

        ShaderCompositionPtr AddShaderComposition(const ShaderCompositionPtr& shaderComposition);

        //SharedHashMap<std::string, ShaderComposition> loadedShaderCompositions_;
        std::vector<ShaderCompositionPtr> shaderCompositions_;

        std::map<const ShaderComposition*, ShaderFilenamesType> shaderCompositionFilesMap_;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================