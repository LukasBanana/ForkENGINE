/*
 * File stream helper functions header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_FILE_H__
#define __FORK_IO_FILE_H__


#include "Core/Export.h"
#include "Core/StaticConfig.h"

#include <type_traits>
#include <fstream>


namespace Fork
{

namespace IO
{


//! Returns the size (in bytes) of the specified file stream.
FORK_EXPORT std::streampos QueryFileSize(std::ifstream& stream);

//! Reads the entire file into a string.
FORK_EXPORT std::string ReadFileIntoString(std::ifstream& stream);

//! Returns true if the specified exists.
FORK_EXPORT bool FileExists(const std::string& filename);
//! Returns true if the specified exists.
FORK_EXPORT bool FileExists(const std::wstring& filename);


//! Reads the specified data from an input stream. The data type 'T' must not be a pointer!
template <typename T> void ReadFromStream(std::istream& stream, T& buffer)
{
    static_assert(!std::is_pointer<T>::value, "IO::ReadFromStream does not allow pointer types");
    stream.read(reinterpret_cast<char*>(&buffer), sizeof(T));
}

/**
Reads the specified data from an input stream. The data type 'T' must not be a pointer!
\throws std::out_of_range If 'size' is larger than the input buffer.
*/
template <typename T> void ReadFromStream(std::istream& stream, T& buffer, std::streamsize size)
{
    static_assert(!std::is_pointer<T>::value, "IO::ReadFromStream does not allow pointer types");
    #ifdef FORK_DEBUG
    if (sizeof(T) < size)
        throw std::out_of_range("In '" __FUNCTION__ "': size is larger than the input buffer");
    #endif
    stream.read(reinterpret_cast<char*>(&buffer), size);
}

//! Writes the specified data to an output stream. The data type 'T' must not be a pointer!
template <typename T> void WriteToStream(std::ostream& stream, const T& buffer)
{
    static_assert(!std::is_pointer<T>::value, "IO::WriteToStream does not allow pointer types");
    stream.write(reinterpret_cast<const char*>(&buffer), sizeof(T));
}

/**
Writes the specified data to an output stream. The data type 'T' must not be a pointer!
\throws std::out_of_range If 'size' is larger than the output buffer.
*/
template <typename T> void WriteToStream(std::ostream& stream, const T& buffer, std::streamsize size)
{
    static_assert(!std::is_pointer<T>::value, "IO::WriteToStream does not allow pointer types");
    #ifdef FORK_DEBUG
    if (sizeof(T) < size)
        throw std::out_of_range("In '" __FUNCTION__ "': size is larger than the ouput buffer");
    #endif
    stream.write(reinterpret_cast<const char*>(&buffer), size);
}


} // /namespace IO

} // /namespace Fork


#endif



// ========================