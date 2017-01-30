/*
 * Shader compilation exception header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_SHADER_COMPILATION_EXCEPTION_H__
#define __FORK_SHADER_COMPILATION_EXCEPTION_H__


#include "Core/Exception/DefaultException.h"


namespace Fork
{


/**
Shader compilation exception.
\ingroup fork_exception
*/
class FORK_EXPORT ShaderCompilationException : public DefaultException
{
    
    public:
        
        ShaderCompilationException(const std::string& shaderName) :
            DefaultException
            {
                "ShaderCompilationException",
                "Shader: " + shaderName
            }
        {
        }

};


} // /namespace Fork


#endif



// ========================