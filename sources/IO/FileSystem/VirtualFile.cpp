/*
 * Virtual file source file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "IO/FileSystem/VirtualFile.h"
#include "IO/Core/Log.h"
#include "Core/Exception/NullPointerException.h"

#include <algorithm>


namespace Fork
{

namespace IO
{


VirtualFile::VirtualFile(const std::string& filename, const OpenFlags::DataType flags)
{
    Open(filename, flags);
}

bool VirtualFile::Open(const std::string& filename, const OpenFlags::DataType flags)
{
    /* Validate parameters */
    if (filename.empty())
    {
        IO::Log::Error("Filenames must not be empty");
        return false;
    }

    if ((flags & OpenFlags::ReadWrite) == 0)
    {
        IO::Log::Error("Invalid open flags for file");
        return false;
    }

    /* Store new settings */
    filename_ = filename;
    flags_ = flags;

    return true;
}

void VirtualFile::Close()
{
    // nothing to do here
}

bool VirtualFile::IsOpen() const
{
    return true;
}

size_t VirtualFile::Pos()
{
    return filePos_;
}

void VirtualFile::SeekPos(std::streampos pos)
{
    filePos_ = static_cast<FilePosType>(pos);
    ClampFilePos();
}

void VirtualFile::SeekPos(std::streamoff offset, const SeekDirections direction)
{
    /* Seek file position */
    switch (direction)
    {
        case SeekDirections::Begin:
            filePos_ = static_cast<FilePosType>(offset);
            break;

        case SeekDirections::Current:
            if (offset < 0 && -offset > filePos_)
            {
                /* Clamp lower bound */
                filePos_ = 0;
            }
            else if (offset > 0 && (filePos_ + offset) < filePos_)
            {
                /* Clamp upper bound */
                filePos_ = buffer_.size();
            }
            else
            {
                /* Add offset */
                filePos_ += static_cast<int>(offset);
            }
            break;

        case SeekDirections::End:
            filePos_ = (offset < buffer_.size() ? buffer_.size() - static_cast<FilePosType>(offset) - 1 : 0);
            break;
    }

    ClampFilePos();
}

bool VirtualFile::IsEOF() const
{
    return buffer_.empty() || filePos_ >= buffer_.size();
}

void VirtualFile::ReadBuffer(void* buffer, size_t size)
{
    ASSERT_POINTER(buffer);

    if (HasReadAccess() && !IsEOF())
    {
        /* Get byte-aligned buffer and size of right-hand-side */
        auto bufferByteAligned = reinterpret_cast<char*>(buffer);
        size = std::min(size, SizeRHS());

        /* Read data from file */
        std::copy(BufferPtr(), BufferPtr() + size, bufferByteAligned);

        /* Seek file pos */
        filePos_ += size;
    }
}

void VirtualFile::WriteBuffer(const void* buffer, size_t size)
{
    ASSERT_POINTER(buffer);

    if (HasWriteAccess())
    {
        /* Get byte-aligned buffer and size of right-hand-side */
        auto bufferByteAligned = reinterpret_cast<const char*>(buffer);

        /* Increase buffer size if required */
        if (filePos_ + size >= buffer_.size())
            buffer_.resize(filePos_ + size);

        /* Read data from file */
        std::copy(bufferByteAligned, bufferByteAligned + size, BufferPtr());

        /* Seek file pos */
        filePos_ += size;
    }
}

std::string VirtualFile::ReadStringC()
{
    std::string str;
    ReadString(str, '\0');
    return str;
}

std::string VirtualFile::ReadStringNL()
{
    std::string str;
    ReadString(str);
    return str;
}

PhysicalFilePtr VirtualFile::WriteToHDD()
{
    /* Open output file */
    auto outFile = std::make_shared<PhysicalFile>();
    if (!outFile->Open(filename_, OpenFlags::Write))
        return nullptr;

    /* Write file */
    outFile->WriteBuffer(buffer_.data(), buffer_.size());

    return outFile;
}


/*
 * ======= Private: =======
 */

void VirtualFile::ClampFilePos()
{
    /* Clamp to buffer size */
    if (filePos_ > buffer_.size())
        filePos_ = buffer_.size();
}

char* VirtualFile::BufferPtr()
{
    return &buffer_[filePos_];
}

const char* VirtualFile::BufferPtr() const
{
    return &buffer_[filePos_];
}

size_t VirtualFile::SizeRHS() const
{
    return buffer_.size() - filePos_;
}

void VirtualFile::ReadString(std::string& str, const char terminator)
{
    while (!IsEOF())
    {
        auto chr = File::Read<char>();
        if (chr != terminator)
            str += chr;
        else
            break;
    }
}


} // /namespace IO

} // /namespace Fork



// ========================