/*
 * Shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SHADER_H__
#define __FORK_SHADER_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"
#include "Video/RenderSystem/HardwareBuffer/ConstantBuffer.h"

#include <vector>
#include <memory>
#include <map>


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(ShaderCode);
DECL_SHR_PTR(Shader);

//! Shader base interface.
class FORK_EXPORT Shader
{
    
    public:
        
        //! Shader types enumeration.
        enum class Types
        {
            Vertex,         //!< Vertex shader.
            Pixel,          //!< Pixel shader (also called "fragment shader").
            Geometry,       //!< Geometry shader.
            TessControl,    //!< Tessellation control shader (also called "hull shader").
            TessEvaluation, //!< Tessellation evaluation shader (also called "domain shader").
            Compute         //!< Compute shader.
        };

        //! Supported shader models.
        enum class Versions
        {
            __Unspecified__,    //!< Unspecified shader model.

            HLSL_4_0,           //!< HLSL version 4.0 for Direct3D 10.0.
            HLSL_4_1,           //!< HLSL version 4.1 for Direct3D 10.1.
            HLSL_5_0,           //!< HLSL version 5.0 for Direct3D 11.0.

            GLSL_110,           //!< GLSL version 1.10.59 for OpenGL 2.0.
            GLSL_120,           //!< GLSL version 1.20.8 for OpenGL 2.1.
            GLSL_130,           //!< GLSL version 1.30.10 for OpenGL 3.0.
            GLSL_140,           //!< GLSL version 1.40.08 for OpenGL 3.1.
            GLSL_150,           //!< GLSL version 1.50.11 for OpenGL 3.2.
            GLSL_330,           //!< GLSL version 3.30.6 for OpenGL 3.3.
            GLSL_400,           //!< GLSL version 4.00.9 for OpenGL 4.0.
            GLSL_410,           //!< GLSL version 4.10.6 for OpenGL 4.1.
            GLSL_420,           //!< GLSL version 4.20.11 for OpenGL 4.2.
            GLSL_430,           //!< GLSL version 4.30.8 for OpenGL 4.3.
            GLSL_440,           //!< GLSL version 4.40 for OpenGL 4.4.
        };

        /**
        Shader compilation flags.
        \see ShaderComposition::Compile
        \remarks This is only used in the ShaderComposition class,
        however it is part of this class anyway to keep the namespace
        short and simple "Shader::CompilationFlags" instead of "ShaderComposition::CompilationFlags".
        \note Compilation options are only supported for D3D11 render system!
        */
        struct CompilationFlags
        {
            typedef unsigned int DataType;
            enum : DataType
            {
                SkipOptimization        = (1 <<  0), //!< Skip optimization process (command line argument: "-skip-opt").
                SkipValidation          = (1 <<  1), //!< Skip validation against known capabilities and constraints (command line argument: "-skip-val").
                DebugInfo               = (1 <<  2), //!< Inserts debug info (command line argument: "-debug").
                WarningsAreErrors       = (1 <<  3), //!< Warnings are treated as errors (command line argument: "-Werror").
                PreferFlowControl       = (1 <<  4), //!< Prefers flow control.
                AvoidFlowControl        = (1 <<  5), //!< Avoids flow control.
                PackMatrixRowMajor      = (1 <<  6), //!< Matrices will be packed in row-major (unless explicitly specified).
                PackMatrixColumnMajor   = (1 <<  7), //!< Matrices will be packed in column-major (unless explicitly specified). This is generally more efficient.
                O0                      = (1 <<  8), //!< Optimization level 0, which means no optimization (command line argument: "-O0").
                O1                      = (1 <<  9), //!< Optimization level 1 (command line argument: "-O1").
                O2                      = (1 << 10), //!< Optimization level 2 (command line argument: "-O2").
                O3                      = (1 << 11), //!< Optimization level 3 (command line argument: "-O3").

                /**
                Posts the 'disassembly output' event to the event handler of the respective shader compisition (command line argument: "-dis-asm").
                \see ShaderComposition::EventHandler::OnDisassemblyOutput
                */
                DisassemblyOutput       = (1 << 12),
            };
        };

        virtual ~Shader();

        Shader(const Shader&) = delete;
        Shader& operator = (const Shader&) = delete;

        //! Returns the shader type.
        virtual Types Type() const = 0;

        //! Returns true if the shader version refers to GLSL.
        static bool IsGLSL(const Versions version);
        //! Returns true if the shader version refers to HLSL.
        static bool IsHLSL(const Versions version);

        //! Returns the map with all shader compilation flags [key = argument | value = flag].
        static std::map<std::string, CompilationFlags::DataType> CompilationFlagsMaps();

        /**
        Shader source code data field. This is a shared point, so that several shaders can shared its code.
        This is particularly useful for HLSL, where all shaders, for a shader composition, can be written into a single file.
        This shared pointer is initialized by default.
        \see ShaderCode
        */
        ShaderCodePtr sourceCode;

        /**
        Shader entry point. This specifies the name of the main function.
        For GLSL this field will be ignored, since the main function is always called "main".
        For HLSL this should be the respective shader's main function name
        (common names are "VS", "VMain", "VSMain", "VertexMain" etc.).
        */
        std::string entryPoint;

        /**
        Shader code target version (or rather shader model). By default Versions::__Unspecified__.
        For GLSL this field will be ignored. For HLSL this should be either
        Versions::HLSL_4_0, Versions::HLSL_4_1 or Versions::HLSL_5_0.
        \see Versions
        */
        Versions targetVersion = Versions::__Unspecified__;

    protected:
        
        Shader();

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================