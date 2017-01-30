/*
 * File source file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "IO/FileSystem/File.h"


namespace Fork
{

namespace IO
{


File::~File()
{
}

std::string File::ReadStringSized()
{
    const auto len = Read<unsigned int>();

    std::string str;
    str.resize(len);

    ReadBuffer(&str[0], len);

    return str;
}

void File::WriteStringSized(const std::string& str)
{
    const auto len = static_cast<unsigned int>(str.size());
    Write(len);
    WriteBuffer(str.c_str(), len);
}

void File::WriteStringC(const std::string& str)
{
    WriteBuffer(str.c_str(), str.size() + 1);
}

void File::WriteStringNL(const std::string& str)
{
    WriteBuffer(str.c_str(), str.size());
    Write('\n');
}


} // /namespace IO

} // /namespace Fork



// ========================