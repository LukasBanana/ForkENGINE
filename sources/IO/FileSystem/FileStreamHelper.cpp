/*
 * File stream helper functions file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "IO/FileSystem/FileStreamHelper.h"


namespace Fork
{

namespace IO
{


FORK_EXPORT std::streampos QueryFileSize(std::ifstream& stream)
{
    /* Store current file position */
    const auto prevPos = stream.tellg();

    /* Get start position */
    stream.seekg(0, std::ifstream::beg);
    const auto startPos = stream.tellg();

    /* Get end position */
    stream.seekg(0, std::ifstream::end);
    const auto endPos = stream.tellg();

    /* Restore previous position */
    stream.seekg(prevPos, std::ios_base::beg);

    return endPos - startPos;
}

FORK_EXPORT std::string ReadFileIntoString(std::ifstream& stream)
{
    std::string str;

    stream.seekg(0, std::ifstream::end);
    str.reserve(static_cast<size_t>(stream.tellg()));
    stream.seekg(0, std::ifstream::beg);

    str.assign(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());

    return str;
}

FORK_EXPORT bool FileExists(const std::string& filename)
{
    std::ifstream file(filename);
    return file.good();
}

FORK_EXPORT bool FileExists(const std::wstring& filename)
{
    std::ifstream file(filename);
    return file.good();
}


} // /namespace IO

} // /namespace Fork



// ========================