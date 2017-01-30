/*
 * Model reader file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/FileHandler/ModelReader.h"


namespace Fork
{

namespace Scene
{


ModelReader::~ModelReader()
{
}

ModelReader::AnimatedModel ModelReader::ReadAnimatedModel(
    const std::string& filename, const IO::PathDictionary& texPathDict, const Flags::DataType flags)
{
    // default implementation
    return { ReadModel(filename, texPathDict, flags)/*, nullptr*/ };
}


} // /namespace Scene

} // /namespace Fork



// ========================