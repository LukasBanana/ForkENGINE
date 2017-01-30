/*
 * Physical file header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_PHYSICAL_FILE_H__
#define __FORK_IO_PHYSICAL_FILE_H__


#include "IO/FileSystem/File.h"

#include <fstream>


namespace Fork
{

namespace IO
{


DECL_SHR_PTR(PhysicalFile);

//! Physical file class.
class FORK_EXPORT PhysicalFile : public File
{
    
    public:
        
        PhysicalFile() = default;
        PhysicalFile(const std::string& filename, const OpenFlags::DataType flags = OpenFlags::ReadWrite);

        PhysicalFile(const PhysicalFile&) = delete;
        PhysicalFile& operator = (const PhysicalFile&) = delete;

        bool Open(const std::string& filename, const OpenFlags::DataType flags = OpenFlags::ReadWrite);
        void Close();

        bool IsOpen() const;

        size_t Pos();

        void SeekPos(std::streampos pos);
        void SeekPos(std::streamoff offset, const SeekDirections direction);

        bool IsEOF() const;

        void ReadBuffer(void* buffer, size_t size);
        void WriteBuffer(const void* buffer, size_t size);

        std::string ReadStringC();
        std::string ReadStringNL();

    private:

        std::fstream stream_;

};


} // /namespace IO

} // /namespace Fork


#endif



// ========================