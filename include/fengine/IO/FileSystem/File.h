/*
 * File header
 * 
 * This file is part of the "ForkENGINE" (Copyright (c) 2014 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef __FORK_IO_FILE_H__
#define __FORK_IO_FILE_H__


#include "Core/Export.h"
#include "Core/DeclPtr.h"

#include "Math/Core/Vector2.h"
#include "Math/Core/Vector3.h"
#include "Math/Core/Vector4.h"
#include "Math/Core/Matrix2.h"
#include "Math/Core/Matrix3.h"
#include "Math/Core/Matrix4.h"

#include "Video/Core/ColorRGB.h"
#include "Video/Core/ColorRGBA.h"

#include <string>


namespace Fork
{

namespace IO
{


DECL_SHR_PTR(File);

/**
File base class.
\see PhysicalFile
\see VirtualFile
*/
class FORK_EXPORT File
{
    
    public:
        
        //! File open flags.
        struct OpenFlags
        {
            typedef unsigned int DataType;
            enum : DataType
            {
                Read        = (1 << 0),         //!< File has read access.
                Write       = (1 << 1),         //!< File has write access.
                Textual     = (1 << 2),         //!< File will be read/write in texual mode. By default binary mode.
                ReadWrite   = (Read | Write),   //!< File has read and write access.
            };
        };

        //! File seek directions.
        enum class SeekDirections
        {
            Begin,      //!< Seek from the beginning of the file.
            Current,    //!< Seek from the current read/write position.
            End         //!< Seek from the end of the file.
        };

        virtual ~File();

        /* === Interface === */

        /**
        Opens the specified file with the specified open flags.
        \param[in] filename Specifies the file which is to be opened.
        \param[in] flags Specifies the open flags. This can be a combination of
        the entries of the "OpenFlags" enumeration. By default OpenFlags::ReadWrite.
        \return True if the file has opened successful. Otherwise an error occured which will be printed into the log output.
        */
        virtual bool Open(const std::string& filename, const OpenFlags::DataType flags = OpenFlags::ReadWrite) = 0;
        //! Closes the file. This will be called automatically when the file object will be destructed.
        virtual void Close() = 0;

        //! Returns true if the file was opend.
        virtual bool IsOpen() const = 0;

        //! Returns the file position.
        virtual size_t Pos() = 0;
        //! Seeks the file position.
        virtual void SeekPos(std::streampos pos) = 0;
        //! Seeks the file position into the specified direction.
        virtual void SeekPos(std::streamoff offset, const SeekDirections direction) = 0;

        //! Returns true if the end-of-file (EOF) has been reached.
        virtual bool IsEOF() const = 0;

        //! Reads data from the file and stores it into the specified buffer.
        virtual void ReadBuffer(void* buffer, size_t size) = 0;
        //! Writes the specified buffer into the file.
        virtual void WriteBuffer(const void* buffer, size_t size) = 0;

        /**
        Reads a sized string. This consists of a 4 byte unsigned integer
        for the string length (Excluding the null termination character),
        followed by the string characters (Also excluding the null termination character).
        */
        virtual std::string ReadStringSized();
        //! Reads a null-terminated (ANSI-C) string.
        virtual std::string ReadStringC() = 0;
        //! Reads a new-line character terminated string.
        virtual std::string ReadStringNL() = 0;

        /**
        Writes a sized string. This will write an 4 byte unsigned integer
        for the string length (Excluding the null termination character),
        followed by the string characters (Also excluding the null termination character).
        */
        virtual void WriteStringSized(const std::string& str);
        //! Write a null-terminated (ANSI-C) string.
        virtual void WriteStringC(const std::string& str);
        //! Write a new-line character terminated string.
        virtual void WriteStringNL(const std::string& str);

        //! Returns the open flags.
        inline OpenFlags::DataType GetFlags() const
        {
            return flags_;
        }

        //! Returns true if this file has read access.
        inline bool HasReadAccess() const
        {
            return (flags_ & OpenFlags::Read) != 0;
        }
        //! Returns true if this file has write access.
        inline bool HasWriteAccess() const
        {
            return (flags_ & OpenFlags::Write) != 0;
        }

        /* === Templates === */

        template <typename T> T inline Read()
        {
            static_assert(!std::is_pointer<T>::value, "\"" __FUNCTION__ "\" does not allow pointer types");
            T data;
            ReadBuffer(&data, sizeof(T));
            return data;
        }

        template <> bool inline Read<bool>()
        {
            return Read<char>() != 0;
        }

        template <typename T> inline void Write(const T& data)
        {
            static_assert(!std::is_pointer<T>::value, "\"" __FUNCTION__ "\" does not allow pointer types");
            WriteBuffer(&data, sizeof(T));
        }

        template <> inline void Write<bool>(const bool& data)
        {
            Write<char>(data ? 1 : 0);
        }

        template <typename T> inline Math::Vector2<T> ReadVec2()
        {
            return Read<Math::Vector2<T>>();
        }
        template <typename T> inline Math::Vector3<T> ReadVec3()
        {
            return Read<Math::Vector3<T>>();
        }
        template <typename T> inline Math::Vector4<T> ReadVec4()
        {
            return Read<Math::Vector4<T>>();
        }

        template <typename T> inline Math::Matrix2<T> ReadMat2()
        {
            return Read<Math::Matrix2<T>>();
        }
        template <typename T> inline Math::Matrix3<T> ReadMat3()
        {
            return Read<Math::Matrix3<T>>();
        }
        template <typename T> inline Math::Matrix4<T> ReadMat4()
        {
            return Read<Math::Matrix4<T>>();
        }

        template <typename T> inline Video::ColorRGB<T> ReadRGB()
        {
            return Read<Video::ColorRGB<T>>();
        }
        template <typename T> inline Video::ColorRGBA<T> ReadRGBA()
        {
            return Read<Video::ColorRGBA<T>>();
        }

    protected:

        File() = default;

        OpenFlags::DataType flags_ = 0;

};


} // /namespace IO

} // /namespace Fork


#endif



// ========================