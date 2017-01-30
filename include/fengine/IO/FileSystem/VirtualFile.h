/*
 * Virtual file header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_VIRTUAL_FILE_H__
#define __FORK_IO_VIRTUAL_FILE_H__


#include "IO/FileSystem/File.h"
#include "IO/FileSystem/PhysicalFile.h"

#include <vector>


namespace Fork
{

namespace IO
{


DECL_SHR_PTR(VirtualFile);

//! Virtual file class. Its file content only exists in the RAM.
class FORK_EXPORT VirtualFile : public File
{
    
    public:
        
        VirtualFile(const std::string& filename, const OpenFlags::DataType flags = OpenFlags::ReadWrite);

        VirtualFile(const VirtualFile&) = delete;
        VirtualFile& operator = (const VirtualFile&) = delete;

        bool Open(const std::string& filename, const OpenFlags::DataType flags = OpenFlags::ReadWrite);
        void Close();

        bool IsOpen() const;

        size_t Pos();

        void SeekPos(std::streampos pos);
        void SeekPos(std::streamoff offset, const SeekDirections direction);

        bool IsEOF() const;

        //! \throws NullPointerException If 'buffer' is null.
        void ReadBuffer(void* buffer, size_t size);
        //! \throws NullPointerException If 'buffer' is null.
        void WriteBuffer(const void* buffer, size_t size);

        std::string ReadStringC();
        std::string ReadStringNL();

        /**
        Writes this virtual file to the hard-disc-drive (HDD) with the current filename.
        \return Shared pointer to the physics file object or null if the file could not be created.
        */
        PhysicalFilePtr WriteToHDD();

    private:

        typedef std::vector<char> BufferType;
        typedef BufferType::size_type FilePosType;

        void ClampFilePos();

        //! Returns a pointer to the current file position.
        char* BufferPtr();
        //! Returns a constant pointer to the current file position.
        const char* BufferPtr() const;

        /**
        Returns the rest size of the right-hand-side (RHS) from the current file position.
        If the file is 10 bytes large and the file position is 0, the function return 10.
        If the file is 10 bytes large and the file position is 4, the function return 6.
        */
        size_t SizeRHS() const;

        void ReadString(std::string& str, const char terminator = '\n');

        std::string filename_;

        BufferType  buffer_;
        FilePosType filePos_ = 0;

};


} // /namespace IO

} // /namespace Fork


#endif



// ========================