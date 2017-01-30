/*
 * Engine asset file evaluator file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "Engine/FileHandler/AssetFileEvaluator.h"
#include "Core/StringModifier.h"

#include <initializer_list>
#include <algorithm>


namespace Fork
{

namespace Engine
{

namespace AssetFileEvaluator
{


static bool MatchFileExt(const std::string& ext, const std::initializer_list<std::string>& extList)
{
    return std::find(extList.begin(), extList.end(), ext) != extList.end();
}

//!TODO! -> read file magic number to determine file type!!!
FORK_EXPORT Asset::Types EvaluateFile(const std::string& filename)
{
    /* Extract file extension from filename */
    auto fileExt = ExtractFileExt(filename);
    MakeLower(fileExt);

    /* Evaluate asset file type */
    if (MatchFileExt(fileExt, { "jpg", "jpeg", "jpe", "jfif", "png", "bmp", "dib", "tga", "tpic" }))
        return Asset::Types::Texture;
    else if (MatchFileExt(fileExt, { "3ds", "x", "ms3d", "b3d", "blend", "obj", "dae", "mdl", "md2", "md3", "pk3", "md5mesh", "smd", "vta", "3d", "lwo", "lws", "lxo", "ply", "irrmesh", "irr", "xml" }))
        return Asset::Types::Geometry;
    else if (MatchFileExt(fileExt, { "wav", "ogg" }))
        return Asset::Types::Sound;

    /* Return with unknown asset file type */
    return Asset::Types::__Unknown__;
}


} // /namespace AssetFileEvaluator

} // /namespace Engine

} // /namespace Fork



// ========================