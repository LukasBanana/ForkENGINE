/*
 * Engine file handler file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "FileHandler.h"
#include "Core/Exception/InvalidStateException.h"
#include "Core/StringModifier.h"


namespace Fork
{

namespace Engine
{

namespace Format
{


FileHandler::FileHandler(IO::File& file) :
    file_{ file }
{
}
FileHandler::~FileHandler()
{
}

std::streampos FileHandler::StreamPos() const
{
    return static_cast<std::streampos>(GetFile().Pos());
}

void FileHandler::StreamPos(std::streampos pos)
{
    GetFile().SeekPos(pos);
}

std::streampos FileHandler::PushPos(std::streampos pos)
{
    /* Push current stream position onto stack and seek to the specified position */
    streamPosStack_.push(StreamPos());
    StreamPos(pos);
    return streamPosStack_.top();
}

void FileHandler::PopPos()
{
    if (!streamPosStack_.empty())
    {
        /* Restore previous stream position and pop entry from stack */
        StreamPos(streamPosStack_.top());
        streamPosStack_.pop();
    }
    else
        throw InvalidStateException(__FUNCTION__, "Can not restore stream position from empty stack");
}

std::string FileHandler::VersionString() const
{
    auto ver = GetHeader().version;
    return ToStr(ver / 100) + "." + NumberOffset(ver, 99, '0');
}


} // /namespace Format

} // /namespace Engine

} // /namespace Fork



// ========================