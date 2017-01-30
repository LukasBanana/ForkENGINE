/*
 * Model file handler file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Scene/FileHandler/ModelFileHandler.h"
#include "Scene/Geometry/GeometryConverter.h"
#include "CommonModelReader.h"
#include "IO/Core/Log.h"


namespace Fork
{

namespace Scene
{

namespace ModelFileHandler
{


static void FinalizeModel(GeometryPtr& model, const std::string& filename, const ModelReader::Flags::DataType flags)
{
    if (model)
    {
        /* Apply extended flags */
        if ((flags & ModelReader::Flags::OptimizeGeometryGraph) != 0)
            GeometryConverter::OptimizeGeometryGraph(model);

        /* Compute bounding volume and store filename in geometry meta data */
        model->ComputeBoundingVolume();
        model->metaData.name = filename;
    }
}

FORK_EXPORT GeometryPtr ReadModel(
    const std::string& filename, const IO::PathDictionary& texPathDict, const ModelReader::Flags::DataType flags)
{
    IO::Log::Message(ToStr("Load model: \"") + filename + ToStr("\""));
    IO::Log::ScopedIndent indent;

    /*
    Always read models with the common model reader,
    since Assimp supports all common model file formats.
    */
    CommonModelReader modelReader;
    auto model = modelReader.ReadModel(filename, texPathDict, flags);

    FinalizeModel(model, filename, flags);

    return model;
}

FORK_EXPORT ModelReader::AnimatedModel ReadAnimatedModel(
    const std::string& filename, const IO::PathDictionary& texPathDict, const ModelReader::Flags::DataType flags)
{
    IO::Log::Message(ToStr("Load animated model: \"") + filename + ToStr("\""));
    IO::Log::ScopedIndent indent;

    /*
    Always read models with the common model reader,
    since Assimp supports all common model file formats.
    */
    CommonModelReader modelReader;
    auto animModel = modelReader.ReadAnimatedModel(filename, texPathDict, flags);

    FinalizeModel(animModel.model, filename, flags);

    return animModel;
}


} // /namespace ModelFileHandler

} // /namespace Scene

} // /namespace Fork



// ========================