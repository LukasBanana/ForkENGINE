/*
 * OpenGL query file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "GLQuery.h"
#include "GLExtensions.h"
#include "GLParamMapper.h"
#include "GLCore.h"


namespace Fork
{

namespace Video
{


GLQuery::GLQuery(const Types type) :
    Query   { type                     },
    target_ { GLParamMapper::Map(type) }
{
    glGenQueries(1, &objectID_);
    ENSURE_GL_SHARED_CONTEXT_FOR_BUFFER("glGenQueries", objectID_);
}
GLQuery::~GLQuery()
{
    ENSURE_GL_SHARED_CONTEXT_FOR_BUFFER("glDeleteQueries", objectID_);
    glDeleteQueries(1, &objectID_);
}


} // /namespace Video

} // /namespace Fork



// ========================