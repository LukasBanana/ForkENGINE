/*
 * Engine file reader file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "FileReader.h"
#include "Core/Exception/InvalidArgumentException.h"
#include "Engine/FileHandler/EngineFormatException.h"


namespace Fork
{

namespace Engine
{

namespace Format
{


FileReader::FileReader(IO::File& file) :
    FileHandler{ file }
{
}

/* --- Base reading --- */

void FileReader::ReadHeader(const std::string& magic, UInt16 version)
{
    /* Validate magic number */
    if (magic.size() != 4)
        throw InvalidArgumentException(__FUNCTION__, "magic", "Magic 'number' must have 4 characters");

    /* Read header */
    header_ = Read<FileHeader>();

    /* Validate header requirements */
    if (header_.magic != *reinterpret_cast<const UInt32*>(magic.c_str()))
        throw EngineFormatException(__FUNCTION__, "Invalid magic number");
    if (version < header_.compatibleVersion)
        throw EngineFormatException(__FUNCTION__, "Requested format version is too old");
}

UInt32 FileReader::ReadUInt32()
{
    return Read<UInt32>();
}

std::string FileReader::ReadString8()
{
    return ReadString<UInt8, char>();
}

std::wstring FileReader::ReadWString8()
{
    return ReadString<UInt8, wchar_t>();
}

std::string FileReader::ReadString16()
{
    return ReadString<UInt16, char>();
}

std::wstring FileReader::ReadWString16()
{
    return ReadString<UInt16, wchar_t>();
}

std::string FileReader::ReadString32()
{
    return ReadString<UInt32, char>();
}

std::wstring FileReader::ReadWString32()
{
    return ReadString<UInt32, wchar_t>();
}

/* --- Lists --- */

std::vector<UInt32> FileReader::ReadChunkDescList()
{
    std::vector<UInt32> chunkIDs;

    /* Read number of chunk descriptions */
    auto size = ReadUInt32();

    while (size-- > 0)
    {
        /* Read next chunk description */
        auto chunk = Read<ChunkDesc>();
        chunkDescMap_[chunk.id] = chunk;
        chunkIDs.push_back(chunk.id);
    }

    return chunkIDs;
}

const ChunkDesc* FileReader::LocateChunk(UInt32 id)
{
    /* Find chunk with specified ID */
    auto it = chunkDescMap_.find(id);
    if (it != chunkDescMap_.end())
    {
        /* Seek stream to the chunk position and return reference */
        StreamPos(static_cast<std::streampos>(it->second.offset));
        return &(it->second);
    }
    return nullptr;
}


/*
 * ======= Private: =======
 */

template <typename SizeType, typename CharType>
std::basic_string<CharType> FileReader::ReadString()
{
    std::basic_string<CharType> str;

    /* Read string length */
    auto len = static_cast<size_t>(Read<SizeType>());

    /* Read string characters */
    if (len > 0)
    {
        str.resize(len);
        GetFile().ReadBuffer(&str[0], len);
    }

    return str;
}


} // /namespace Format

} // /namespace Engine

} // /namespace Fork



// ========================