/*
 * Engine asset file evaluator header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_ENGINE_ASSET_FILE_EVALUATOR_H__
#define __FORK_ENGINE_ASSET_FILE_EVALUATOR_H__


#include "Core/Export.h"
#include "Engine/Asset/Asset.h"

#include <string>


namespace Fork
{

namespace Engine
{

namespace AssetFileEvaluator
{


/**
Evaluates the specified asset file and returns the asset type.
\return The evaluated asset file type or Asset::Types::__Unknwon__ if the file could not be evaluated.
\see Asset::Types
\todo Currently only evaluates the filename extension. Change this to reading the file's magic number to determine the file type!
*/
FORK_EXPORT Asset::Types EvaluateFile(const std::string& filename);


} // /namespace AssetFileEvaluator

} // /namespace Engine

} // /namespace Fork


#endif



// ========================