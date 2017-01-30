/*
 * Model file handler header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_MODEL_FILE_HANDLER_H__
#define __FORK_MODEL_FILE_HANDLER_H__


#include "Core/Export.h"
#include "Scene/Node/GeometryNode.h"
#include "Scene/FileHandler/ModelReader.h"

#include <string>


namespace Fork
{

namespace Scene
{

namespace ModelFileHandler
{


/**
Reads the specified model from file.
\see ModelReader::ReadModel
\see ModelReader::Flags
*/
FORK_EXPORT GeometryPtr ReadModel(
    const std::string& filename,
    const IO::PathDictionary& texPathDict = {},
    const ModelReader::Flags::DataType flags = 0
);

/**
Reads the specified animated model from file.
\see ModelReader::ReadAnimatedModel
\see ModelReader::Flags
*/
FORK_EXPORT ModelReader::AnimatedModel ReadAnimatedModel(
    const std::string& filename,
    const IO::PathDictionary& texPathDict = {},
    const ModelReader::Flags::DataType flags = 0
);


} // /namespace ModelFileHandler

} // /namespace Scene

} // /namespace Fork


#endif



// ========================