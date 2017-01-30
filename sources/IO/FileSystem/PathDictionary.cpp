/*
 * Path dictionary file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "IO/FileSystem/PathDictionary.h"
#include "IO/FileSystem/FileStreamHelper.h"
#include "Core/StringModifier.h"
#include "Core/STLHelper.h"


namespace Fork
{

namespace IO
{


static bool IsPathCorrect(const std::string& path)
{
    return path.back() == '/' || path.back() == '\\';
}

void PathDictionary::AddSearchPath(const std::string& path)
{
    if (!path.empty())
    {
        if (!IsPathCorrect(path))
            searchPaths_.push_back(path + '/');
        else
            searchPaths_.push_back(path);
    }
}

void PathDictionary::RemoveSearchPath(const std::string& path)
{
    if (!path.empty())
    {
        if (!IsPathCorrect(path))
            RemoveFromList(searchPaths_, path + '/');
        else
            RemoveFromList(searchPaths_, path);
    }
}

bool PathDictionary::FindFile(std::string& filename) const
{
    /* Check in all search paths */
    for (const auto& path : searchPaths_)
    {
        if (FileExists(path + filename))
        {
            filename = path + filename;
            return true;
        }
    }

    /* Then check with extracted filename in all search paths */
    if (filename.find_first_of("/\\") != std::string::npos)
    {
        auto name = ExtractFileName(filename);

        for (const auto& path : searchPaths_)
        {
            if (FileExists(path + name))
            {
                filename = path + name;
                return true;
            }
        }
    }

    /* At last check filename only */
    return FileExists(filename);
}


} // /namespace IO

} // /namespace Fork



// ========================