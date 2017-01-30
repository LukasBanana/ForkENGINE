/*
 * Physical file source file
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#include "IO/FileSystem/PhysicalFile.h"
#include "IO/Core/Log.h"


namespace Fork
{

namespace IO
{


PhysicalFile::PhysicalFile(const std::string& filename, const OpenFlags::DataType flags)
{
    Open(filename, flags);
}

bool PhysicalFile::Open(const std::string& filename, const OpenFlags::DataType flags)
{
    if ((flags & OpenFlags::ReadWrite) == 0)
    {
        IO::Log::Error("Invalid open flags for file");
        return false;
    }

    /* Store open flags */
    flags_ = flags;

    bool r = HasReadAccess();
    bool w = HasWriteAccess();

    /* Open file */
    std::ios_base::openmode mode = 0;

    if ((flags & OpenFlags::Textual) == 0)
        mode |= std::fstream::binary;
    if (r)
        mode |= std::fstream::in;
    if (w)
        mode |= std::fstream::out;

    stream_.open(filename, mode);

    /* Check for errors */
    if (!stream_.good())
    {
        std::string err;
        
        if (r && w)
            err += "Opening";
        else if (r)
            err += "Reading";
        else if (w)
            err += "Writing";

        IO::Log::Error(err + " file \"" + filename + "\" failed");

        return false;
    }

    return true;
}

void PhysicalFile::Close()
{
    stream_.close();
}

bool PhysicalFile::IsOpen() const
{
    return stream_.is_open();
}

size_t PhysicalFile::Pos()
{
    return static_cast<size_t>(HasReadAccess() ? stream_.tellg() : stream_.tellp());
}

void PhysicalFile::SeekPos(std::streampos pos)
{
    /* Seek file position */
    if (HasReadAccess())
       stream_.seekg(pos);
    else
       stream_.seekp(pos);
}

void PhysicalFile::SeekPos(std::streamoff offset, const SeekDirections direction)
{
    /* Setup seek position */
    std::ios_base::seekdir way = std::ios_base::beg;

    switch (direction)
    {
        case SeekDirections::Begin:
            way = std::ios_base::beg;
            break;
        case SeekDirections::Current:
            way = std::ios_base::cur;
            break;
        case SeekDirections::End:
            way = std::ios_base::end;
            break;
    }

    /* Seek file position */
    if (HasReadAccess())
       stream_.seekg(offset, way);
    else
       stream_.seekp(offset, way);
}

bool PhysicalFile::IsEOF() const
{
    return stream_.eof();
}

void PhysicalFile::ReadBuffer(void* buffer, size_t size)
{
    if (HasReadAccess())
        stream_.read(reinterpret_cast<char*>(buffer), size);
}

void PhysicalFile::WriteBuffer(const void* buffer, size_t size)
{
    if (HasWriteAccess())
        stream_.write(reinterpret_cast<const char*>(buffer), size);
}

std::string PhysicalFile::ReadStringC()
{
    std::string str;
    std::getline(stream_, str, '\0');
    return str;
}

std::string PhysicalFile::ReadStringNL()
{
    std::string str;
    std::getline(stream_, str);
    return str;
}


} // /namespace IO

} // /namespace Fork



// ========================