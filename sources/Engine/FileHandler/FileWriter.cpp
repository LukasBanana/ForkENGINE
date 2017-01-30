/*
 * Engine file writer file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "FileWriter.h"
#include "Core/Exception/InvalidStateException.h"


namespace Fork
{

namespace Engine
{

namespace Format
{


FileWriter::FileWriter(IO::File& file) :
    FileHandler{ file }
{
}

/* --- Stacks --- */

void FileWriter::ReserveOffset()
{
    /* Push current file position onto stack and write placeholder */
    offsetStack_.push(StreamPos());
    WriteUInt32(0);
}

void FileWriter::ResolveOffset()
{
    if (!offsetStack_.empty())
    {
        /* Overwrite placeholder */
        auto currentPos = PushPos(offsetStack_.top());
        {
            offsetStack_.pop();
            WriteUInt32(static_cast<UInt32>(currentPos));
        }
        PopPos();
    }
    else
        throw InvalidStateException(__FUNCTION__, "Can not resolve offset with empty stack");
}

void FileWriter::ReserveChunkDesc(UInt32 id)
{
    /* Check if ID is already occupied */
    if (chunkDescMap_.find(id) != chunkDescMap_.end())
        throw InvalidArgumentException(__FUNCTION__, "id", "ID for chunk description is already occupied");

    /* Store stream position in hash-map */
    StreamChunkDesc chunk;
    {
        chunk.reservedPos   = StreamPos();
        chunk.chunkDesc.id  = id;
    }
    chunkDescMap_[id] = chunk;

    /* Reserve memory in file for chunk description */
    Write(ChunkDesc());
}

void FileWriter::BeginChunk(UInt32 id)
{
    auto chunk = FindChunkDesc(id);
    chunk->chunkDesc.offset = static_cast<UInt32>(StreamPos());
}

void FileWriter::EndChunk(UInt32 id)
{
    auto chunk = FindChunkDesc(id);
    chunk->chunkDesc.size = (static_cast<UInt32>(StreamPos()) - chunk->chunkDesc.offset);

    PushPos(chunk->reservedPos);
    {
        Write(chunk->chunkDesc);
    }
    PopPos();
}

/* --- Lists --- */

void FileWriter::WriteChunkDescList(const std::vector<UInt32>& idList)
{
    WriteUInt32(ListSize(idList));
    for (auto id : idList)
        ReserveChunkDesc(id);
}

/* --- Write functions --- */

void FileWriter::WriteHeader(const std::string& magic, UInt16 version, UInt16 compatibleVersion, UInt32 flags)
{
    /* Validate magic number */
    if (magic.size() != 4)
        throw InvalidArgumentException(__FUNCTION__, "magic", "Magic 'number' must have 4 characters");

    /* Setup header structure */
    header_.magic               = *reinterpret_cast<const UInt32*>(magic.c_str());
    header_.version             = version;
    header_.compatibleVersion   = compatibleVersion;
    header_.flags               = flags;

    /* Write header */
    Write(header_);
}

void FileWriter::WriteUInt32(UInt32 value)
{
    Write(value);
}

void FileWriter::WriteString8(const std::string& str)
{
    WriteString<UInt8>(str);
}

void FileWriter::WriteWString8(const std::wstring& str)
{
    WriteString<UInt8>(str);
}

void FileWriter::WriteString16(const std::string& str)
{
    WriteString<UInt16>(str);
}

void FileWriter::WriteWString16(const std::wstring& str)
{
    WriteString<UInt16>(str);
}

void FileWriter::WriteString32(const std::string& str)
{
    WriteString<UInt32>(str);
}

void FileWriter::WriteWString32(const std::wstring& str)
{
    WriteString<UInt32>(str);
}


/*
 * ======= Private: =======
 */

FileWriter::StreamChunkDesc* FileWriter::FindChunkDesc(UInt32 id)
{
    auto it = chunkDescMap_.find(id);
    if (it == chunkDescMap_.end())
        throw InvalidArgumentException(__FUNCTION__, "id", "ID for chunk description has not been reserved");
    return &(it->second);
}

template <typename SizeType, typename CharType>
void FileWriter::WriteString(const std::basic_string<CharType>& str)
{
    /* Validate string size */
    auto len = str.size();
    auto maxLen = std::numeric_limits<SizeType>::max();

    if (len > maxLen)
    {
        throw InvalidArgumentException(
            __FUNCTION__, "str",
            "String \"" + ToStr(str.substr(0, 10)) + "\"... is too long, maximal size is " + ToStr(maxLen)
        );
    }

    /* Write string length */
    Write<SizeType>(static_cast<SizeType>(len));

    /* Write string characters */
    GetFile().WriteBuffer(str.data(), len);
}


} // /namespace Format

} // /namespace Engine

} // /namespace Fork



// ========================