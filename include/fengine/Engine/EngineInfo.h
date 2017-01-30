/*
 * Engine info header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_ENGINE_INFO_H__
#define __FORK_ENGINE_INFO_H__


#include "Core/Export.h"
#include "Video/RenderSystem/RenderContext.h"


namespace Fork
{

namespace Engine
{


/**
Prints the engine information using the log output.
\param[in] context Optional raw-pointer to the render context where the renderer information can be queried.
If this is a null pointer, no renderer information will be printed. By default null.
\see IO::Log
\see Video::RenderContext
*/
FORK_EXPORT void PrintEngineInfo(const Video::RenderContext* context = nullptr);


} // /namespace Engine

} // /namespace Fork


#endif



// ========================