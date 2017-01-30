/*
 * Shader code header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SHADER_CODE_H__
#define __FORK_SHADER_CODE_H__


#include "Core/Export.h"
#include "Video/RenderSystem/Shader/Shader.h"

#include <vector>
#include <string>


namespace Fork
{

namespace Video
{


//! Class to store shader source code.
class FORK_EXPORT ShaderCode
{
    
    public:
        
        /* === Enumerations === */

        //! Supported shader languages.
        enum class Languages
        {
            None, //!< No shading language is supported.
            HLSL, //!< HLSL (DirectX High Level Shading Language) is supported.
            GLSL, //!< GLSL (OpenGL Shading Language) is supported.
        };

        /* === Functions === */

        //! Returns the sources as one single string buffer.
        std::string ConvertToSingleBuffer() const;

        /**
        Loads the sources from the specified file and returns true on success.
        \see ShaderReader
        */
        bool LoadFromFile(const std::string& filename);
        /**
        Loads the sources from the specified file and translates the code
        from HLSL to GLSL if 'outputLanguage' is Languages::GLSL.
        */
        bool LoadFromFile(
            const std::string& filename, const std::string& entryPoint,
            const Shader::Types targetType, const Languages outputLanguage
        );

        //! Clears the sources.
        void Clear();

        /**
        Appends the specified code line to the sources (plus the '\\n' new-line character).
        This will be added at the end of the list (push_back).
        */
        void Append(const std::string& codeLine);

        /* === Members === */

        /**
        Shader sources. This is an array list of strings.
        Each string represents one line of the source code.
        */
        std::vector<std::string> sources;

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================