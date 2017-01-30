/*
 * Geometry shader header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_GEOMETRY_SHADER_H__
#define __FORK_GEOMETRY_SHADER_H__


#include "Video/RenderSystem/Shader/Shader.h"


namespace Fork
{

namespace Video
{


DECL_SHR_PTR(GeometryShader);

//! Geometry shader interface.
class FORK_EXPORT GeometryShader : public Shader
{
    
    public:
        
        virtual ~GeometryShader()
        {
        }

        GeometryShader(const GeometryShader&) = delete;
        GeometryShader& operator = (const GeometryShader&) = delete;

        Types Type() const override
        {
            return Types::Geometry;
        }

    protected:
        
        GeometryShader()
        {
        }

};


} // /namespace Video

} // /namespace Fork


#endif



// ========================